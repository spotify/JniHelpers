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

#include "JniHelpersTest.h"
#include "ByteArrayTest.h"
#include "ClassRegistryTest.h"
#include "JavaClassTest.h"
#include "JavaClassUtilsTest.h"
#include "JavaExceptionUtilsTest.h"
#include "JavaStringTest.h"
#include "JavaThreadUtilsTest.h"
#include "NativeObjectTest.h"
#include "PersistedObject.h"
#include "JavaStringArrayTest.h"

ClassRegistry gClasses;

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *jvm, void*) {
  LOG_INFO("Initializing JNI");
  JNIEnv *env = jniHelpersInitialize(jvm);
  if (env == NULL) {
    return -1;
  }

  gClasses.add(env, new ByteArrayTest(env));
  gClasses.add(env, new ClassRegistryTest(env));
  gClasses.add(env, new JavaClassTest(env));
  gClasses.add(env, new JavaClassUtilsTest(env));
  gClasses.add(env, new JavaExceptionUtilsTest(env));
  gClasses.add(env, new JavaStringTest(env));
  gClasses.add(env, new NativeObjectTest(env));
  gClasses.add(env, new PersistedObject(env));
  gClasses.add(env, new JavaStringArrayTest(env));

  LOG_INFO("Initialization complete");
  return JAVA_VERSION;
}
