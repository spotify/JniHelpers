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

#include "JavaArrayTest.h"
#include "JUnitUtils.h"
#include "JavaArray.h"

void JavaArrayTest::initialize(JNIEnv *env) {
  setClass(env);

  addNativeMethod("createNewByteArray", (void*)createNewByteArray, kTypeVoid, NULL);
  addNativeMethod("createNewByteArrayWithData", (void*)createNewByteArrayWithData, kTypeVoid, NULL);
  addNativeMethod("createNewByteArrayWithDataCopy", (void*)createNewByteArrayWithData, kTypeVoid, NULL);
  addNativeMethod("nativeCreateNewByteArrayWithJavaData", (void*)nativeCreateNewByteArrayWithJavaData, kTypeVoid, kTypeArray(kTypeByte), NULL);
  addNativeMethod("createNewByteArrayWithNull", (void*)createNewByteArrayWithNull, kTypeVoid, NULL);
  addNativeMethod("createNewByteArrayWithNullAndNonZeroLength", (void*)createNewByteArrayWithNullAndNonZeroLength, kTypeVoid, NULL);
  addNativeMethod("nativeGetTestJavaByteArray", (void*)nativeGetTestJavaByteArray, kTypeArray(kTypeByte), NULL);
  addNativeMethod("setData", (void*)setData, kTypeVoid, NULL);
  addNativeMethod("setDataWithCopy", (void*)setData, kTypeVoid, NULL);
  addNativeMethod("nativeSetJavaByteArray", (void*)nativeSetJavaByteArray, kTypeVoid, kTypeArray(kTypeByte), kTypeInt, NULL);

  registerNativeMethods(env);
}

void* JavaArrayTest::getTestData() {
  char *result = (char*)malloc(getTestDataSize());
  for (size_t i = 0; i < getTestDataSize(); i++) {
    result[i] = (char)i;
  }
  return result;
}

size_t JavaArrayTest::getTestDataSize() {
  return 10;
}

void JavaArrayTest::createNewByteArray(JNIEnv *env, jobject javaThis) {
  JavaArray byteArray;
  JUNIT_ASSERT_EQUALS_INT(0, byteArray.size());
  JUNIT_ASSERT_NULL(byteArray.get());
}

void JavaArrayTest::createNewByteArrayWithData(JNIEnv *env, jobject javaThis) {
  void *data = getTestData();
  JavaArray byteArray(data, getTestDataSize(), false);
  JUNIT_ASSERT_EQUALS_INT(getTestDataSize(), byteArray.size());
  JUNIT_ASSERT_EQUALS_ARRAY(data, byteArray.get(), byteArray.size());
}

void JavaArrayTest::createNewByteArrayWithDataCopy(JNIEnv *env, jobject javaThis) {
  void *data = getTestData();
  JavaArray byteArray(data, getTestDataSize(), true);
  free(data);
  void *expected = getTestData();
  JUNIT_ASSERT_EQUALS_INT(getTestDataSize(), byteArray.size());
  JUNIT_ASSERT_EQUALS_ARRAY(expected, byteArray.get(), byteArray.size());
}

void JavaArrayTest::nativeCreateNewByteArrayWithJavaData(JNIEnv *env, jobject javaThis, jbyteArray javaData) {
  void *data = getTestData();
  JavaArray byteArray(env, javaData);
  JUNIT_ASSERT_EQUALS_INT(getTestDataSize(), byteArray.size());
  JUNIT_ASSERT_EQUALS_ARRAY(data, byteArray.get(), byteArray.size());
}

void JavaArrayTest::createNewByteArrayWithNull(JNIEnv *env, jobject javaThis) {
  JavaArray byteArray(env, (jbyteArray)NULL);
  JUNIT_ASSERT_EQUALS_INT(0, byteArray.size());
  JUNIT_ASSERT_NULL(byteArray.get());
}

void JavaArrayTest::createNewByteArrayWithNullAndNonZeroLength(JNIEnv *env, jobject javaThis) {
  JavaArray byteArray(NULL, 1, false);
  JUNIT_ASSERT_EQUALS_INT(0, byteArray.size());
  JUNIT_ASSERT_NULL(byteArray.get());
}

jbyteArray JavaArrayTest::nativeGetTestJavaByteArray(JNIEnv *env, jobject javaThis) {
  void *data = getTestData();
  JavaArray byteArray(data, getTestDataSize(), true);
  JUNIT_ASSERT_EQUALS_INT(getTestDataSize(), byteArray.size());
  JUNIT_ASSERT_EQUALS_ARRAY(data, byteArray.get(), byteArray.size());
  JniLocalRef<jbyteArray> result = byteArray.toJavaByteArray(env);
  return result.leak();
}

void JavaArrayTest::setData(JNIEnv *env, jobject javaThis) {
  void *data = getTestData();
  JavaArray byteArray;
  byteArray.set(data, getTestDataSize(), false);
  JUNIT_ASSERT_EQUALS_INT(getTestDataSize(), byteArray.size());
  JUNIT_ASSERT_EQUALS_ARRAY(data, byteArray.get(), byteArray.size());
}

void JavaArrayTest::setDataWithCopy(JNIEnv *env, jobject javaThis) {
  void *data = getTestData();
  JavaArray byteArray;
  byteArray.set(data, getTestDataSize(), true);
  // Write 0's over the original data to make sure that a false positive
  // doesn't cause the test to pass.
  memset(data, 0, getTestDataSize());
  void *expectedData = getTestData();
  JUNIT_ASSERT_EQUALS_INT(getTestDataSize(), byteArray.size());
  JUNIT_ASSERT_EQUALS_ARRAY(expectedData, byteArray.get(), byteArray.size());
  free(expectedData);
}

void JavaArrayTest::nativeSetJavaByteArray(JNIEnv *env, jobject javaThis, jbyteArray javaData, jint expectedSize) {
  void *data = getTestData();
  JavaArray byteArray;
  byteArray.set(env, javaData);
  JUNIT_ASSERT_EQUALS_INT(expectedSize, byteArray.size());
  JUNIT_ASSERT_EQUALS_ARRAY(data, byteArray.get(), byteArray.size());
}
