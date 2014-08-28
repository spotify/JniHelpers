/*
 * Copyright (c) 2014 Spotify AB
 *
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 */

#include "NativeObject.h"
#include "JavaClassUtils.h"
#include "JavaExceptionUtils.h"

namespace spotify {
namespace jni {

NativeObject::NativeObject() {}
NativeObject::NativeObject(JNIEnv *env) : JavaClass(env) {}
NativeObject::~NativeObject() {}

bool NativeObject::isInitialized() const {
  bool result = JavaClass::isInitialized();
  if (result) {
    // We expect the persisted field to be cached, otherwise searching for persisted
    // fields in non-persisted classes will throw java.lang.NoSuchField exception. :(
    const std::string key(PERSIST_FIELD_NAME);
    FieldMap::const_iterator mapFindIter = _fields->find(key);
    return mapFindIter != _fields->end();
  }
  return false;
}

jobject NativeObject::toJavaObject(JNIEnv *env) {
  return JavaClass::toJavaObject(env);
}

jobject NativeObject::toJavaObject(JNIEnv *env, jobject javaThis) {
  jobject result = JavaClass::toJavaObject(env, javaThis);
  // Persist the current object address to the Java instance
  if (persist(env, javaThis)) {
    return result;
  }
  return NULL;
}

bool NativeObject::persist(JNIEnv *env, jobject javaThis) {
  if (isInitialized()) {
    LOG_DEBUG("Persisting instance of '%s' to Java object", getSimpleName());
    if (javaThis == NULL) {
      JavaExceptionUtils::throwExceptionOfType(env, kTypeIllegalArgumentException,
        "Cannot persist object without corresponding Java instance");
      return false;
    }
    jlong resultPtr = reinterpret_cast<jlong>(this);
    env->SetLongField(javaThis, getField(PERSIST_FIELD_NAME), resultPtr);
    JavaExceptionUtils::checkException(env);
    LOG_DEBUG("Persist was successful");
    return true;
  }
  return false;
}

JavaClass* NativeObject::getPersistedInstance(JNIEnv *env, jobject javaThis) const {
  if (isInitialized()) {
    LOG_DEBUG("Retrieving persisted instance of '%s'", getSimpleName());
    jlong resultPtr = env->GetLongField(javaThis, getField(PERSIST_FIELD_NAME));
    return reinterpret_cast<JavaClass*>(resultPtr);
  } else {
    return NULL;
  }
}

void NativeObject::destroy(JNIEnv *env, jobject javaThis) {
  if (isInitialized()) {
    LOG_DEBUG("Destroying persisted instance of '%s'", getSimpleName());
    if (javaThis == NULL) {
      JavaExceptionUtils::throwExceptionOfType(env, kTypeIllegalArgumentException,
        "Cannot destroy persisted object without corresponding Java instance");
      return;
    }

    jfieldID persistField = getField(PERSIST_FIELD_NAME);
    if (persistField == NULL) {
      JavaExceptionUtils::throwExceptionOfType(env, kTypeIllegalStateException,
        "Cannot destroy, object lacks persist field");
      return;
    }

    jlong resultPtr = env->GetLongField(javaThis, persistField);
    JavaClass *instance = reinterpret_cast<JavaClass*>(resultPtr);
    if (instance != NULL) {
      delete instance;
      env->SetLongField(javaThis, persistField, 0);
    }
  }
}

void NativeObject::setClass(JNIEnv *env) {
  JavaClass::setClass(env);
  // We can't call cacheField() here because it checks isInitialized(), which we
  // have overridden to check to see that the persist field is present...
  std::string fieldTypeSignature;
  JavaClassUtils::makeNameForSignature(fieldTypeSignature, kTypeLong);
  jfieldID field = env->GetFieldID(_clazz_global.get(), PERSIST_FIELD_NAME, fieldTypeSignature.c_str());
  JavaExceptionUtils::checkException(env);
  if (field != NULL) {
    _fields_global[PERSIST_FIELD_NAME] = field;
  } else {
    JavaExceptionUtils::throwExceptionOfType(env, kTypeJavaClass(NoSuchFieldError),
      "Persisted field '%s' (type '%s') not found on class %s",
      PERSIST_FIELD_NAME, kTypeLong, getCanonicalName());
  }
}

} // namespace jni
} // namespace spotify
