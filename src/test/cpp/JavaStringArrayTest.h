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

#ifndef __JavaStringArrayTest_h__
#define __JavaStringArrayTest_h__

#include "JniHelpers.h"
#include "JniHelpersTest.h"

using namespace spotify::jni;

class JavaStringArrayTest : public JavaClass {
public:
  JavaStringArrayTest() : JavaClass() {}
  JavaStringArrayTest(JNIEnv *env) : JavaClass(env) { initialize(env); }
  ~JavaStringArrayTest() {}

  const char* getCanonicalName() const {
    return MAKE_CANONICAL_NAME(PACKAGE, JavaStringArrayTest);
  }
  void initialize(JNIEnv *env);
  void mapFields() {}

private:
  static JavaString **getTestData();
  static size_t getTestDataSize();

  static void createNewJavaStringArray(JNIEnv *env, jobject javaThis);
  static void createNewJavaStringArrayWithData(JNIEnv *env, jobject javaThis);
  static void createNewJavaStringArrayWithDataCopy(JNIEnv *env, jobject javaThis);
  static void nativeCreateNewJavaStringArrayWithJavaData(JNIEnv *env, jobject javaThis, jobjectArray javaData);
  static void createNewJavaStringArrayWithNull(JNIEnv *env, jobject javaThis);
  static void createNewJavaStringArrayWithNullAndNonZeroLength(JNIEnv *env, jobject javaThis);
  static jobjectArray nativeGetTestJavaStringArray(JNIEnv *env, jobject javaThis);
  static void setData(JNIEnv *env, jobject javaThis);
  static void setDataWithCopy(JNIEnv *env, jobject javaThis);
  static void nativeSetJavaStringArray(JNIEnv *env, jobject javaThis, jobjectArray javaData, jint expectedSize);
};

#endif // __JavaStringArrayTest_h__
