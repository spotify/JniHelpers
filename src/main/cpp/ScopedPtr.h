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

#ifndef __ScopedPtr_h__
#define __ScopedPtr_h__

namespace spotify {
namespace jni {

/**
 * @brief Scoped pointer class
 *
 * Under normal circumstances it would be preferable to use the scoped
 * pointer provided by stl::scoped_ptr, or even boost, however both of
 * these libraries are a bit problematic under Android. Since we only
 * need the most basic implementation of a scoped pointer, re-inventing
 * the wheel is ok (in small doses).
 */
template<typename Type>
class ScopedPtr {
public:
    ScopedPtr() : _obj(NULL) {}
    ScopedPtr(Type *object) : _obj(NULL) { set(object); }
    ScopedPtr(const ScopedPtr<Type> &ref) : _obj(NULL) { set(ref.get()); }

    ~ScopedPtr() { set(NULL); }

    Type *get() const { return _obj; }

    void set(Type *object) {
        if (_obj) {
            delete _obj;
        }
        _obj = object;
    }

    Type *leak() {
        Type result = _obj;
        _obj = NULL;
        return result;
    }

private:
    Type *_obj;
};

} // namespace jni
} // namespace spotify

#endif // __ScopedPtr_h__
