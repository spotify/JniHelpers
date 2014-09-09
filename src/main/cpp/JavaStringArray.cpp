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

#include "JavaStringArray.h"
#include "JavaExceptionUtils.h"
#include "JavaString.h"
#include <stdlib.h>

namespace spotify {
namespace jni {

JavaStringArray::JavaStringArray() : _data(NULL), _num_elements(0) {}

JavaStringArray::JavaStringArray(JavaString **data, const size_t numElements, bool copyData) :
_data(NULL), _num_elements(0) {
  // In the rare (but possible) event that this constructor is called with
  // NULL but non-zero length data, correct the elements count so as to avoid
  // segfaults later on.
  if (data == NULL && numElements > 0) {
    _num_elements = 0;
  } else if (data != NULL && numElements > 0) {
    set(data, numElements, copyData);
  }
}

JavaStringArray::JavaStringArray(JNIEnv *env, jobjectArray data) :
_data(NULL), _num_elements(0) {
  set(env, data);
}

JavaStringArray::~JavaStringArray() {
  freeData();
}

JavaString **JavaStringArray::leak() {
  JavaString **result = _data;
  _data = NULL;
  _num_elements = 0;
  return result;
}

JniLocalRef<jobjectArray> JavaStringArray::toJavaStringArray(JNIEnv *env) const {
  jclass stringClass = env->FindClass(kTypeString);
  JniLocalRef<jobjectArray> result = env->NewObjectArray((jsize)_num_elements, stringClass, 0);
  JavaExceptionUtils::checkException(env);
  if (_num_elements == 0 || _data == NULL) {
    return result;
  }
  for(size_t i = 0; i < _num_elements; i++) {
    // This might be a potential problem as it's not entirely
    // certain who owns these.
    env->SetObjectArrayElement(result.get(), i, _data[i]->toJavaString(env));
  }
  return result.leak();
}

void JavaStringArray::set(JavaString **data, const size_t numElements, bool copyData) {
  if (data == NULL && numElements > 0) {
    JNIEnv *env = JavaThreadUtils::getEnvForCurrentThread();
    JavaExceptionUtils::throwExceptionOfType(env, kTypeIllegalArgumentException,
      "Cannot set data with non-zero size and NULL object");
    return;
  }

  // Make sure not to leak the old data if it exists
  freeData();

  if (copyData) {
    _data = (JavaString **)malloc(numElements * sizeof(JavaString *));
    for (size_t i = 0; i < numElements; i++) {
      _data[i] = data[i];
    }
  } else {
    _data = data;
  }
  _num_elements = numElements;
}

void JavaStringArray::set(JNIEnv *env, jobjectArray data) {
  freeData();

  if (data != NULL) {
    _num_elements = env->GetArrayLength(data);
    if (_num_elements == 0) {
      _data = NULL;
    } else {
      _data = (JavaString **)malloc(_num_elements * sizeof(JavaString *));
      for (size_t i = 0; i < _num_elements; i++) {
        _data[i] = new JavaString(env, (jstring) env->GetObjectArrayElement(data, i));
      }
    }
  }
}

void JavaStringArray::freeData() {
  if (_data != NULL) {
    for (size_t i = 0; i < size(); i++) {
      free(_data[i]);
    }
    free(_data);
    _data = NULL;
  }
}

} // namespace jni
} // namespace spotify
