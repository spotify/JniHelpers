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

#include "ClassRegistryTest.h"
#include "JUnitUtils.h"
#include "TestObject.h"

/*
 * NOTE: In this test class, JavaClass objects are created on the fly with the
 * constructor which takes JNIEnv*. Normally you should *not* do this; that ctor
 * is only meant to be called during initialization. However, the initialization
 * step isn't really present in these tests, so objects are created in this manner
 * simply to populate them with the correct class information.
 */

void ClassRegistryTest::initialize(JNIEnv *env) {
  setClass(env);

  addNativeMethod("createRegistry", (void*)createRegistry, kTypeVoid, NULL);

  addNativeMethod("addClass", (void*)addClass, kTypeVoid, NULL);
  addNativeMethod("addNullClass", (void*)addNullClass, kTypeVoid, NULL);
  addNativeMethod("addClassWithEmptyName", (void*)addClassWithEmptyName, kTypeVoid, NULL);
  addNativeMethod("addClassWithNullName", (void*)addClassWithNullName, kTypeVoid, NULL);
  addNativeMethod("addClassWithoutInfo", (void*)addClassWithoutInfo, kTypeVoid, NULL);
  addNativeMethod("addClassMultipleTimes", (void*)addClassMultipleTimes, kTypeVoid, NULL);

  addNativeMethod("get", (void*)get, kTypeVoid, NULL);
  addNativeMethod("getWithBracketOperator", (void*)getWithBracketOperator, kTypeVoid, NULL);
  addNativeMethod("getNullClass", (void*)getNullClass, kTypeVoid, NULL);
  addNativeMethod("getInvalidClass", (void*)getInvalidClass, kTypeVoid, NULL);

  TestObject testObject;
  addNativeMethod("nativeNewInstance", (void*)nativeNewInstance, kTypeVoid, testObject.getCanonicalName(), NULL);
  addNativeMethod("nativeNewInstanceWithNull", (void*)nativeNewInstanceWithNull, kTypeVoid, testObject.getCanonicalName(), NULL);

  registerNativeMethods(env);
}

void ClassRegistryTest::createRegistry(JNIEnv *env, jobject javaThis) {
  LOG_INFO("Starting test: createRegistry");
  ClassRegistry registry;
  JUNIT_ASSERT_EQUALS_INT(0, registry.size());
}

void ClassRegistryTest::addClass(JNIEnv *env, jobject javaThis) {
  LOG_INFO("Starting test: addClass");
  ClassRegistry registry;
  JUNIT_ASSERT_EQUALS_INT(0, registry.size());
  registry.add(env, new TestObject(env));
  JUNIT_ASSERT_EQUALS_INT(1, registry.size());
}

void ClassRegistryTest::addNullClass(JNIEnv *env, jobject javaThis) {
  LOG_INFO("Starting test: addNullClass");
  ClassRegistry registry;
  JUNIT_ASSERT_EQUALS_INT(0, registry.size());
  registry.add(env, NULL);
  JUNIT_ASSERT_EQUALS_INT(0, registry.size());
}

void ClassRegistryTest::addClassWithEmptyName(JNIEnv *env, jobject javaThis) {
  LOG_INFO("Starting test: addClassWithEmptyName");
  ClassRegistry registry;
  registry.add(env, new ClassWithName(""));
}

void ClassRegistryTest::addClassWithNullName(JNIEnv *env, jobject javaThis) {
  LOG_INFO("Starting test: addClassWithNullName");
  ClassRegistry registry;
  registry.add(env, new ClassWithName(static_cast<const char*>(NULL)));
}

void ClassRegistryTest::addClassWithoutInfo(JNIEnv *env, jobject javaThis) {
  LOG_INFO("Starting test: addClassWithoutInfo");
  ClassRegistry registry;
  registry.add(env, new ClassWithName("invalid"));
}

void ClassRegistryTest::addClassMultipleTimes(JNIEnv *env, jobject javaThis) {
  LOG_INFO("Starting test: addClassMultipleTimes");
  ClassRegistry registry;
  JUNIT_ASSERT_EQUALS_INT(0, registry.size());
  registry.add(env, new TestObject(env));
  JUNIT_ASSERT_EQUALS_INT(1, registry.size());
  registry.add(env, new TestObject(env));
  // Should not be inserted multiple times
  JUNIT_ASSERT_EQUALS_INT(1, registry.size());
}

void ClassRegistryTest::get(JNIEnv *env, jobject javaThis) {
  LOG_INFO("Starting test: get");
  ClassRegistry registry;
  TestObject dummy;

  JUNIT_ASSERT_EQUALS_INT(0, registry.size());
  const TestObject *shouldBeNull = dynamic_cast<const TestObject*>(registry.get(dummy.getCanonicalName()));
  JUNIT_ASSERT_NULL(shouldBeNull);

  registry.add(env, new TestObject(env));
  JUNIT_ASSERT_EQUALS_INT(1, registry.size());
  const TestObject *result = dynamic_cast<const TestObject*>(registry.get(dummy.getCanonicalName()));
  JUNIT_ASSERT_NOT_NULL(result);
  // Sanity check just to make sure that the returned result matches our test object
  JUNIT_ASSERT_EQUALS_CSTRING(dummy.getCanonicalName(), result->getCanonicalName());
}

void ClassRegistryTest::getWithBracketOperator(JNIEnv *env, jobject javaThis) {
  LOG_INFO("Starting test: getWithBracketOperator");
  ClassRegistry registry;
  TestObject dummy;

  JUNIT_ASSERT_EQUALS_INT(0, registry.size());
  const TestObject *shouldBeNull = dynamic_cast<const TestObject*>(registry[dummy.getCanonicalName()]);
  JUNIT_ASSERT_NULL(shouldBeNull);

  registry.add(env, new TestObject(env));
  JUNIT_ASSERT_EQUALS_INT(1, registry.size());
  const TestObject *result = dynamic_cast<const TestObject*>(registry[dummy.getCanonicalName()]);
  JUNIT_ASSERT_NOT_NULL(result);
  // Sanity check just to make sure that the returned result matches our test object
  JUNIT_ASSERT_EQUALS_CSTRING(dummy.getCanonicalName(), result->getCanonicalName());
}

void ClassRegistryTest::getNullClass(JNIEnv *env, jobject javaThis) {
  LOG_INFO("Starting test: getNullClass");
  ClassRegistry registry;
  // Should throw
  JUNIT_ASSERT_NULL(registry.get(NULL));
}

void ClassRegistryTest::getInvalidClass(JNIEnv *env, jobject javaThis) {
  LOG_INFO("Starting test: getInvalidClass");
  ClassRegistry registry;
  // Should throw
  JUNIT_ASSERT_NULL(registry.get("invalid"));
}

void ClassRegistryTest::nativeNewInstance(JNIEnv *env, jobject javaThis, jobject javaTestObject) {
  LOG_INFO("Starting test: nativeNewInstance");
  ClassRegistry registry;
  registry.add(env, new TestObject(env));
  TestObject *result = registry.newInstance<TestObject>(env, javaTestObject);
  JUNIT_ASSERT_NOT_NULL(result);
  JUNIT_ASSERT_EQUALS_INT(TEST_INTEGER, result->i);
  JUNIT_ASSERT_EQUALS_INT(TEST_SHORT, result->s);
  JUNIT_ASSERT_EQUALS_FLOAT(TEST_FLOAT, result->f, TEST_FLOAT_TOLERANCE);
  JUNIT_ASSERT_EQUALS_FLOAT(TEST_DOUBLE, result->d, TEST_FLOAT_TOLERANCE);
  JUNIT_ASSERT_EQUALS_BOOL(TEST_BOOLEAN, result->z);
  JUNIT_ASSERT_EQUALS_STRING(TEST_STRING, result->string.get());
  JUNIT_ASSERT_EQUALS_INT(TEST_BYTE, result->b);
#if HAS_RAW_STRING_LITERALS
  JUNIT_ASSERT_EQUALS_INT(TEST_CHAR, result->c);
#endif
}

void ClassRegistryTest::nativeNewInstanceWithNull(JNIEnv *env, jobject javaThis, jobject javaTestObject) {
  LOG_INFO("Starting test: nativeNewInstanceWithNull");
  ClassRegistry registry;
  registry.add(env, new TestObject(env));
  TestObject *result = registry.newInstance<TestObject>(env, NULL);
  JUNIT_ASSERT_NOT_NULL(result);
  // Should have populated class info
  JUNIT_ASSERT_TRUE(result->isInitialized());
}
