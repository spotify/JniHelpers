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

#ifndef __JniGlobalRef_h__
#define __JniGlobalRef_h__

#include "JniHelpersCommon.h"
#include "JavaThreadUtils.h"
#include "JniLocalRef.h"

namespace spotify {
namespace jni {

// RAII helper to maintain global references automatically.
template<typename JniType>
class EXPORT JniGlobalRef {
 public:
  JniGlobalRef() : _obj(NULL) {}
  JniGlobalRef(const JniGlobalRef<JniType> &ref) : _obj(NULL) { set(ref.get()); }
  JniGlobalRef(const JniLocalRef<JniType> &ref) : _obj(NULL) { set(ref.get()); }

  ~JniGlobalRef() { set(NULL); }

  JniType get() const { return _obj; }

  void set(JniType obj) {
    JNIEnv *env = NULL;
    if (_obj || obj) {
      env = JavaThreadUtils::getEnvForCurrentThread();
    }
    if (_obj) {
      if (env) {
        env->DeleteGlobalRef(_obj);
      }
      _obj = NULL;
    }
    if (obj && env) {
      _obj = (JniType)env->NewGlobalRef(obj);
    }
  }

  operator JniType() const { return _obj; }

  void operator=(const JniLocalRef<JniType> &ref) { set(ref.get()); }

 private:
  JniType _obj;
};

} // namespace jni
} // namespace spotify

#endif // __JniGlobalRef_h__
