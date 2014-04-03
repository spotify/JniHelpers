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

#ifndef __JavaArrayTest_h__
#define __JavaArrayTest_h__

#include "JniHelpers.h"
#include "JniHelpersTest.h"

using namespace spotify::jni;

class JavaArrayTest : public JavaClass {
public:
  JavaArrayTest() : JavaClass() {}
  JavaArrayTest(JNIEnv *env) : JavaClass(env) { initialize(env); }
  ~JavaArrayTest() {}

  const char* getCanonicalName() const {
    return MAKE_CANONICAL_NAME(PACKAGE, JavaArrayTest);
  }
  void initialize(JNIEnv *env);
  void mapFields() {}

private:
#if 0
  static void* getTestData();
  static size_t getTestDataSize();

  static void createNewByteArray(JNIEnv *env, jobject javaThis);
  static void createNewByteArrayWithData(JNIEnv *env, jobject javaThis);
  static void createNewByteArrayWithDataCopy(JNIEnv *env, jobject javaThis);
  static void nativeCreateNewByteArrayWithJavaData(JNIEnv *env, jobject javaThis, jbyteArray javaData);
  static void createNewByteArrayWithNull(JNIEnv *env, jobject javaThis);
  static void createNewByteArrayWithNullAndNonZeroLength(JNIEnv *env, jobject javaThis);
  static jbyteArray nativeGetTestJavaByteArray(JNIEnv *env, jobject javaThis);
  static void setData(JNIEnv *env, jobject javaThis);
  static void setDataWithCopy(JNIEnv *env, jobject javaThis);
  static void nativeSetJavaByteArray(JNIEnv *env, jobject javaThis, jbyteArray javaData, jint expectedSize);
#endif
};

#endif // __JavaArrayTest_h__
