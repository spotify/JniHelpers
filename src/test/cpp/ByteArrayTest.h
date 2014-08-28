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

#ifndef __ByteArrayTest_h__
#define __ByteArrayTest_h__

#include "JniHelpers.h"
#include "JniHelpersTest.h"

using namespace spotify::jni;

class ByteArrayTest : public JavaClass {
public:
  ByteArrayTest() : JavaClass() {}
  ByteArrayTest(JNIEnv *env) : JavaClass(env) { initialize(env); }
  ~ByteArrayTest() {}

  const char* getCanonicalName() const {
    return MAKE_CANONICAL_NAME(PACKAGE, ByteArrayTest);
  }
  void initialize(JNIEnv *env);
  void mapFields() {}

private:
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
};

#endif // __ByteArrayTest_h__
