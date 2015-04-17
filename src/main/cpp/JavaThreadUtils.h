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

#ifndef __JavaThreadUtils_h__
#define __JavaThreadUtils_h__

#include "JniHelpersCommon.h"
#include <string>

namespace spotify {
namespace jni {

class JavaThreadUtils {
private:
  // Direct instantiation not allowed
  JavaThreadUtils() {}
  JavaThreadUtils(const JavaThreadUtils&) {}
  virtual ~JavaThreadUtils() {}

public:
  static EXPORT JavaVM* getJavaVM();
  static EXPORT JNIEnv* initialize(JavaVM* jvm);
  static EXPORT JNIEnv* getEnvForCurrentThread();
  static EXPORT JNIEnv* getEnvForCurrentThread(JavaVM *jvm);
  static EXPORT JNIEnv* attachCurrentThreadToJVM(const char* thread_name);
  static EXPORT JNIEnv* attachCurrentThreadAsDaemonToJVM(const char* thread_name);
  static EXPORT void detatchCurrentThreadFromJVM();
};

} // namespace jni
} // namespace spotify

#endif // __JavaThreadUtils_h__
