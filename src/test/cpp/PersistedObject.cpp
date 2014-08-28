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

#include "PersistedObject.h"

PersistedObject::PersistedObject() : NativeObject(), i(0) {}

PersistedObject::PersistedObject(JNIEnv *env) : NativeObject(env), i(0) {
  initialize(env);
}

void PersistedObject::initialize(JNIEnv *env) {
  setClass(env);
  cacheConstructor(env);
  cacheField(env, "i", kTypeInt);
  addNativeMethod("destroy", &PersistedObject::nativeDestroy, kTypeVoid, NULL);
  registerNativeMethods(env);
}

void PersistedObject::mapFields() {
  mapField("i", kTypeInt, &i);
}

void PersistedObject::nativeDestroy(JNIEnv *env, jobject java_this) {
  PersistedObject *object = gClasses.getNativeInstance<PersistedObject>(env, java_this);
  if (object != NULL) {
    // NativeObject *native_object = dynamic_cast<NativeObject*>(object);
    object->destroy(env, java_this);
  }
}
