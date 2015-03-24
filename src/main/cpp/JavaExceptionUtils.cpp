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

#include "JavaExceptionUtils.h"
#include "JavaClassUtils.h"
#include "JavaString.h"
#include <sstream>
#include <stdarg.h>

namespace spotify {
namespace jni {

static const size_t kExceptionMaxLength = 512;

void JavaExceptionUtils::checkException(JNIEnv *env) {
  if (env->ExceptionCheck()) {
    env->ExceptionDescribe();
#if TERMINATE_ON_EXCEPTION
    std::terminate();
#endif
  }
}

void JavaExceptionUtils::checkExceptionAndClear(JNIEnv *env) {
  checkException(env);
  env->ExceptionClear();
}

JniLocalRef<jobject> JavaExceptionUtils::newThrowable(JNIEnv *env, const char *message, ...) {
  // Find the Throwable class and its associated String constructor
  jclass throwableClazz = JavaClassUtils::findClass(env, kTypeJavaClass(Throwable), false);
  if (throwableClazz == NULL) {
    JavaExceptionUtils::throwRuntimeException(env, "Could not find class Throwable");
    return NULL;
  }

  std::string signature;
  JavaClassUtils::makeSignature(signature, kTypeVoid, kTypeString, NULL);
  jmethodID throwableCtor = env->GetMethodID(throwableClazz, "<init>", signature.c_str());
  if (throwableCtor == NULL) {
    JavaExceptionUtils::throwRuntimeException(env, "Could not find Throwable constructor");
    return NULL;
  }

  // Construct error message
  va_list arguments;
  va_start(arguments, message);
  char formattedMessage[kExceptionMaxLength];
  vsnprintf(formattedMessage, kExceptionMaxLength, message, arguments);
  va_end(arguments);
  JavaString javaMessage(formattedMessage);
  JniLocalRef<jobject> result = env->NewObject(throwableClazz, throwableCtor, javaMessage.toJavaString(env).get());
  JavaExceptionUtils::checkException(env);
  if (result == NULL) {
    JavaExceptionUtils::throwRuntimeException(env, "Could not create new Throwable instance");
    return NULL;
  }

  return result;
}

void JavaExceptionUtils::throwExceptionOfType(JNIEnv *env, const char *exception_class_name, const char *message, va_list arguments) {
  jclass clazz = JavaClassUtils::findClass(env, exception_class_name, false);
  checkException(env);
  if (clazz == NULL) {
#if ENABLE_EXCEPTIONS
    std::stringstream fatalErrorMessage;
    fatalErrorMessage << "Could not throw exception of type '" << exception_class_name << "'";
    env->FatalError(fatalErrorMessage.str().c_str());
#endif
    return;
  }
  char exceptionMessage[kExceptionMaxLength];
  vsnprintf(exceptionMessage, kExceptionMaxLength, message, arguments);
  LOG_ERROR("Throwing exception %s: %s", exception_class_name, exceptionMessage);
#if ENABLE_EXCEPTIONS
  env->ThrowNew(clazz, exceptionMessage);
#endif
}

void JavaExceptionUtils::throwExceptionOfType(JNIEnv *env, const char *exception_class_name, const char *message, ...) {
  va_list arguments;
  va_start(arguments, message);
  throwExceptionOfType(env, exception_class_name, message, arguments);
  va_end(arguments);
}

void JavaExceptionUtils::throwException(JNIEnv *env, const char *message, ...) {
  va_list arguments;
  va_start(arguments, message);
  throwExceptionOfType(env, kTypeException, message, arguments);
  va_end(arguments);
}

void JavaExceptionUtils::throwRuntimeException(JNIEnv *env, const char *message, ...) {
  va_list arguments;
  va_start(arguments, message);
  throwExceptionOfType(env, kTypeJavaClass(RuntimeException), message, arguments);
  va_end(arguments);
}

} // namespace jni
} // namespace spotify
