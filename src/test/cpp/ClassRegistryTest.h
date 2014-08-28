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

#ifndef __ClassRegistryTest_h__
#define __ClassRegistryTest_h__

#include "JniHelpersTest.h"
#include "JniHelpers.h"

class ClassWithName : public JavaClass {
public:
  ClassWithName(const char *name) : JavaClass(), _name(name) {}
  ClassWithName(JNIEnv *env) : JavaClass(env) { initialize(env); }
  ~ClassWithName() {}

  void initialize(JNIEnv *env) {}
  void mapFields() {}
  const char* getCanonicalName() const { return _name; }
  void setJavaObject(JNIEnv *env, jobject javaObject) {}
  jobject toJavaObject(JavaClass *nativeObject) { return NULL; }

private:
  const char* _name;
};

class EXPORT ClassRegistryTest : public JavaClass {
public:
  ClassRegistryTest() : JavaClass() {}
  ClassRegistryTest(JNIEnv *env) : JavaClass(env) { initialize(env); }
  ~ClassRegistryTest() {}

  void initialize(JNIEnv *env);
  void mapFields() {}
  const char* getCanonicalName() const {
    return MAKE_CANONICAL_NAME(PACKAGE, ClassRegistryTest);
  }

private:
  static void createRegistry(JNIEnv *env, jobject javaThis);

  static void addClass(JNIEnv *env, jobject javaThis);
  static void addNullClass(JNIEnv *env, jobject javaThis);
  static void addClassWithEmptyName(JNIEnv *env, jobject javaThis);
  static void addClassWithNullName(JNIEnv *env, jobject javaThis);
  static void addClassWithoutInfo(JNIEnv *env, jobject javaThis);
  static void addClassMultipleTimes(JNIEnv *env, jobject javaThis);

  static void get(JNIEnv *env, jobject javaThis);
  static void getWithBracketOperator(JNIEnv *env, jobject javaThis);
  static void getNullClass(JNIEnv *env, jobject javaThis);
  static void getInvalidClass(JNIEnv *env, jobject javaThis);

  static void nativeNewInstance(JNIEnv *env, jobject javaThis, jobject javaTestObject);
  static void nativeNewInstanceWithNull(JNIEnv *env, jobject javaThis, jobject javaTestObject);
};

#endif // __ClassRegistryTest_h__
