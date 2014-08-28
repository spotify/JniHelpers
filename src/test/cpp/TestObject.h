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

#ifndef __TestObject_h__
#define __TestObject_h__

#include "JniHelpers.h"
#include "JniHelpersTest.h"

using namespace spotify::jni;

/**
 * @brief Basic model-type class
 *
 * This class is a very simple object which holds a few pieces of data. It
 * demonstrates conversion between Java <--> C++ objects.
 */
class EXPORT TestObject : public JavaClass {
public:
  /**
   * @brief Basic no-arg constructor
   *
   * If you want to use ClassRegistry::newInstance(), then you must provide
   * a no-arg constructor.
   */
  TestObject();
  /**
   * @brief Initialization constructor
   * @param env JNIEnv
   *
   * Normally you shouldn't call this constructor directly, instead it is invoked
   * in JNI_OnLoad and should call initialize() in order to cache any method/field
   * definitions for later.
   */
  TestObject(JNIEnv *env);
  ~TestObject() {}

  /**
   * @brief Return the class canonical name
   * @return String with JNI canonical name (ie, "com/example/Foo")
   *
   * Use of the MAKE_CANONICAL_NAME macro is recommended here
   */
  const char* getCanonicalName() const {
    return MAKE_CANONICAL_NAME(PACKAGE, TestObject);
  }

  /**
   * @brief Initialize class info, which is utilized by ClassRegistry::newInstance
   * @param env JNIEnv
   */
  virtual void initialize(JNIEnv *env);

  /**
   * @brief Configure field mappings
   *
   * Field mappings tell JniHelpers how to copy data between Java <--> C++ instances
   * when invoking toJavaObject() and setJavaObject(). If you do not need to use
   * these methods, then it is not necessary to do any work in this method.
   */
  virtual void mapFields();

public:
  JavaString string;
  int i;
  short s;
  float f;
  double d;
  bool z;
  signed char b;
  wchar_t c;
  // TODO: byte[] b;
};

#endif // __TestObject_h__
