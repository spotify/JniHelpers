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

#ifndef __JavaClassUtilsTest_h__
#define __JavaClassUtilsTest_h__

#include "JniHelpersTest.h"

class JavaClassUtilsTest : public JavaClass {
public:
  JavaClassUtilsTest() : JavaClass() {}
  JavaClassUtilsTest(JNIEnv *env) : JavaClass(env) { initialize(env); }
  ~JavaClassUtilsTest() {}

  const char* getCanonicalName() const {
    return MAKE_CANONICAL_NAME(PACKAGE, JavaClassUtilsTest);
  }

  void initialize(JNIEnv *env);
  void mapFields() {}

public:
  static jclass findObjectClass(JNIEnv *env, jobject javaThis);
  static void findInvalidClass(JNIEnv *env, jobject javaThis);
  static jclass nativeFindClassWithLoader(JNIEnv *env, jobject javaThis);
  static void findInvalidClassWithLoader(JNIEnv *env, jobject javaThis);
  static void makeNameForSignatureWithNull(JNIEnv *env, jobject javaThis);
  static void makeNameForSignatureWithPrimitive(JNIEnv *env, jobject javaThis);
  static void makeNameForSignatureWithArray(JNIEnv *env, jobject javaThis);
  static void makeNameForSignatureWithObject(JNIEnv *env, jobject javaThis);
  static void makeNameForSignatureWithJniSignature(JNIEnv *env, jobject javaThis);
  static void makeNameForSignatureWithArrayOfObjects(JNIEnv *env, jobject javaThis);
};

#endif // __JavaClassUtilsTest_h__
