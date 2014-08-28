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

#include "TestObject.h"

TestObject::TestObject() : JavaClass(),
string(), i(0), s(0), f(0.0f), d(0.0), z(false), b(0), c(0) {
}

TestObject::TestObject(JNIEnv *env) : JavaClass(env),
string(), i(0), s(0), f(0.0f), d(0.0), z(false), b(0), c(0) {
  initialize(env);
  // Set up field mappings for the instance. Normally this isn't necessary,
  // since the global instance is typically used for merging into other objects.
  // If you want to actually use the initialized instance in combination with
  // setJavaObject(), you should call this though.
  merge(this);
}

void TestObject::initialize(JNIEnv *env) {
  setClass(env);

  // Cache some jfieldIDs for quick lookup later. Note that this is necessary
  // for the corresponding fields to be mapped in mapFields().
  cacheField(env, "string", kTypeString);
  cacheField(env, "i", kTypeInt);
  cacheField(env, "s", kTypeShort);
  cacheField(env, "f", kTypeFloat);
  cacheField(env, "d", kTypeDouble);
  cacheField(env, "z", kTypeBool);
  cacheField(env, "b", kTypeByte);
  cacheField(env, "c", kTypeChar);
  // cacheField(env, "b", kTypeArray(kTypeByte));

  // Cache default constructor
  cacheConstructor(env);

  // Cache methods for later. Currently these are unused, but would provide an
  // easy mechanism for calling Java methods. Note that when calling cacheMethod,
  // the last argument must always be NULL. See the documentation for
  // JavaClassUtils::makeSignature for more details.
  cacheMethod(env, "getString", kTypeString, NULL);
  cacheMethod(env, "setString", kTypeVoid, kTypeString, NULL);
  cacheMethod(env, "getI", kTypeInt, NULL);
  cacheMethod(env, "setI", kTypeVoid, kTypeInt, NULL);
  cacheMethod(env, "getS", kTypeShort, NULL);
  cacheMethod(env, "setS", kTypeVoid, kTypeShort, NULL);
  cacheMethod(env, "getF", kTypeFloat, NULL);
  cacheMethod(env, "setF", kTypeVoid, kTypeFloat, NULL);
  cacheMethod(env, "getD", kTypeDouble, NULL);
  cacheMethod(env, "setD", kTypeVoid, kTypeDouble, NULL);
  cacheMethod(env, "getZ", kTypeBool, NULL);
  cacheMethod(env, "setZ", kTypeVoid, kTypeBool, NULL);
  cacheMethod(env, "getB", kTypeByte, NULL);
  cacheMethod(env, "setB", kTypeVoid, kTypeByte, NULL);
  cacheMethod(env, "getC", kTypeChar, NULL);
  cacheMethod(env, "setC", kTypeVoid, kTypeChar, NULL);
  // TODO: Getters/setters for byte array
}

void TestObject::mapFields() {
  mapField("i", kTypeInt, &i);
  mapField("s", kTypeShort, &s);
  mapField("f", kTypeFloat, &f);
  mapField("d", kTypeDouble, &d);
  mapField("string", kTypeString, &string);
  mapField("z", kTypeBool, &z);
  mapField("b", kTypeByte, &b);
  mapField("c", kTypeChar, &c);
}
