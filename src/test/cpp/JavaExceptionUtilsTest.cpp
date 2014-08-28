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

#include "JavaExceptionUtilsTest.h"
#include "TestConstants.h"

void JavaExceptionUtilsTest::initialize(JNIEnv *env) {
  setClass(env);
  addNativeMethod("nativeThrowException", (void*)nativeThrowException, kTypeVoid, NULL);
  addNativeMethod("nativeThrowExceptionWithFormatString", (void*)nativeThrowExceptionWithFormatString, kTypeVoid, NULL);
  addNativeMethod("nativeThrowRuntimeException", (void*)nativeThrowRuntimeException, kTypeVoid, NULL);
  addNativeMethod("nativeThrowExceptionOfType", (void*)nativeThrowExceptionOfType, kTypeVoid, NULL);
  registerNativeMethods(env);
}

void JavaExceptionUtilsTest::nativeThrowException(JNIEnv *env, jobject javaThis) {
  JavaExceptionUtils::throwException(env, TEST_STRING);
}

void JavaExceptionUtilsTest::nativeThrowExceptionWithFormatString(JNIEnv *env, jobject javaThis) {
  JavaExceptionUtils::throwException(env, "%s, %d", TEST_STRING, TEST_INTEGER);
}

void JavaExceptionUtilsTest::nativeThrowRuntimeException(JNIEnv *env, jobject javaThis) {
  JavaExceptionUtils::throwRuntimeException(env, TEST_STRING);
}

void JavaExceptionUtilsTest::nativeThrowExceptionOfType(JNIEnv *env, jobject javaThis) {
  JavaExceptionUtils::throwExceptionOfType(env, kTypeUnsupportedOperationException, TEST_STRING);
}
