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

#ifndef __JniTypes_h__
#define __JniTypes_h__

#include <string.h>

/** @brief Java int primitive */
#define kTypeInt "I"
/** @brief Java short primitive */
#define kTypeShort "S"
/** @brief Java long primitive */
#define kTypeLong "J"
/** @brief Java float primitive */
#define kTypeFloat "F"
/** @brief Java double primitive */
#define kTypeDouble "D"
/** @brief Java bool primitive */
#define kTypeBool "Z"
/** @brief Java byte primitive */
#define kTypeByte "B"
/** @brief Java char primitive */
#define kTypeChar "C"
/** @brief Java void */
#define kTypeVoid "V"
/** @brief Java String class */
#define kTypeString "java/lang/String"
/** @brief Java base Object class */
#define kTypeObject "java/lang/Object"

/**
 * @brief Java class builder macro
 *
 * It is not necessary to call this macro with a quoted string. For example:
 *
 * kTypeJavaClass(Object)
 *
 * Will produce the string "java/lang/Object".
 */
#define kTypeJavaClass(x) "java/lang/" #x

/** @brief Java base exception */
#define kTypeException kTypeJavaClass(Exception)
/** @brief Java illegal argument exception */
#define kTypeIllegalArgumentException kTypeJavaClass(IllegalArgumentException)
/** @brief Java illegal state exception */
#define kTypeIllegalStateException kTypeJavaClass(IllegalStateException)
/** @brief Java unsuported operation exception */
#define kTypeUnsupportedOperationException kTypeJavaClass(UnsupportedOperationException)

/**
 * @brief Java array builder macro for primitives
 *
 * Use this macro to make an array of primitives, for example:
 *
 * kTypeArray(kTypeInt)
 *
 * Will produce "[I", which corresponds to int[] in Java. Do not use this
 * macro for non-primitive (ie, Object) arrays, instead use kTypeObjectArray
 * for that.
 */
#define kTypeArray(x) "[" x

/**
 * @brief Java array builder macro for objects
 *
 * Use this macro to make an array of objects, for example:
 *
 * #define PACKAGE "com/example"
 * kTypeArray(MAKE_CANONICAL_NAME(PACKAGE, MyClass))
 *
 * Will produce "[Lcom/example/MyClass;", which corresponds to MyClass[] in
 * Java. Do not use this macro for arrays with primitive types, instead use
 * kTypeArray for that.
 *
 * Due to the limitations of the preprocessor, it is not possible to call this
 * macro with a char* string, instead it must be a string object visible to the
 * preprocessor itself. For this reason, use of MAKE_CANONICAL_NAME is advised.
 */
#define kTypeObjectArray(x) "[L" x ";"

/**
 * @brief Compare two Java types for equality
 */
#define TYPE_EQUALS(_TYPE1, _TYPE2) (strcmp(_TYPE1, _TYPE2) == 0)

#endif // __JniTypes_h__
