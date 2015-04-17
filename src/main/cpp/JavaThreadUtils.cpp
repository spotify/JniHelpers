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

#include "JavaThreadUtils.h"
#include "JavaExceptionUtils.h"

namespace spotify {
namespace jni {

static JavaVM* sJavaVm = NULL;

JNIEnv* JavaThreadUtils::initialize(JavaVM *jvm) {
  sJavaVm = jvm;
  return getEnvForCurrentThread(jvm);
}

JavaVM* JavaThreadUtils::getJavaVM() {
  return sJavaVm;
}

JNIEnv* JavaThreadUtils::getEnvForCurrentThread() {
  return getEnvForCurrentThread(sJavaVm);
}

JNIEnv* JavaThreadUtils::getEnvForCurrentThread(JavaVM *jvm) {
  JNIEnv *env;

  if (jvm == NULL) {
    return NULL;
  } else if (jvm->GetEnv(reinterpret_cast<void**>(&env), JAVA_VERSION) != JNI_OK) {
    // The current thread isn't attached to a Java thread, this could happen when
    // mistakenly calling JniHelpers functions directly from other C++ code.
    return NULL;
  }

  return env;
}

JNIEnv* JavaThreadUtils::attachCurrentThreadToJVM(const char* thread_name) {
  JNIEnv *env;
  JavaVMAttachArgs args;
  int result = -1;

  args.version = JAVA_VERSION;
  args.name = const_cast<char*>(thread_name);
  args.group = NULL;

#ifdef ANDROID
  result = sJavaVm->AttachCurrentThread(&env, &args);
#else
  result = sJavaVm->AttachCurrentThread((void**)(&env), &args);
#endif
  if (result != JNI_OK) {
    JavaExceptionUtils::throwRuntimeException(env, "Could not attach thread %s to JVM", thread_name);
    return NULL;
  }

  return env;
}

JNIEnv* JavaThreadUtils::attachCurrentThreadAsDaemonToJVM(const char* thread_name) {
  JNIEnv *env;
  JavaVMAttachArgs args;
  int result = -1;

  args.version = JAVA_VERSION;
  args.name = const_cast<char*>(thread_name);
  args.group = NULL;

#ifdef ANDROID
  result = sJavaVm->AttachCurrentThreadAsDaemon(&env, &args);
#else
  result = sJavaVm->AttachCurrentThreadAsDaemon((void**)(&env), &args);
#endif
  if (result != JNI_OK) {
    JavaExceptionUtils::throwRuntimeException(env, "Could not attach daemon thread %s to JVM", thread_name);
    return NULL;
  }

  return env;
}

void JavaThreadUtils::detatchCurrentThreadFromJVM() {
  sJavaVm->DetachCurrentThread();
}

} // namespace jni
} // namespace spotify
