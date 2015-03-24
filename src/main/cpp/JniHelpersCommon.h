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

#ifndef __JniHelpersCommon_h__
#define __JniHelpersCommon_h__

#include "JniTypes.h"
#include <jni.h>

// Disable some annoying compiler warnings
#if WIN32
#pragma warning(disable: 4996) // "Security" warnings for vsnprintf
#endif

#if WIN32
#define EXPORT __declspec(dllexport)
#else
#define EXPORT
#endif

// What version of Java should be used when initializing JniHelpers. By default this
// is defined to be Java 1.6.
#ifndef JAVA_VERSION
#define JAVA_VERSION JNI_VERSION_1_6
#endif

// Whether string literals are supported by the platform. This is a C++11 feature
// which the Visual C++ does not yet support. Currently this feature is only used
// by the test suite.
#if ! WIN32
#define HAS_RAW_STRING_LITERALS 1
#endif

// Whether the JVM should be forcibly terminated if an exception is thrown. This does
// not apply to exceptions thrown from within JniHelpers or within Java code attached
// to JNI code. This is mostly relevant when looking up classes/methods/fields which
// do not exist, and thus represent a misconfiguration on the programmer's end. In such
// cases it is sometimes prudent to forcibly quit rather than try to continue running.
#ifndef TERMINATE_ON_EXCEPTION
#define TERMINATE_ON_EXCEPTION 0
#endif

// Windows uses _DEBUG by default, but we prefer plain 'ol DEBUG instead
#if WIN32
#define DEBUG _DEBUG
#endif

// Determines whether exceptions will be raised by JniHelpers
#ifndef ENABLE_EXCEPTIONS
#define ENABLE_EXCEPTIONS 1
#endif

// Determines whether logging messages will be printed by JniHelpers
#ifndef ENABLE_LOGGING
#define ENABLE_LOGGING 0
#endif

// Determines whether debug/info logging messages will be printed by JniHelpers
#ifndef ENABLE_LOGGING_DEBUG
#define ENABLE_LOGGING_DEBUG DEBUG
#endif

#if ENABLE_LOGGING
#if ANDROID
#include <android/log.h>
#define LOGGING_TAG "JniHelpers"
#if ENABLE_LOGGING_DEBUG
#define LOG_DEBUG(...) __android_log_print(ANDROID_LOG_INFO, LOGGING_TAG, __VA_ARGS__)
#define LOG_INFO(...) __android_log_print(ANDROID_LOG_INFO, LOGGING_TAG, __VA_ARGS__)
#else
#define LOG_DEBUG(...)
#define LOG_INFO(...)
#endif
#define LOG_WARN(...) __android_log_print(ANDROID_LOG_WARN, LOGGING_TAG, __VA_ARGS__)
#define LOG_ERROR(...) __android_log_print(ANDROID_LOG_ERROR, LOGGING_TAG, __VA_ARGS__)
#else
#if ENABLE_LOGGING_DEBUG
#define LOG_DEBUG(...) fprintf(stderr, "DEBUG: "); fprintf(stderr, __VA_ARGS__); fprintf(stderr, "\n")
#define LOG_INFO(...) fprintf(stderr, "INFO: "); fprintf(stderr, __VA_ARGS__); fprintf(stderr, "\n")
#else
#define LOG_DEBUG(...)
#define LOG_INFO(...)
#endif
#define LOG_WARN(...) fprintf(stderr, "WARN: "); fprintf(stderr, __VA_ARGS__); fprintf(stderr, "\n")
#define LOG_ERROR(...) fprintf(stderr, "ERROR: "); fprintf(stderr, __VA_ARGS__); fprintf(stderr, "\n")
#endif
#else
#define LOG_DEBUG(...)
#define LOG_INFO(...)
#define LOG_WARN(...)
#define LOG_ERROR(...)
#endif

#endif // __JniHelpersCommon_h__
