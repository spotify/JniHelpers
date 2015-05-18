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

#ifndef __JavaArray_h__
#define __JavaArray_h__

#include "JniHelpersCommon.h"
#include "JniLocalRef.h"

namespace spotify {
namespace jni {

/**
 * @brief Class for holding Java byte[] data
 *
 * This class is used for passing raw data arrays through JNI. Internally
 * the data is stored as a void*.
 */
template<class JavaType, class NativeType>
class EXPORT JavaArray {
public:
  /**
   * @brief Create a new empty byte array
   */
  JavaArray() : _data(NULL), _num_elements(0) {}

  /**
   * @brief Create a new byte array with data.
   * @param data Pointer to data
   * @param numBytes Size of data pointed to
   * @param copyData True if the data should be copied to this instance
   *
   * See the documentation for set() regarding the ownership of data stored
   * in JavaArray instances.
   */
  JavaArray(NativeType data, const size_t numElements, bool copyData) :
    _data(NULL), _num_elements(0) {
    // In the rare (but possible) event that this constructor is called with
    // NULL but non-zero length data, correct the byte count so as to avoid
    // segfaults later on.
    if (data == NULL && numElements > 0) {
      _num_bytes = 0;
    } else if (data != NULL && numElements > 0) {
      set(data, numElements, copyData);
    }
  }

  virtual ~JavaArray() {
    if (_data != NULL) {
      free(_data);
      _data = NULL;
    }
  }

  /**
   * @brief Get a pointer to the natively stored data
   */
  virtual const NativeType get() const { return _data; }

  /**
   * @brief Convert data to a Java byte[] array
   */
  virtual JniLocalRef<JavaType> toJavaArray(JNIEnv *env) const = 0;

  /**
   * @brief Return a pointer to the data stored by this instance
   *
   * When an instance of JavaArray is destroyed, it will attempt to free
   * the data stored internally. If this data is still needed elsewhere,
   * then you should call leak() or else it will be unavailable.
   */
  virtual NativeType leak() {
    NativeType result = _data;
    _data = NULL;
    _num_elements = 0;
    return result;
  }

  /**
   * @brief Store data in this instance
   * @param data Pointer to data
   * @param numBytes Size of data pointed to
   * @param copyData True if the data should be copied to this instance
   *
   * If copyData is true, then this JavaArray instance owns that data and
   * the original data passed to this method can be freed without worry.
   * However, if copyData is false, then this JavaArray effectively just
   * points to that instance instead. In either case, after setting data
   * in a JavaArray, it effectively owns that data.
   *
   * When this JavaArray is destroyed, it will free the data stored in it,
   * regardless of how it has been set. This means that if copyData was
   * false and that data is still needed elsewhere, then a segfault will
   * probably occur when attempting to access that data after this object
   * has been destroyed. Thus, the leak() method can be used to remedy the
   * situation by removing the pointer to the data so the JavaArray will
   * not free it upon destruction.
   *
   * It is obviously more efficient to not copy the data, however this can
   * cause problems if your code does not respect the ownership behaviors
   * described above.
   */
  virtual void set(NativeType data, const size_t numElements, bool copyData) {
    if (data == NULL && numElements > 0) {
      JNIEnv *env = JavaThreadUtils::getEnvForCurrentThread();
      JavaExceptionUtils::throwExceptionOfType(env, kTypeIllegalArgumentException,
        "Cannot set data with non-zero size and NULL object");
      return;
    }

    // Make sure not to leak the old data if it exists
    if (_data != NULL) {
      free(_data);
    }

    if (copyData) {
      size_t numBytes = numElements * sizeof(NativeType);
      _data = malloc(numBytes);
      memcpy(_data, data, numBytes);
    } else {
      _data = data;
    }

    _num_elements = numElements;
  }

  /**
   * @brief Store data in this instance from a Java byte[] array
   * @param env JNIenv
   * @param data Java byte[] array
   */
  virtual void set(JNIEnv *env, JavaType data) = 0;

  /**
   * @brief Get the size of the data stored by this instance
   */
  virtual const size_t size() const { return _num_elements; }

protected:
  NativeType _data;
  size_t _num_elements;
};

} // namespace jni
} // namespace spotify

#endif // __JavaArray_h__
