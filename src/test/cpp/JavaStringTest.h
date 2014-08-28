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

#ifndef __JavaStringTest_h__
#define __JavaStringTest_h__

#include "JniHelpers.h"
#include "JniHelpersTest.h"

using namespace spotify::jni;

class EXPORT JavaStringTest : public JavaClass {
public:
  JavaStringTest() : JavaClass() {}
  JavaStringTest(JNIEnv *env) : JavaClass(env) {
    initialize(env);
  }
  ~JavaStringTest() {}

  void initialize(JNIEnv *env);
  void mapFields() {}
  const char* getCanonicalName() const {
    return MAKE_CANONICAL_NAME(PACKAGE, JavaStringTest);
  }

private:
  static bool supportsRawStringLiterals(JNIEnv *env);

  static void createJavaString(JNIEnv *env, jobject javaThis);
  static void createJavaStringFromStdString(JNIEnv *env, jobject javaThis);
  static void nativeCreateJavaStringFromJavaString(JNIEnv *env, jobject javaThis, jobject javaString);

  static jstring nativeGetJavaString(JNIEnv *env, jobject javaThis);
  static jstring nativeGetJavaStringWithNullChar(JNIEnv *env, jobject javaThis);
  static jstring nativeGetJavaStringUtf16(JNIEnv *env, jobject javaThis);
  static jstring nativeGetJavaStringUtf8(JNIEnv *env, jobject javaThis);

  static void nativeSetValue(JNIEnv *env, jobject javaThis, jobject javaString);
  static void nativeSetValueWithOperator(JNIEnv *env, jobject javaThis);
  static jstring nativeSetAndReturnValue(JNIEnv *env, jobject javaThis, jobject javaString);
};

#endif // __JavaStringTest_h__
