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

#include "JavaClass.h"
#include "JavaClassUtils.h"
#include "JavaExceptionUtils.h"
#include <string.h>

namespace spotify {
namespace jni {

JavaClass::JavaClass() :
_clazz(NULL),
_default_constructor(NULL),
_methods(NULL),
_fields(NULL),
_clazz_global(NULL) {
  LOG_DEBUG("Creating new empty instance of class");
}

JavaClass::JavaClass(JNIEnv *env) :
_clazz(NULL),
_default_constructor(NULL),
_methods(NULL),
_fields(NULL),
_clazz_global(NULL) {
  // Ideally, we would like to call initialize() from the JavaClass() ctor.
  // However this won't work because initialize() is pure virtual, and such methods
  // cannot be called here because the object is in an incomplete state. So instead,
  // one needs to make sure that initialize(env) is called in the subclass' ctor.
  LOG_DEBUG("Initializing new instance of class");
  _methods = &_methods_global;
  _fields = &_fields_global;
}

JavaClass::~JavaClass() {
  LOG_DEBUG("Destroying instance of class");
}

bool JavaClass::isInitialized() const {
  return _clazz != NULL;
}

const char* JavaClass::getSimpleName() const {
  const char* lastSlash = strrchr(getCanonicalName(), '/');
  return lastSlash != NULL ? lastSlash + 1 : getCanonicalName();
}

void JavaClass::merge(const JavaClass *globalInstance) {
  LOG_DEBUG("Merging instance of '%s' with global class info", getSimpleName());
  _clazz = globalInstance->_clazz_global.get();
  _methods = &globalInstance->_methods_global;
  _fields = &globalInstance->_fields_global;
  _default_constructor = globalInstance->_default_constructor;
}

void JavaClass::setJavaObject(JNIEnv *env, jobject javaThis) {
  LOG_DEBUG("Setting fields from Java object of type '%s' to native instance", getSimpleName());

  // Set up field mappings, if this has not already been done
  if (_field_mappings.empty()) {
    mapFields();
  }

   for (FieldMap::const_iterator iter = _fields->begin(); iter != _fields->end(); ++iter) {
    std::string key = iter->first;
    LOG_DEBUG("Copying field '%s'", key.c_str());

    jfieldID field = iter->second;
    const FieldMapping *mapping = getFieldMapping(key.c_str());
    if (field != NULL && mapping != NULL) {
      if (TYPE_EQUALS(mapping->type, kTypeInt)) {
        int *address = static_cast<int*>(mapping->address);
        *address = env->GetIntField(javaThis, field);
      } else if (TYPE_EQUALS(mapping->type, kTypeShort)) {
        short *address = static_cast<short*>(mapping->address);
        *address = env->GetShortField(javaThis, field);
      } else if (TYPE_EQUALS(mapping->type, kTypeBool)) {
        bool *address = static_cast<bool*>(mapping->address);
        // The somewhat odd "? true : false" fixes a performance warning on windows,
        // since GetBooleanField actually returns jboolean, not bool.
        *address = env->GetBooleanField(javaThis, field) ? true : false;
      } else if (TYPE_EQUALS(mapping->type, kTypeFloat)) {
        float *address = static_cast<float*>(mapping->address);
        *address = env->GetFloatField(javaThis, field);
      } else if (TYPE_EQUALS(mapping->type, kTypeDouble)) {
        double *address = static_cast<double*>(mapping->address);
        *address = env->GetDoubleField(javaThis, field);
      } else if (TYPE_EQUALS(mapping->type, kTypeString)) {
        jstring string = (jstring)env->GetObjectField(javaThis, field);
        JavaString *address = static_cast<JavaString*>(mapping->address);
        address->set(env, string);
      } else if (TYPE_EQUALS(mapping->type, kTypeByte)) {
        unsigned char *address = static_cast<unsigned char*>(mapping->address);
        *address = env->GetByteField(javaThis, field);
      } else if (TYPE_EQUALS(mapping->type, kTypeChar)) {
        wchar_t *address = static_cast<wchar_t*>(mapping->address);
        *address = env->GetCharField(javaThis, field);
      } else {
        LOG_ERROR("Unable to copy data to field '%s'", key.c_str());
      }
    }
  }
}

jobject JavaClass::toJavaObject(JNIEnv *env) {
  LOG_DEBUG("Converting native instance of '%s' to Java instance", getSimpleName());

  if (_default_constructor == NULL) {
    JavaExceptionUtils::throwExceptionOfType(env, kTypeIllegalStateException,
      "Cannot call toJavaObject without a constructor (did you forget to call cacheConstructor() in initialize()?");
    return NULL;
  } else if (!isInitialized()) {
    JavaExceptionUtils::throwExceptionOfType(env, kTypeIllegalStateException,
      "Cannot call toJavaObject without registering class info");
    return NULL;
  }

  // Set up field mappings, if this has not already been done
  if (_field_mappings.empty()) {
    mapFields();
  }

  JniLocalRef<jobject> result;
  result.set(env->NewObject(_clazz, _default_constructor));
  return toJavaObject(env, result.leak());
}

jobject JavaClass::toJavaObject(JNIEnv *env, jobject javaThis) {
  for (FieldMap::const_iterator iter = _fields->begin(); iter != _fields->end(); ++iter) {
    std::string key = iter->first;
    LOG_DEBUG("Copying field %s", key.c_str());

    jfieldID field = iter->second;
    const FieldMapping *mapping = getFieldMapping(key.c_str());
    if (field != NULL && mapping != NULL) {
      if (TYPE_EQUALS(mapping->type, kTypeInt)) {
        int *address = static_cast<int*>(mapping->address);
        env->SetIntField(javaThis, field, *address);
      } else if (TYPE_EQUALS(mapping->type, kTypeShort)) {
        short *address = static_cast<short*>(mapping->address);
        env->SetShortField(javaThis, field, *address);
      } else if (TYPE_EQUALS(mapping->type, kTypeBool)) {
        bool *address = static_cast<bool*>(mapping->address);
        env->SetBooleanField(javaThis, field, *address);
      } else if (TYPE_EQUALS(mapping->type, kTypeFloat)) {
        float *address = static_cast<float*>(mapping->address);
        env->SetFloatField(javaThis, field, *address);
      } else if (TYPE_EQUALS(mapping->type, kTypeDouble)) {
        double *address = static_cast<double*>(mapping->address);
        env->SetDoubleField(javaThis, field, *address);
      } else if (TYPE_EQUALS(mapping->type, kTypeString)) {
        JavaString *address = static_cast<JavaString*>(mapping->address);
        JniLocalRef<jstring> string = address->toJavaString(env);
        env->SetObjectField(javaThis, field, string.get());
      } else if (TYPE_EQUALS(mapping->type, kTypeByte)) {
        unsigned char *address = static_cast<unsigned char*>(mapping->address);
        env->SetByteField(javaThis, field, *address);
      } else if (TYPE_EQUALS(mapping->type, kTypeChar)) {
        wchar_t *address = static_cast<wchar_t*>(mapping->address);
        env->SetCharField(javaThis, field, *address);
      } else {
        LOG_ERROR("Unable to copy data to field '%s'", key.c_str());
      }
    }
  }

  return javaThis;
}

jmethodID JavaClass::getMethod(const char *method_name) const {
  if (!isInitialized()) {
    JavaExceptionUtils::throwExceptionOfType(JavaThreadUtils::getEnvForCurrentThread(),
      kTypeIllegalStateException,
      "Cannot call getMethod without class info (forgot to merge?)");
    return NULL;
  }

  const std::string key(method_name);
  MethodMap::const_iterator mapFindIter = _methods->find(key);
  if (mapFindIter == _methods->end()) {
    JavaExceptionUtils::throwExceptionOfType(JavaThreadUtils::getEnvForCurrentThread(),
      kTypeIllegalArgumentException,
      "Method '%s' is not cached in class '%s'", method_name, getCanonicalName());
    return NULL;
  }

  return mapFindIter->second;
}

jfieldID JavaClass::getField(const char* field_name) const {
  if (!isInitialized()) {
    JavaExceptionUtils::throwExceptionOfType(JavaThreadUtils::getEnvForCurrentThread(),
      kTypeIllegalStateException,
      "Cannot call getField without class info (forgot to merge?)");
    return NULL;
  }

  const std::string key(field_name);
  FieldMap::const_iterator mapFindIter = _fields->find(key);
  if (mapFindIter == _fields->end()) {
    JavaExceptionUtils::throwExceptionOfType(JavaThreadUtils::getEnvForCurrentThread(),
      kTypeIllegalArgumentException,
      "Field '%s' is not cached in class '%s'", field_name, getCanonicalName());
    return NULL;
  }

  return mapFindIter->second;
}

void JavaClass::setClass(JNIEnv *env) {
  LOG_INFO("Looking up corresponding Java class for '%s'", getCanonicalName());
  _clazz_global.set(env->FindClass(getCanonicalName()));
  JavaExceptionUtils::checkException(env);
  _clazz = _clazz_global.get();
}

void JavaClass::cacheConstructor(JNIEnv *env) {
  LOG_DEBUG("Caching constructor in class '%s'", getSimpleName());
  if (!isInitialized()) {
    JavaExceptionUtils::throwExceptionOfType(env, kTypeIllegalStateException,
      "Attempt to call cacheMethod without having set class info");
    return;
  }

  std::string signature;
  JavaClassUtils::makeSignature(signature, kTypeVoid, NULL);
  _default_constructor = env->GetMethodID(_clazz_global.get(), "<init>", signature.c_str());
  JavaExceptionUtils::checkException(env);
}

void JavaClass::cacheMethod(JNIEnv *env, const char* method_name, const char* return_type, ...) {
  LOG_DEBUG("Caching method '%s' in class '%s'", method_name, getSimpleName());
  if (!isInitialized()) {
    JavaExceptionUtils::throwExceptionOfType(env, kTypeIllegalStateException,
      "Attempt to call cacheMethod without having set class info");
    return;
  }

  va_list arguments;
  va_start(arguments, return_type);
  std::string signature;
  JavaClassUtils::makeSignatureWithList(signature, return_type, arguments);
  va_end(arguments);

  jmethodID method = env->GetMethodID(_clazz_global.get(), method_name, signature.c_str());
  JavaExceptionUtils::checkException(env);
  if (method != NULL) {
    _methods_global[method_name] = method;
  } else {
    JavaExceptionUtils::throwExceptionOfType(env, kTypeJavaClass(NoSuchMethodError),
      "Method '%s' (signature: %s) not found on class '%s'",
      method_name, signature.c_str(), getCanonicalName());
  }
}

void JavaClass::cacheField(JNIEnv *env, const char *field_name, const char *field_type) {
  LOG_DEBUG("Caching field '%s' (type %s) in class '%s'", field_name, field_type, getSimpleName());
  if (!isInitialized()) {
    JavaExceptionUtils::throwExceptionOfType(env, kTypeIllegalStateException,
      "Attempt to call cacheField without having set class info");
    return;
  }

  std::string fieldTypeSignature;
  JavaClassUtils::makeNameForSignature(fieldTypeSignature, field_type);
  jfieldID field = env->GetFieldID(_clazz_global.get(), field_name, fieldTypeSignature.c_str());
  JavaExceptionUtils::checkException(env);
  if (field != NULL) {
    _fields_global[field_name] = field;
  } else {
    JavaExceptionUtils::throwExceptionOfType(env, kTypeJavaClass(NoSuchFieldError),
      "Field '%s' (type '%s') not found on class %s",
      field_name, field_type, getCanonicalName());
  }
}

void JavaClass::mapField(const char *field_name, const char *field_type, void *field_ptr) {
  FieldMapping *mapping = new FieldMapping;
  mapping->type = field_type;
  mapping->address = field_ptr;
  _field_mappings[field_name].set(mapping);
}

const FieldMapping* JavaClass::getFieldMapping(const char *key) const {
  std::string keyString(key);
  const FieldMappingMap::const_iterator findMapIter = _field_mappings.find(keyString);
  return findMapIter != _field_mappings.end() ? findMapIter->second.get() : NULL;
}

void JavaClass::addNativeMethod(const char *method_name, void *function, const char *return_type, ...) {
  LOG_DEBUG("Adding native method '%s' to class '%s'", method_name, getSimpleName());
  JNINativeMethod nativeMethod;
  nativeMethod.name = const_cast<char*>(method_name);
  nativeMethod.fnPtr = function;

  va_list arguments;
  va_start(arguments, return_type);
  std::string signature;
  JavaClassUtils::makeSignatureWithList(signature, return_type, arguments);
  nativeMethod.signature = const_cast<char*>(strdup(signature.c_str()));
  LOG_DEBUG("Native signature is '%s'", nativeMethod.signature);
  va_end(arguments);

  _jni_methods.push_back(nativeMethod);
}

bool JavaClass::registerNativeMethods(JNIEnv *env) {
  LOG_DEBUG("Registering native methods on class '%s'", getSimpleName());
  if (_jni_methods.empty()) {
    return false;
  }

  if (!isInitialized()) {
    JavaExceptionUtils::throwRuntimeException(env, "Could not find cached class for %s", getCanonicalName());
    return false;
  }

  bool result = (env->RegisterNatives(_clazz_global.get(), &_jni_methods[0], (jint)_jni_methods.size()) < 0);
  _jni_methods.clear();
  return result;
}

} // namespace jni
} // namespace spotify
