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

#include "JavaStringArrayTest.h"
#include "JUnitUtils.h"
#include "JavaStringArray.h"

void JavaStringArrayTest::initialize(JNIEnv *env) {
  setClass(env);

  addNativeMethod("createNewJavaStringArray", (void*)createNewJavaStringArray, kTypeVoid, NULL);
  addNativeMethod("createNewJavaStringArrayWithData", (void*)createNewJavaStringArrayWithData, kTypeVoid, NULL);
  addNativeMethod("createNewJavaStringArrayWithDataCopy", (void*)createNewJavaStringArrayWithData, kTypeVoid, NULL);
  addNativeMethod("nativeCreateNewJavaStringArrayWithJavaData", (void*)nativeCreateNewJavaStringArrayWithJavaData, kTypeVoid, kTypeObjectArray(kTypeString), NULL);
  addNativeMethod("createNewJavaStringArrayWithNull", (void*)createNewJavaStringArrayWithNull, kTypeVoid, NULL);
  addNativeMethod("createNewJavaStringArrayWithNullAndNonZeroLength", (void*)createNewJavaStringArrayWithNullAndNonZeroLength, kTypeVoid, NULL);
  addNativeMethod("nativeGetTestJavaStringArray", (void*)nativeGetTestJavaStringArray, kTypeObjectArray(kTypeString), NULL);
  addNativeMethod("setData", (void*)setData, kTypeVoid, NULL);
  addNativeMethod("setDataWithCopy", (void*)setData, kTypeVoid, NULL);
  addNativeMethod("nativeSetJavaStringArray", (void*)nativeSetJavaStringArray, kTypeVoid, kTypeObjectArray(kTypeString), kTypeInt, NULL);

  registerNativeMethods(env);
}

JavaString **JavaStringArrayTest::getTestData() {
  JavaString **result = (JavaString **)malloc(getTestDataSize() * sizeof(JavaString*));
  result[0] = new JavaString("abc");
  result[1] = new JavaString("def");
  result[2] = new JavaString("ghi");
  result[3] = new JavaString("ążń");
  return result;
}

size_t JavaStringArrayTest::getTestDataSize() {
  return 4;
}

void JavaStringArrayTest::createNewJavaStringArray(JNIEnv *env, jobject javaThis) {
  JavaStringArray JavaStringArray;
  JUNIT_ASSERT_EQUALS_INT(0, JavaStringArray.size());
  JUNIT_ASSERT_NULL(JavaStringArray.get());
}

void JavaStringArrayTest::createNewJavaStringArrayWithData(JNIEnv *env, jobject javaThis) {
  JavaString **data = getTestData();
  JavaStringArray JavaStringArray(data, getTestDataSize(), false);
  JUNIT_ASSERT_EQUALS_INT(getTestDataSize(), JavaStringArray.size());
  JUNIT_ASSERT_EQUALS_JAVA_STRING_ARRAY(data, JavaStringArray.get(), JavaStringArray.size());
}

void JavaStringArrayTest::createNewJavaStringArrayWithDataCopy(JNIEnv *env, jobject javaThis) {
  JavaString **data = getTestData();
  JavaStringArray JavaStringArray(data, getTestDataSize(), true);
  free(data);
  JavaString **expected = getTestData();
  JUNIT_ASSERT_EQUALS_INT(getTestDataSize(), JavaStringArray.size());
  JUNIT_ASSERT_EQUALS_JAVA_STRING_ARRAY(expected, JavaStringArray.get(), JavaStringArray.size());
}

void JavaStringArrayTest::nativeCreateNewJavaStringArrayWithJavaData(JNIEnv *env, jobject javaThis, jobjectArray javaData) {
  JavaString **data = getTestData();
  JavaStringArray JavaStringArray(env, javaData);
  JUNIT_ASSERT_EQUALS_INT(getTestDataSize(), JavaStringArray.size());
  JUNIT_ASSERT_NOT_NULL(JavaStringArray.get());
  JUNIT_ASSERT_EQUALS_JAVA_STRING_ARRAY(data, JavaStringArray.get(), JavaStringArray.size());
}

void JavaStringArrayTest::createNewJavaStringArrayWithNull(JNIEnv *env, jobject javaThis) {
  JavaStringArray JavaStringArray(env, (jobjectArray)NULL);
  JUNIT_ASSERT_EQUALS_INT(0, JavaStringArray.size());
  JUNIT_ASSERT_NULL(JavaStringArray.get());
}

void JavaStringArrayTest::createNewJavaStringArrayWithNullAndNonZeroLength(JNIEnv *env, jobject javaThis) {
  JavaStringArray JavaStringArray(NULL, 1, false);
  JUNIT_ASSERT_EQUALS_INT(0, JavaStringArray.size());
  JUNIT_ASSERT_NULL(JavaStringArray.get());
}

jobjectArray JavaStringArrayTest::nativeGetTestJavaStringArray(JNIEnv *env, jobject javaThis) {
  JavaString **data = getTestData();
  JavaStringArray JavaStringArray(data, getTestDataSize(), true);
  JUNIT_ASSERT_EQUALS_INT(getTestDataSize(), JavaStringArray.size());
  JUNIT_ASSERT_EQUALS_JAVA_STRING_ARRAY(data, JavaStringArray.get(), JavaStringArray.size());
  JniLocalRef<jobjectArray> result = JavaStringArray.toJavaStringArray(env);
  return result.leak();
}

void JavaStringArrayTest::setData(JNIEnv *env, jobject javaThis) {
  JavaString **data = getTestData();
  JavaStringArray JavaStringArray;
  JavaStringArray.set(data, getTestDataSize(), false);
  JUNIT_ASSERT_EQUALS_INT(getTestDataSize(), JavaStringArray.size());
  JUNIT_ASSERT_EQUALS_JAVA_STRING_ARRAY(data, JavaStringArray.get(), JavaStringArray.size());
}

void JavaStringArrayTest::setDataWithCopy(JNIEnv *env, jobject javaThis) {
  JavaString **data = getTestData();
  JavaStringArray JavaStringArray;
  JavaStringArray.set(data, getTestDataSize(), true);
  // Write 0's over the original data to make sure that a false positive
  // doesn't cause the test to pass.
  memset(data, 0, getTestDataSize());
  JavaString **expected = getTestData();
  JUNIT_ASSERT_EQUALS_INT(getTestDataSize(), JavaStringArray.size());
  JUNIT_ASSERT_EQUALS_JAVA_STRING_ARRAY(expected, JavaStringArray.get(), JavaStringArray.size());
  free(expected);
}

void JavaStringArrayTest::nativeSetJavaStringArray(JNIEnv *env, jobject javaThis, jobjectArray javaData, jint expectedSize) {
  JavaString **data = getTestData();
  JavaStringArray JavaStringArray;
  JavaStringArray.set(env, javaData);
  JUNIT_ASSERT_EQUALS_INT(expectedSize, JavaStringArray.size());
  JUNIT_ASSERT_EQUALS_JAVA_STRING_ARRAY(data, JavaStringArray.get(), JavaStringArray.size());
}
