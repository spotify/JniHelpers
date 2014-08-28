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

#ifndef __JavaStringUtils_h__
#define __JavaStringUtils_h__

#include "JniHelpersCommon.h"
#include "JniLocalRef.h"
#include <string>

namespace spotify {
namespace jni {

#if WIN32
// TODO: This is a MSVC thing, should refactor to use PIMPL instead (ugh)
template class EXPORT std::basic_string<char, std::char_traits<char>, std::allocator<char>>;
#endif

class EXPORT JavaString {
public:
  JavaString();
  JavaString(const std::string &string);
  JavaString(JNIEnv *env, jstring javaString);
  virtual ~JavaString() {}

  const std::string& get() const;
  const std::string& operator*() const { return get(); }

  JniLocalRef<jstring> toJavaString(JNIEnv *env) const;

  void set(const char *value);
  void operator=(const char *value) { set(value); }
  void set(const std::string &value);
  void operator=(const std::string &value) { set(value); }
  void set(JNIEnv *env, jstring javaString);

public:
  std::string _value;
};

} // namespace jni
} // namespace spotify

#endif // __JavaStringUtils_h__
