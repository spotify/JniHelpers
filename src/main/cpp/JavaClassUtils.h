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

#ifndef __JavaClassUtils_h__
#define __JavaClassUtils_h__

#include "JniHelpersCommon.h"
#include <string>
#include <stdarg.h>

namespace spotify {
namespace jni {

/**
 * @brief Convenience macro to create canonical name for a class
 *
 * Note that the class name should *not* be a quoted string, otherwise the quotes
 * will be added literally to the string. Example usage:
 *
 * #define PACKAGE "com/example/stuff"
 * MAKE_CANONICAL_NAME(PACKAGE, Foo)
 *
 * In the above example, the PACKAGE definition should ideally be placed in a header
 * file which is accessible to your package sources. The above macro will then produce
 * the string "com/example/stuff/Foo".
 */
#define MAKE_CANONICAL_NAME(_PACKAGE, _CLASS) _PACKAGE "/" #_CLASS

class JavaClassUtils {
private:
  // Direct instantiation is not allowed for this class
  JavaClassUtils() {}
  JavaClassUtils(const JavaClassUtils&) {}
  virtual ~JavaClassUtils() {}

public:
  /**
   * @brief Cache an instance of the Java ClassLoader
   * @param env JNIEnv
   *
   * If you intend to use findClass() with a Java ClassLoader, then this method will be
   * invoked automatically to initialize a static JavaClass instance of this class,
   * which is needed by findClass() to do the actual lookup. Since initialization of
   * JavaClass instances may take a bit of time, this method is exposed in case you
   * wish to manually call it during JNI initialization.
   *
   * In most cases, it should not be necessary to explicitly call this method from your
   * code.
   */
  static EXPORT void setJavaClassLoader(JNIEnv *env);

  /**
   * @brief Look up a Java class object
   * @param env JNIEnv
   * @param class_name Canonical class name
   * @param use_class_loader Use a Java ClassLoader instance. This is not necessary for
   *                         looking up Java classes, however it may be needed in other
   *                         cases, particularly when trying to load your own classes in
   *                         Android.
   * @return Java class object, or NULL if none was found
   *
   * This method will throw NoClassDefFoundError if the class could not be found.
   */
  static EXPORT jclass findClass(JNIEnv *env, const char *class_name, bool use_class_loader);

  /**
   * @brief Make a name safe to pass to a method requiring JNI signatures
   * @param receiver String to recieve the generated signature
   * @param name Name to convert
   *
   * When looking up field or method names which involve another class name, it may be
   * necessary to convert these to the corresponding JNI signature types (for example,
   * Lcom/example/MyClass; for com.example.MyClass). This method will create a JNI
   * signature string based on a canonical name.
   *
   * Passing a JNI signature to this class should have no effect, it will simply return
   * the same string to the receiver.
   */
  static EXPORT void makeNameForSignature(std::string &receiver, const char *name);

  /**
    * @brief Make a method signature from a variable list of arguments
    * @param receiver String to receive the generated signature
    * @param return_type Return type of function (see JniTypes.h)
    * @param ... List of arguments which function takes, ending with NULL. If the method
    *            takes no arguments, just pass NULL here. If you do not pass NULL as the
    *            last argument to this method, unexpected behavior will occur!
    */
  static EXPORT void makeSignature(std::string &receiver, const char *return_type, ...);

  /**
   * @brief Make a method signature from a variable list of arguments
   * @param receiver String to receive the generated signature
   * @param return_type Return type of function (see JniTypes.h)
   * @param arguments List of arguments which function takes
   */
  static EXPORT void makeSignatureWithList(std::string &receiver, const char *return_type, va_list arguments);
};

} // namespace jni
} // namespace spotify

#endif // __JavaClassUtils_h__
