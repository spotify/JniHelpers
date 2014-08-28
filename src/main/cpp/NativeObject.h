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

#ifndef __NativeObject_h__
#define __NativeObject_h__

#include "JniHelpersCommon.h"
#include "JavaClass.h"

namespace spotify {
namespace jni {

#ifndef PERSIST_FIELD_NAME
  /**
  * @brief Field name to use for persisting objects (see persist() for details)
  */
#define PERSIST_FIELD_NAME "nPtr"
#endif

/**
 * @brief Interface for a Java/C++ bridge class with an associated C++ object
 *
 * This interface extends the base JavaClass and provides a mechanism to store a
 * non-copyable native object with the instance. This is useful for keeping track of
 * a C++ object instance that is not directly usable from within Java, but needed to
 * do work inside of native Java methods.
 *
 * In order for this class to work properly, the corresponding Java class must also
 * implement the `NativeObject` interface provided in the JniHelpers jar.
 *
 * The NativeObject instance is stored internally in a `long` field within the Java
 * object, and must be manually deleted since it cannot be garbage collected. For
 * example usage, see the test cases distributed with JniHelpers.
 */
class EXPORT NativeObject : public JavaClass {
// Constructors /////////////////////////////////////////////////////////////////////
public:
  NativeObject();
  NativeObject(JNIEnv *env);
  virtual ~NativeObject();

// JavaClass overrides //////////////////////////////////////////////////////////////
public:
  /**
   * @brief Check to see if global class info is registered for this object
   *
   * In addition to the isInitialized() check performed by the base class, NativeObject
   * also will check to see if the expected persistence field is present.
   *
   * @return True if this instance has valid class info, false otherwise
   */
  virtual bool isInitialized() const;

  /**
   * @brief Set data from this instance to a Java object
   *
   * This method creates a new Java representation of this JavaClass instance,
   * which is then populated with data from this object. The resulting object can
   * then be passed up to Java and used there. For this method to work properly,
   * you need to set up field mappings by overriding merge() and calling mapField().
   * See the documentation for mapField() for more details.
   *
   * This override will create a Java object using the default constructor, which
   * must be cached via cacheConstructor(). If you can't provide a default
   * constructor, then you should cache the constructor of your choice and create
   * a NewObject with it, which then can be passed to the other toJavaObject()
   * override.
   *
   * @param env JNIEnv
   * @return Initialized Java object, or NULL if the object could not be created
   */
  virtual jobject toJavaObject(JNIEnv *env);

  /**
   * @brief Set data from this instance to a Java object
   *
   * This method copies data to a Java representation of this JavaClass instance,
   * which is then populated with data from this object. The resulting object can
   * then be passed up to Java and used there. For this method to work properly,
   * you need to set up field mappings by overriding merge() and calling mapField().
   * See the documentation for mapField() for more details.
   *
   * @param env JNIEnv
   * @param javaThis Java object created with NewObject (in JNI)
   * @return Initialized Java object, or NULL if the object could not be created
   */
  virtual jobject toJavaObject(JNIEnv *env, jobject javaThis);

// Persistence calls ////////////////////////////////////////////////////////////////

  /**
   * @brief Persist this object as a reference in a Java object
   *
   * Sometimes you might need to create a C++ object with a long lifespan which may
   * be too expensive to keep copying between native and Java code. Or perhaps the
   * C++ object contains members which are not easily mapped to Java types and can't
   * be copied up to Java.
   *
   * In such cases, it makes more sense to create the C++ object on the heap and
   * pass a reference to it up to Java. In this manner, subsequent calls from Java
   * may return to native code and retrieve the native object instance directly.
   *
   * For this method to work, your Java class must declare a `long` field named
   * `PERSIST_FIELD_NAME` (which is defined to `nPtr`). If your class defines such
   * a field, then it will be used by ClassRegistry::newInstance to give you a
   * pointer to that instance instead of creating a new object.
   *
   * Note that this implies that you may only have one persisted native object in
   * memory at any given time. Also, you are responsible for disposing of it; the
   * preferred convention for this is to define a native method to call destroy().
   * This is perhaps a bit tedious, however that's one of the risks of mixing Java
   * and C++ lifecycles. Therefore, you should only use this if you absolutely
   * cannot copy object data to Java via the toJavaObject() method.
   *
   * Rather than make assumptions on how your application manages memory, calling
   * this method multiple times with different objects will leak them rather than
   * free them. To free the objects, you should call destroy() instead. You've been
   * warned!
   *
   * The restrictions of the hard-coded field name and limitation of one persisted
   * object may be removed in a future version of the library.
   *
   * @param env JNIEnv
   * @param javaThis Java object to set the field on (may not be NULL)
   * @return True if the object was persisted, false otherwise. If your class is
   *         incorrectly configured (ie, has no field named `nPtr`) then this
   *         method will return false rather than throwing.
   */
  virtual bool persist(JNIEnv *env, jobject javaThis);

  /**
   * @brief Return the object saved by a previous call to persist()
   * @param env JNIEnv
   * @param javaThis Java object to set the field from
   * @return Object instance, or NULL if no persisted instance exists.
   */
  virtual JavaClass* getPersistedInstance(JNIEnv *env, jobject javaThis) const;

  /**
   * @brief Free an object set with persist()
   *
   * If your class has been configured to persist objects (see the documentation in
   * persist() for more details), then this method will free the persisted object,
   * and set the `nPtr` field on the corresponding Java object to 0.
   *
   * This method is safe to call even if no object has been persisted in the Java
   * instance. In such cases, it does nothing.
   *
   * @param env JNIEnv
   * @param javaThis Java object to free the field on (may not be NULL)
   */
  virtual void destroy(JNIEnv *env, jobject javaThis);

// Internal helper calls ////////////////////////////////////////////////////////////
protected:
  /**
   * @brief Find and save JNI class information for this object
   *
   * This method uses getCanonicalName() to find the class and saves it to the
   * cached class info. This method should be called during initialize() or
   * you may get some unexpected behavior when trying to use instances of this
   * class.
   *
   * @param env JNIEnv
   */
  virtual void setClass(JNIEnv *env);
};

} // namespace jni
} // namespace spotify

#endif // __NativeObject_h__
