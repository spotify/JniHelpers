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

#ifndef __ClassRegistry_h__
#define __ClassRegistry_h__

#include "JniHelpersCommon.h"
#include "JavaClass.h"
#include "JavaExceptionUtils.h"
#include "NativeObject.h"
#include "ScopedPtr.h"
#include <map>
#include <string>
#include <string.h>

namespace spotify {
namespace jni {

#if WIN32
// TODO: This is a MSVC thing, should refactor to use PIMPL instead (ugh)
template class EXPORT std::map<std::string, ScopedPtr<const JavaClass> >;
#endif

typedef std::map<std::string, ScopedPtr<const JavaClass> > ClassRegistryMap;

/**
 * @brief Keeps a map of cached JavaClass instances
 *
 * The ClassRegistry class holds a map of bare (but initialized) JavaClass objects.
 * Since looking up class, field, and method definitions is somewhat expensive, it
 * makes sense to cache this data (and in fact, the JNI documentation also advises
 * doing this).
 *
 * Your program should keep an instance of the ClassRegistry either globally (with a
 * static object), or in some other long-lived object. During the JNI initialization
 * called via System.loadLibrary() in Java, you should add each JavaClass object
 * that you want to use in the map. Assuming that you have correctly subclassed the
 * JavaClass interface in your objects, they should be initialized and cache their
 * field and method data for quick access later.
 *
 * After the map has been configured, you can call newInstance<>() to create a new
 * JavaClass object of the desired type. The method/field IDs from the global map
 * will be merged into this object, thus making it relatively cheap to copy to/from
 * Java objects.
 */
class EXPORT ClassRegistry {
public:
  ClassRegistry();
  virtual ~ClassRegistry();

  /**
   * @brief Add a new class to the map
   * @param env JNIEnv
   * @param item JavaClass instance to add. This instance should be properly
   *             initialized (see the JavaClass documentation for more details).
   */
  virtual void add(JNIEnv *env, const JavaClass *item);

  /**
   * @brief Get an instance of a JavaClass definition in the map.
   * @param name Canonincal class name. If NULL, this method will throw an
   *             IllegalArgumentException to Java.
   * @return Pointer to instance, or NULL if no such instance exists.
   */
  virtual const JavaClass* get(const char *name) const;
  virtual const JavaClass* operator[](const char *name) const { return get(name); }

  /**
   * @brief Create a new instance of a given JavaClass subclass
   *
   * This method will create a new instance of the given type, which must subclass
   * the JavaClass abstract class. The instance will be populated with the cached
   * field and method IDs that were (presumably) set during initialization.
   *
   * If local field mappings have been configured with JavaClass::mapField, then
   * the new instance will be populated with data from the Java object.
   *
   * @param env JNIEnv
   * @param fromObject Java object to copy data from. If NULL, then a new object
   *                   will be created with the default constructor and class info
   *                   from merge().
   * @return New instance, or NULL if none could not be created.
   */
  template<typename TypeName>
  TypeName* newInstance(JNIEnv *env, jobject fromObject) {
    LOG_DEBUG("Creating new instance of class with registry info");
    TypeName *result = new TypeName();
    const char *name = result->getCanonicalName();
    if (name == NULL || strlen(name) == 0) {
      JavaExceptionUtils::throwExceptionOfType(env, kTypeIllegalArgumentException,
        "Could not find canonical name for class");
      delete result;
      return NULL;
    }

    const TypeName *classInfo = dynamic_cast<const TypeName*>(get(name));
    if (classInfo == NULL) {
      JavaExceptionUtils::throwExceptionOfType(env, kTypeIllegalStateException,
        "No class information registered for '%s'", name);
      delete result;
      return NULL;
    }

    result->merge(classInfo);
    result->mapFields();
    if (fromObject != NULL) {
      result->setJavaObject(env, fromObject);
    }

    return result;
  }

  template<typename TypeName>
  TypeName* getNativeInstance(JNIEnv *env, jobject fromObject) {
    LOG_DEBUG("Getting native instance of class from registry");
    TypeName *result = new TypeName();
    const char *name = result->getCanonicalName();
    if (name == NULL || strlen(name) == 0) {
      JavaExceptionUtils::throwExceptionOfType(env, kTypeIllegalArgumentException,
        "Could not find canonical name for class");
      delete result;
      return NULL;
    }

    const TypeName *classInfo = dynamic_cast<const TypeName*>(get(name));
    if (classInfo == NULL) {
      JavaExceptionUtils::throwExceptionOfType(env, kTypeIllegalStateException,
        "No class information registered for '%s'", name);
      delete result;
      return NULL;
    }

    if (classInfo->isInitialized()) {
      // Merge must be called so that cached fields information (namely for
      // the persisted long field pointer) can be found.
      result->merge(classInfo);
      JavaClass *instance = result->getPersistedInstance(env, fromObject);
      if (instance != NULL) {
        // Don't leak the result; we will instead return the object pointed to
        // by the persisted field pointer.
        delete result;
        return dynamic_cast<TypeName*>(instance);
      }
    }

    return NULL;
  }

  /**
   * @brief Return number of items in the map
   * @return Number of items
   */
  virtual size_t size() const {
    return _classes.size();
  }

protected:
  ClassRegistryMap _classes;
};

} // namespace jni
} // namespace spotify

#endif // __ClassRegistry_h__
