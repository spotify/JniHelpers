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

#include "JavaClassUtilsTest.h"
#include "JUnitUtils.h"
#include "JavaClassUtils.h"
#include "TestObject.h"

void JavaClassUtilsTest::initialize(JNIEnv *env) {
  setClass(env);
  addNativeMethod("findObjectClass", (void*)&findObjectClass, kTypeJavaClass(Class), NULL);
  addNativeMethod("findInvalidClass", (void*)&findInvalidClass, kTypeVoid, NULL);
  addNativeMethod("nativeFindClassWithLoader", (void*)&nativeFindClassWithLoader, kTypeJavaClass(Class), NULL);
  addNativeMethod("findInvalidClassWithLoader", (void*)&findInvalidClass, kTypeVoid, NULL);
  addNativeMethod("makeNameForSignatureWithNull", (void*)&makeNameForSignatureWithNull, kTypeVoid, NULL);
  addNativeMethod("makeNameForSignatureWithPrimitive", (void*)&makeNameForSignatureWithPrimitive, kTypeVoid, NULL);
  addNativeMethod("makeNameForSignatureWithArray", (void*)&makeNameForSignatureWithArray, kTypeVoid, NULL);
  addNativeMethod("makeNameForSignatureWithObject", (void*)&makeNameForSignatureWithObject, kTypeVoid, NULL);
  addNativeMethod("makeNameForSignatureWithJniSignature", (void*)&makeNameForSignatureWithJniSignature, kTypeVoid, NULL);
  addNativeMethod("makeNameForSignatureWithArrayOfObjects", (void*)&makeNameForSignatureWithArrayOfObjects, kTypeVoid, NULL);
  registerNativeMethods(env);
}

jclass JavaClassUtilsTest::findObjectClass(JNIEnv *env, jobject javaThis) {
  LOG_INFO("Starting test findObjectClass");
  return JavaClassUtils::findClass(env, "java/lang/Object", false);
}

void JavaClassUtilsTest::findInvalidClass(JNIEnv *env, jobject javaThis) {
  JavaClassUtils::findClass(env, "invalid", false);
}

jclass JavaClassUtilsTest::nativeFindClassWithLoader(JNIEnv *env, jobject javaThis) {
  LOG_INFO("Starting test nativeFindClassWithLoader");
  JavaClassUtilsTest dummy;
  return JavaClassUtils::findClass(env, dummy.getCanonicalName(), true);
}

void JavaClassUtilsTest::findInvalidClassWithLoader(JNIEnv *env, jobject javaThis) {
  JavaClassUtils::findClass(env, "invalid", true);
}

void JavaClassUtilsTest::makeNameForSignatureWithNull(JNIEnv *env, jobject javaThis) {
  std::string result;
  JavaClassUtils::makeNameForSignature(result, NULL);
  JUNIT_ASSERT_EQUALS_STRING("", result);
}

void JavaClassUtilsTest::makeNameForSignatureWithPrimitive(JNIEnv *env, jobject javaThis) {
  std::string result;
  JavaClassUtils::makeNameForSignature(result, kTypeInt);
  JUNIT_ASSERT_EQUALS_STRING(kTypeInt, result);
}

void JavaClassUtilsTest::makeNameForSignatureWithArray(JNIEnv *env, jobject javaThis) {
  std::string result;
  JavaClassUtils::makeNameForSignature(result, kTypeArray(kTypeInt));
  JUNIT_ASSERT_EQUALS_STRING(kTypeArray(kTypeInt), result);
}

void JavaClassUtilsTest::makeNameForSignatureWithObject(JNIEnv *env, jobject javaThis) {
  std::string result;
  TestObject testObject;
  JavaClassUtils::makeNameForSignature(result, testObject.getCanonicalName());
  std::stringstream expected;
  expected << "L" << testObject.getCanonicalName() << ";";
  JUNIT_ASSERT_EQUALS_STRING(expected.str().c_str(), result);
}

void JavaClassUtilsTest::makeNameForSignatureWithJniSignature(JNIEnv *env, jobject javaThis) {
  std::string result;
  TestObject testObject;
  std::stringstream signature;
  signature << "L" << testObject.getCanonicalName() << ";";
  JavaClassUtils::makeNameForSignature(result, signature.str().c_str());
  JUNIT_ASSERT_EQUALS_STRING(signature.str().c_str(), result);
}

void JavaClassUtilsTest::makeNameForSignatureWithArrayOfObjects(JNIEnv *env, jobject javaThis) {
  std::string result;
  TestObject testObject;
  JavaClassUtils::makeNameForSignature(result, kTypeObjectArray(MAKE_CANONICAL_NAME(PACKAGE, TestObject)));
  std::stringstream expected;
  expected << "[L" << testObject.getCanonicalName() << ";";
  JUNIT_ASSERT_EQUALS_STRING(expected.str().c_str(), result);
}
