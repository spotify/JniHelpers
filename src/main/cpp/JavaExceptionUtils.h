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

#ifndef __JavaExceptionUtils_h__
#define __JavaExceptionUtils_h__

#include "JniHelpersCommon.h"
#include "JniLocalRef.h"

namespace spotify {
namespace jni {

class JavaExceptionUtils {
private:
  // Direct instantiation not allowed
  JavaExceptionUtils() {}
  JavaExceptionUtils(const JavaExceptionUtils&) {}
  virtual ~JavaExceptionUtils() {}

public:
  static EXPORT JniLocalRef<jobject> newThrowable(JNIEnv *env, const char *message, ...);

  static EXPORT void checkException(JNIEnv *env);
  static EXPORT void checkExceptionAndClear(JNIEnv *env);
  static EXPORT void throwException(JNIEnv *env, const char *message, ...);
  static EXPORT void throwRuntimeException(JNIEnv *env, const char *message, ...);
  static EXPORT void throwExceptionOfType(JNIEnv *env, const char *exception_class_name, const char *message, ...);

protected:
  static EXPORT void throwExceptionOfType(JNIEnv *env, const char *exception_class_name, const char *message, va_list arguments);
};
}
}

#endif // __JavaExceptionUtils_h__
