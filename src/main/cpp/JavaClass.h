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

#ifndef __JavaClass_h__
#define __JavaClass_h__

#include "JniHelpersCommon.h"
#include "JavaString.h"
#include "JniGlobalRef.h"
#include "ScopedPtr.h"
#include <map>
#include <string>
#include <sstream>
#include <vector>
#include <stdarg.h>

namespace spotify {
namespace jni {

typedef struct {
  const char* type;
  void* address;
} FieldMapping;

#if WIN32
// TODO: This is a MSVC thing, should refactor to use PIMPL instead (ugh)
template class EXPORT std::vector<JNINativeMethod>;
template class EXPORT std::map<std::string, jfieldID>;
template class EXPORT std::map<std::string, ScopedPtr<const FieldMapping> >;
template class EXPORT std::map<std::string, jmethodID>;
#endif

typedef std::map<std::string, jfieldID> FieldMap;
typedef std::map<std::string, ScopedPtr<const FieldMapping> > FieldMappingMap; // meta meta!
typedef std::map<std::string, jmethodID> MethodMap;

/**
 * @brief Interface for a Java/C++ bridge class
 *
 * This interface serves as a bridge between Java/C++ classes, allowing one to easily
 * transfer data over JNI. You can use this class to sync data between Java/C++, or
 * simply to register native method callbacks. For example usage, see the test cases
 * distributed with JniHelpers.
 */
class EXPORT JavaClass {
// Constructors /////////////////////////////////////////////////////////////////////
public:
  /**
   * @brief Create a new empty instance.
   *
   * This method is invoked by ClassRegistry::newInstance, so it should have a minimal
   * implementation.
   */
  JavaClass();

  /**
   * @brief Create a new JavaClass with class information
   *
   * This method will be called by the top-level JNI initialization when the library
   * is loaded into memory. This constructor is expected to contain a call to the
   * initialize() method, which in turn will cache all necessary class, method, and
   * field data for quick access later on.
   *
   * Generally speaking, you should not create JavaClass objects with this ctor,
   * as it will be rather expensive to lookup all of the class information. Instead
   * use the empty ctor, or the ClassRegistry::newInstance method.
   *
   * NOTE: When you subclass JavaClass, you *must* call initialize() from within
   * this constructor.
   *
   * @param env JNIEnv
   */
  JavaClass(JNIEnv *env);

  virtual ~JavaClass();

// Pure virtual methods /////////////////////////////////////////////////////////////
public:
  /**
   * @brief Cache Java class information
   *
   * This method should take care of any necessary class information caching, namely
   * it must call setClass(), and can optionally call cacheMethod() and cacheField()
   * on any methods or fields that you might want to use later.
   *
   * Additionally, you may set up callbacks from here by calling addNativeMethod()
   * for each method, and then registerNativeMethods() at the end.
   *
   * @param env JNIEnv
   */
  virtual void initialize(JNIEnv *env) = 0;

  /**
   * @brief Configure field mappings per instance
   *
   * If you want to create field mappings (see mapField()), then you should do it
   * here. That will configure direct mappings between your class' local fields
   * and those of the Java world, which allow setJavaObject() and toJavaObject()
   * to automatically copy data between C++/Java class instances.
   *
   * If you don't care about syncing data between Java/C++ classes (for instance, in
   * the case of a class which is only designed to receive native callbacks), you
   * can ignore this method and simply implement it as an empty body.
   */
  virtual void mapFields() = 0;

  /**
   * @brief Get the canonical name for the associated Java class
   *
   * Note that the canonical name is in JNI notation, not Java notation. So if your
   * Java class is com.example.Foo, then this should return "com/example/Foo".
   * Preferably this method should return a pointer to some heap method; returning
   * a stack pointer will probably result in leaked memory.
   *
   * @return Canonincal name (may NOT be NULL, or other problems will result!)
   */
  virtual const char* getCanonicalName() const = 0;

// Public methods ///////////////////////////////////////////////////////////////////
public:
  /**
  * @brief Check to see if global class info is registered for this object
  *
  * Instances of JavaClass are still usable without having been initialized,
  * however most JNI-related operations such as toJavaObject() and setJavaObject()
  * will not work properly.
  *
  * @return True if this instance has valid class info, false otherwise
  */
  virtual bool isInitialized() const;

  /**
   * @brief Return the simple name for the class (ie, com.example.Foo -> "Foo")
   * @return Class name (should not be NULL)
   */
  virtual const char* getSimpleName() const;

  /**
   * @brief Copy cached class info data from the global instance
   *
   * If correctly configured, the ClassRegistry map should contains bare instances
   * of your JavaClass subclasses, each of which has been called with the
   * JavaClass(JNIEnv *env) ctor and therefore done some actual work in the
   * initialize(JNIEnv *env) method.
   *
   * When ClassRegistry::newInstance is called, it will create an empty instance of
   * this class and then populate it with data from a corresponding Java instance.
   * For this to work correctly, the cached class/field/method data which was fetched
   * during initialize() must be populated into this instance. That is done with this
   * method.
   *
   * @param globalInstance Global instance from ClassRegistry (should not be NULL)
   */
  virtual void merge(const JavaClass *globalInstance);

  /**
   * @brief Set data from a Java instance to this class
   *
   * This call is invoked by ClassRegistry::newInstance to set data from a Java
   * object into this instance. For this method to work, you need to set up field
   * mappings by overriding merge() and calling mapField(). See the documentation
   * for mapField() for more details.
   *
   * @param env JNIEnv
   * @param javaThis Java object to copy data from
   */
  virtual void setJavaObject(JNIEnv *env, jobject javaThis);

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

  /**
   * @brief Retreive a method from the cache
   * @param method_name Method name (without signature)
   * @return JNI method ID, or NULL if no such method was cached
   */
  virtual jmethodID getMethod(const char *method_name) const;

  /**
   * @brief Retreive a field from the cache
   * @param field_name Field name
   * @return JNI field ID, or NULL if no such field was cached
   */
  virtual jfieldID getField(const char* field_name) const;

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

  /**
   * @brief Cache a constructor
   *
   * This method should be called from within initialize() if you plan on using
   * toJavaObject() with this class instance. If you do not need toJavaObject(),
   * then it is not necessary to call this.
   *
   * Note that only no-argument constructors are supported at the moment. This is
   * a known limitation and will be addressed in a future version of the library.
   *
   * @param env JNIEnv
   */
  virtual void cacheConstructor(JNIEnv *env);

  /**
   * @brief Find a method and save it in the cache
   * @param env JNIEnv
   * @param method_name Method name, as it appears in the Java class
   * @param return_type Return type (see type definitions in JniTypes.h)
   * @param ... Method argument list, which *must* end with NULL. If the method
   *            takes no arguments, pass only NULL here instead of kTypeVoid.
   */
  virtual void cacheMethod(JNIEnv *env, const char *method_name, const char *return_type, ...);

  /**
   * @brief Find a field and save it in the cache
   * @param env JNIEnv
   * @param field_name Field name, as it appears in the Java class
   * @param field_type Field type (see type definitions in JniTypes.h)
   */
  virtual void cacheField(JNIEnv *env, const char *field_name, const char *field_type);

  /**
   * @brief Add a mapping between a Java field name and an instance of this class
   * @param field_name Field name
   * @param field_type Field type (see type definitions in JniTypes.h)
   * @param field_ptr Pointer to field. Must be public. :(
   */
  // TODO: Would be nice to be able to pass in a setter function, not sure how to manage that
  // At least, how to manage it without C++11 features or boost::bind...
  virtual void mapField(const char *field_name, const char *field_type, void *field_ptr);

  /**
   * @brief Get a field mapping for this class instance
   * @param key Field name
   * @return Field mapping, or NULL if no such field mapping exists
   */
  virtual const FieldMapping* getFieldMapping(const char *key) const;

  /**
   * @brief Add a native function callback to this class
   *
   * This method should be called in initialize() to register native methods to
   * be called from Java. After adding the methods, you must call the
   * registerNativeMethods() function at the end of initialize(), but only once!
   *
   * Note since JNI is fundamentally a C API, the function pointer must be static,
   * therefore the intended usage is to call ClassRegistry::newInstance from
   * within the static method in order to obtain a real object.
   *
   * @param method_name Method name
   * @param function Function to be called when the method is invoked from Java.
   * @param return_type Return type (see types declared in JniTypes.h)
   * @param ... Method argument list, which *must* end with NULL. If the method
   *            takes no arguments, pass only NULL here instead of kTypeVoid.
   */
  virtual void addNativeMethod(const char *method_name, void *function, const char *return_type, ...);

  /**
   * @brief Register all native methods on the class
   *
   * This method should be called *once* at the end of initialize(). It will tell
   * the JVM to register mappings between `native` Java methods and those which
   * you have created in calls to addNativeMethod().
   *
   * @param env JNIEnv
   * @return True if successful, false otherwise. If you register methods which
   *         do not exist in Java or have invalid signatures, then an exception
   *         may be thrown to Java.
   */
  virtual bool registerNativeMethods(JNIEnv *env);

// Fields ///////////////////////////////////////////////////////////////////////////
protected:
  jclass _clazz;
  jmethodID _default_constructor;
  const MethodMap *_methods;
  const FieldMap *_fields;
  FieldMappingMap _field_mappings;

protected:
  // This reference to the class information should *only* be held by the global
  // instance of the class held in the ClassRegistry. For this reason it is private,
  // and a pointer to the internal jclass is copied over to any locally created
  // instances during merging (ie, from newInstance).
  // Otherwise the global instance will be destroyed when any local instances are
  // destroyed, which would be bad.
  JniGlobalRef<jclass> _clazz_global;
  MethodMap _methods_global;
  FieldMap _fields_global;

private:
  std::vector<JNINativeMethod> _jni_methods;
};

} // namespace jni
} // namespace spotify

#endif // __JavaClass_h__
