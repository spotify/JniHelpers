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

#include "JavaClassTest.h"
#include "JUnitUtils.h"
#include "TestObject.h"

void JavaClassTest::initialize(JNIEnv *env) {
  setClass(env);

  TestObject testObject;
  const char* testObjectName = testObject.getCanonicalName();

  addNativeMethod("createJavaClass", (void*)&createJavaClass, kTypeVoid, NULL);
  addNativeMethod("isInitialized", (void*)&nativeIsInitialized, kTypeVoid, NULL);
  addNativeMethod("getCanonicalName", (void*)&testGetCanonicalName, kTypeVoid, NULL);
  addNativeMethod("getSimpleName", (void*)&testGetSimpleName, kTypeVoid, NULL);
  addNativeMethod("merge", (void*)&testMerge, kTypeVoid, NULL);
  addNativeMethod("nativeSetJavaObject", (void*)&nativeSetJavaObject, kTypeVoid, testObjectName, NULL);
  addNativeMethod("nativeToJavaObject", (void*)&nativeToJavaObject, testObjectName, NULL);
  addNativeMethod("getCachedMethod", (void*)&getCachedMethod, kTypeVoid, NULL);
  addNativeMethod("getInvalidCachedMethod", (void*)&getInvalidCachedMethod, kTypeVoid, NULL);
  addNativeMethod("getCachedMethodOnUninitialized", (void*)&getCachedMethodOnUninitialized, kTypeVoid, NULL);
  addNativeMethod("getCachedField", (void*)&getCachedField, kTypeVoid, NULL);
  addNativeMethod("getInvalidCachedField", (void*)&getInvalidCachedField, kTypeVoid, NULL);
  addNativeMethod("getCachedFieldOnUninitialized", (void*)&getCachedFieldOnUninitialized, kTypeVoid, NULL);
  addNativeMethod("cacheInvalidField", (void*)&cacheInvalidField, kTypeVoid, NULL);
  addNativeMethod("cacheInvalidMethod", (void*)&cacheInvalidMethod, kTypeVoid, NULL);

  registerNativeMethods(env);
}

void JavaClassTest::createJavaClass(JNIEnv *env, jobject javaThis) {
  // For objects created with the no-arg constructor, we would not expect them
  // to have class info, but they should always return a valid canonical name.
  TestObject emptyObject;
  JUNIT_ASSERT_FALSE(emptyObject.isInitialized());
  JUNIT_ASSERT_NOT_NULL(emptyObject.getCanonicalName());
  JUNIT_ASSERT_NOT_NULL(emptyObject.getSimpleName());

  JUNIT_ASSERT_EQUALS_INT(0, emptyObject.i);
  JUNIT_ASSERT_EQUALS_INT(0, emptyObject.s);
  JUNIT_ASSERT_EQUALS_FLOAT(0.0f, emptyObject.f, DEFAULT_FLOAT_TOLERANCE);
  JUNIT_ASSERT_EQUALS_FLOAT(0.0, emptyObject.d, DEFAULT_FLOAT_TOLERANCE);
  JUNIT_ASSERT_EQUALS_STRING("", emptyObject.string.get());
  JUNIT_ASSERT_EQUALS_BOOL(false, emptyObject.z);
  JUNIT_ASSERT_EQUALS_INT(0, emptyObject.b);
  JUNIT_ASSERT_EQUALS_INT(0, emptyObject.c);

  // When creating with a JNIEnv*, initialize() should be called and class info
  // will be populated.
  TestObject infoObject(env);
  JUNIT_ASSERT_TRUE(infoObject.isInitialized());
  JUNIT_ASSERT_NOT_NULL(infoObject.getField("i"));
  JUNIT_ASSERT_NOT_NULL(infoObject.getMethod("getI"));
}

void JavaClassTest::nativeIsInitialized(JNIEnv *env, jobject javaThis) {
  LOG_INFO("Starting test: nativeIsInitialized");
  TestObject testObject;
  JUNIT_ASSERT_FALSE(testObject.isInitialized());
  testObject.initialize(env);
  JUNIT_ASSERT_TRUE(testObject.isInitialized());
}

void JavaClassTest::testGetCanonicalName(JNIEnv *env, jobject javaThis) {
  LOG_INFO("Starting test: testGetCanonicalName");
  // Should always be valid, even with no-arg ctor
  TestObject testObject;
  JUNIT_ASSERT_FALSE(testObject.isInitialized());
  JUNIT_ASSERT_NOT_NULL(testObject.getCanonicalName());
  JUNIT_ASSERT_EQUALS_CSTRING("com/spotify/jni/TestObject", testObject.getCanonicalName());
}

void JavaClassTest::testGetSimpleName(JNIEnv *env, jobject javaThis) {
  LOG_INFO("Starting test: testGetSimpleName");
  // Should always be valid, even with no-arg ctor
  TestObject testObject;
  JUNIT_ASSERT_FALSE(testObject.isInitialized());
  JUNIT_ASSERT_NOT_NULL(testObject.getSimpleName());
  JUNIT_ASSERT_EQUALS_CSTRING("TestObject", testObject.getSimpleName());
}

void JavaClassTest::testMerge(JNIEnv *env, jobject javaThis) {
  LOG_INFO("Starting test: testMerge");
  TestObject testObject(env);
  JUNIT_ASSERT_TRUE(testObject.isInitialized());
  JUNIT_ASSERT_NOT_NULL(testObject.getField("i"));
  JUNIT_ASSERT_NOT_NULL(testObject.getMethod("getI"));

  TestObject mergeObject;
  JUNIT_ASSERT_FALSE(mergeObject.isInitialized());
  mergeObject.merge(&testObject);
  JUNIT_ASSERT_TRUE(mergeObject.isInitialized());
  JUNIT_ASSERT_NOT_NULL(mergeObject.getField("i"));
  JUNIT_ASSERT_NOT_NULL(mergeObject.getMethod("getI"));
}

void JavaClassTest::nativeSetJavaObject(JNIEnv *env, jobject javaThis, jobject object) {
  LOG_INFO("Starting test: nativeSetJavaObject");
  TestObject testObject(env);
  testObject.setJavaObject(env, object);
  JUNIT_ASSERT_EQUALS_STRING(TEST_STRING, testObject.string.get());
  JUNIT_ASSERT_EQUALS_INT(TEST_INTEGER, testObject.i);
  JUNIT_ASSERT_EQUALS_INT(TEST_SHORT, testObject.s);
  JUNIT_ASSERT_EQUALS_FLOAT(TEST_FLOAT, testObject.f, DEFAULT_FLOAT_TOLERANCE);
  JUNIT_ASSERT_EQUALS_FLOAT(TEST_DOUBLE, testObject.d, DEFAULT_FLOAT_TOLERANCE);
  JUNIT_ASSERT_EQUALS_BOOL(TEST_BOOLEAN, testObject.z);
  JUNIT_ASSERT_EQUALS_INT(TEST_BYTE, testObject.b);
#if HAS_RAW_STRING_LITERALS
  JUNIT_ASSERT_EQUALS_INT(TEST_CHAR, testObject.c);
#endif
}

jobject JavaClassTest::nativeToJavaObject(JNIEnv *env, jobject javaThis) {
  LOG_INFO("Starting test: nativeToJavaObject");
  TestObject testObject(env);
  testObject.string.set(TEST_STRING);
  testObject.i = TEST_INTEGER;
  testObject.s = TEST_SHORT;
  testObject.f = TEST_FLOAT;
  testObject.d = TEST_DOUBLE;
  testObject.z = TEST_BOOLEAN;
  testObject.b = TEST_BYTE;
#if HAS_RAW_STRING_LITERALS
  testObject.c = TEST_CHAR;
#endif
  return testObject.toJavaObject(env);
}

void JavaClassTest::getCachedMethod(JNIEnv *env, jobject javaThis) {
  LOG_INFO("Starting test: getCachedMethod");
  TestObject testObject(env);
  jmethodID method = testObject.getMethod("getString");
  JUNIT_ASSERT_NOT_NULL(method);
}

void JavaClassTest::getInvalidCachedMethod(JNIEnv *env, jobject javaThis) {
  LOG_INFO("Starting test: getInvalidCachedMethod");
  TestObject testObject(env);
  jmethodID method = testObject.getMethod("invalid");
  JUNIT_ASSERT_NULL(method);
}

void JavaClassTest::getCachedMethodOnUninitialized(JNIEnv *env, jobject javaThis) {
  LOG_INFO("Starting test: getCachedMethodOnUninitialized");
  TestObject testObject;
  jmethodID method = testObject.getMethod("getString");
  JUNIT_ASSERT_NULL(method);
}

void JavaClassTest::getCachedField(JNIEnv *env, jobject javaThis) {
  LOG_INFO("Starting test: getCachedField");
  TestObject testObject(env);
  jfieldID field = testObject.getField("string");
  JUNIT_ASSERT_NOT_NULL(field);
}

void JavaClassTest::getInvalidCachedField(JNIEnv *env, jobject javaThis) {
  LOG_INFO("Starting test: getInvalidCachedField");
  TestObject testObject(env);
  jfieldID field = testObject.getField("invalid");
  JUNIT_ASSERT_NULL(field);
}

void JavaClassTest::getCachedFieldOnUninitialized(JNIEnv *env, jobject javaThis) {
  LOG_INFO("Starting test: getCachedFieldOnUninitialized");
  TestObject testObject;
  jfieldID field = testObject.getField("string");
  JUNIT_ASSERT_NULL(field);
}

class CacheInvalidField : public TestObject {
public:
  CacheInvalidField() : TestObject() {}
  virtual void initialize(JNIEnv *env) {
    TestObject::initialize(env);
    cacheField(env, "invalid", kTypeString);
  }
};

void JavaClassTest::cacheInvalidField(JNIEnv *env, jobject javaThis) {
  LOG_INFO("Starting testn: cacheInvalidField");
  CacheInvalidField testObject;
  testObject.initialize(env);
}

class CacheInvalidMethod : public TestObject {
public:
  CacheInvalidMethod() : TestObject() {}
  virtual void initialize(JNIEnv *env) {
    TestObject::initialize(env);
    cacheMethod(env, "invalid", kTypeVoid, NULL);
  }
};

void JavaClassTest::cacheInvalidMethod(JNIEnv *env, jobject javaThis) {
  LOG_INFO("Starting test: cacheInvalidMethod");
  CacheInvalidMethod testObject;
  testObject.initialize(env);
}
