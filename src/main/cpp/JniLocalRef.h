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

#ifndef __JniLocalRef_h__
#define __JniLocalRef_h__

#include "JniHelpersCommon.h"
#include "JavaThreadUtils.h"

namespace spotify {
namespace jni {

// RAII helper to maintain local references automatically
template<typename JniType>
class JniLocalRef {
public:
  JniLocalRef() : _obj(NULL) {}
  JniLocalRef(JniType obj) : _obj(NULL) { set(obj); }
  JniLocalRef(const JniLocalRef<JniType> &ref) : _obj(NULL) {
    if (ref.get()) {
      JNIEnv *env = JavaThreadUtils::getEnvForCurrentThread();
      set(env ? (JniType)env->NewLocalRef(ref.get()): NULL);
    }
  }

  ~JniLocalRef() { set(NULL); }

  JniType get() const { return _obj; }
  void set(JniType obj) {
    if (_obj) {
      JNIEnv *env = JavaThreadUtils::getEnvForCurrentThread();
      if (env) env->DeleteLocalRef(_obj);
    }
    _obj = obj;
  }

  JniType leak() {
    JniType obj = _obj;
    _obj = NULL;
    return obj;
  }

  operator JniType() const { return _obj; }

  void operator=(JniType obj) { set(obj); }
  void operator=(const JniLocalRef<JniType> &ref) {
    if (ref.get()) {
      JNIEnv *env = JavaThreadUtils::getEnvForCurrentThread();
      set(env ? (JniType)env->NewLocalRef(ref.get()) : NULL);
    }
    else {
      set(NULL);
    }
  }

private:
  JniType _obj;
};

} // namespace jni
} // namespace spotify

#endif // __JniLocalRef_h__
