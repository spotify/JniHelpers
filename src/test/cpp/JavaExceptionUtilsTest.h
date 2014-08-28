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

#ifndef __JavaExceptionUtilsTest_h__
#define __JavaExceptionUtilsTest_h__

#include "JniHelpersTest.h"

class JavaExceptionUtilsTest : public JavaClass {
public:
  JavaExceptionUtilsTest() : JavaClass() {}
  JavaExceptionUtilsTest(JNIEnv *env) : JavaClass(env) { initialize(env); }
  ~JavaExceptionUtilsTest() {}

  const char* getCanonicalName() const {
    return MAKE_CANONICAL_NAME(PACKAGE, JavaExceptionUtilsTest);
  }

  void initialize(JNIEnv *env);
  void mapFields() {}

private:
  static void nativeThrowException(JNIEnv *env, jobject javaThis);
  static void nativeThrowExceptionWithFormatString(JNIEnv *env, jobject javaThis);
  static void nativeThrowRuntimeException(JNIEnv *env, jobject javaThis);
  static void nativeThrowExceptionOfType(JNIEnv *env, jobject javaThis);
};

#endif // __JavaExceptionUtilsTest_h__
