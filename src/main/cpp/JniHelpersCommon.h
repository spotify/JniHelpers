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

// Whether the JVM should be forcibly terminated if an exception is thrown. This does
// not apply to exceptions thrown from within JniHelpers or within Java code attached
// to JNI code. This is mostly relevant when looking up classes/methods/fields which
// do not exist, and thus represent a misconfiguration on the programmer's end. In such
// cases it is sometimes prudent to forcibly quit rather than try to continue running.
#ifndef TERMINATE_ON_EXCEPTION
#define TERMINATE_ON_EXCEPTION 0
#endif

#ifndef ENABLE_LOGGING
#define ENABLE_LOGGING DEBUG
#endif

#if ANDROID
#include <android/log.h>
#define LOGGING_TAG "JniHelpers"

#define LOG_DEBUG(...) __android_log_print(ANDROID_LOG_INFO, LOGGING_TAG, __VA_ARGS__);
#define LOG_INFO(...) __android_log_print(ANDROID_LOG_INFO, LOGGING_TAG, __VA_ARGS__);
#define LOG_WARN(...) __android_log_print(ANDROID_LOG_WARN, LOGGING_TAG, __VA_ARGS__);
#define LOG_ERROR(...) __android_log_print(ANDROID_LOG_ERROR, LOGGING_TAG, __VA_ARGS__);
#else
#define LOG_DEBUG(...) fprintf(stderr, "DEBUG: "); fprintf(stderr, __VA_ARGS__); fprintf(stderr, "\n");
#define LOG_INFO(...) fprintf(stderr, "INFO: "); fprintf(stderr, __VA_ARGS__); fprintf(stderr, "\n");
#define LOG_WARN(...) fprintf(stderr, "WARN: "); fprintf(stderr, __VA_ARGS__); fprintf(stderr, "\n");
#define LOG_ERROR(...) fprintf(stderr, "ERROR: "); fprintf(stderr, __VA_ARGS__); fprintf(stderr, "\n");
#endif

#endif // __JniHelpersCommon_h__
