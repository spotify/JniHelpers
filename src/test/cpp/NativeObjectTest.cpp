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

#include "NativeObjectTest.h"
#include "JUnitUtils.h"
#include "PersistedObject.h"
#include "TestObject.h"

void NativeObjectTest::initialize(JNIEnv *env) {
  setClass(env);

  PersistedObject persistedObject;
  const char* persistedObjectName = persistedObject.getCanonicalName();

  addNativeMethod("createPersistedObject", (void*)&createPersistedObject, persistedObjectName, NULL);
  addNativeMethod("getPersistedInstance", (void*)&getPersistedInstance, persistedObjectName, persistedObjectName, NULL);
  addNativeMethod("nativeIsPersistenceEnabled", (void*)nativeIsPersistenceEnabled, kTypeVoid, NULL);
  addNativeMethod("isPersistenceEnabledWithoutInit", (void*)isPersistenceEnabledWithoutInit, kTypeVoid, NULL);
  addNativeMethod("destroyPersistedObject", (void*)&destroyPersistedObject, kTypeVoid, persistedObjectName, NULL);
  addNativeMethod("persistInvalidClass", (void*)&persistInvalidClass, kTypeVoid, NULL);
  addNativeMethod("persistNullObject", (void*)&persistNullObject, kTypeVoid, NULL);
  addNativeMethod("destroyInvalidClass", (void*)&destroyInvalidClass, kTypeVoid, NULL);
  addNativeMethod("destroyNullObject", (void*)&destroyNullObject, kTypeVoid, NULL);

  registerNativeMethods(env);
}

jobject NativeObjectTest::createPersistedObject(JNIEnv *env, jobject javaThis) {
  LOG_INFO("Starting test: createPersistedObject");
  PersistedObject *persistedObject = new PersistedObject(env);
  persistedObject->i = TEST_INTEGER;
  // Persist should be called for us here. Note that the original object is leaked; it will
  // be cleaned up in destroyPersistedObject().
  return persistedObject->toJavaObject(env);
}

jobject NativeObjectTest::getPersistedInstance(JNIEnv *env, jobject javaThis, jobject object) {
  LOG_INFO("Starting test: getPersistedInstance");
  ClassRegistry registry;
  registry.add(env, new PersistedObject(env));
  PersistedObject *persistedObject = registry.getNativeInstance<PersistedObject>(env, object);
  JUNIT_ASSERT_EQUALS_INT(TEST_INTEGER, persistedObject->i);
  JUNIT_ASSERT_NOT_NULL(persistedObject->getCanonicalName());
  JUNIT_ASSERT_TRUE(persistedObject->isInitialized());
  return persistedObject->toJavaObject(env);
}

void NativeObjectTest::nativeIsPersistenceEnabled(JNIEnv *env, jobject javaThis) {
  LOG_INFO("Starting test: nativeIsPersistenceEnabled");
  PersistedObject persistedObject(env);
  JUNIT_ASSERT_TRUE(persistedObject.isInitialized());
  PersistedObject mergedObject;
  mergedObject.merge(&persistedObject);
  JUNIT_ASSERT_TRUE(mergedObject.isInitialized());
}

void NativeObjectTest::isPersistenceEnabledWithoutInit(JNIEnv *env, jobject javaThis) {
  PersistedObject persistedObject;
  JUNIT_ASSERT_FALSE(persistedObject.isInitialized());
}

void NativeObjectTest::destroyPersistedObject(JNIEnv *env, jobject javaThis, jobject object) {
  LOG_INFO("Starting test: destroyPersistedObject");
  ClassRegistry registry;
  registry.add(env, new PersistedObject(env));
  PersistedObject *persistedObject = registry.getNativeInstance<PersistedObject>(env, object);
  persistedObject->destroy(env, object);
}

void NativeObjectTest::persistInvalidClass(JNIEnv *env, jobject javaThis) {
  LOG_INFO("Starting test: persistInvalidClass");
  PersistedObject persistedObject(env);
  persistedObject.mapFields();
  persistedObject.persist(env, javaThis);
}

void NativeObjectTest::persistNullObject(JNIEnv *env, jobject javaThis) {
  LOG_INFO("Starting test: persistNullObject");
  PersistedObject persistedObject(env);
  persistedObject.mapFields();
  JUNIT_ASSERT_FALSE(persistedObject.persist(env, NULL));
}

void NativeObjectTest::destroyInvalidClass(JNIEnv *env, jobject javaThis) {
  // This test is almost impossible to replicate from Java, and frankly should
  // not happen from (responsible) C++ code either. It would be possible to catch
  // if we are willing to do fieldID lookups on the fly rather than cached, but
  // that assumes that performance is not an issue here. For that reason, this
  // test is excluded and the erroneous behavior will (and probably should) crash
  // the JVM if enabled.
#if 0
  LOG_INFO("Starting test: destroyInvalidClass");
  PersistedObject persistedObject(env);
  persistedObject.mapFields();
  persistedObject.destroy(env, javaThis);
#endif
}

void NativeObjectTest::destroyNullObject(JNIEnv *env, jobject javaThis) {
  LOG_INFO("Starting test: destroyNullObject");
  PersistedObject persistedObject(env);
  persistedObject.mapFields();
  persistedObject.destroy(env, NULL);
}
