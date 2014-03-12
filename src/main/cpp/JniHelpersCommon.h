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

#include <android/log.h>
#define LIB_NAME "JniHelpers"

#define ILOG_LIB(...)    __android_log_print(ANDROID_LOG_INFO, LIB_NAME, __VA_ARGS__);
#define WLOG_LIB(...)    __android_log_print(ANDROID_LOG_WARN, LIB_NAME, __VA_ARGS__);
#define ELOG_LIB(...)    __android_log_print(ANDROID_LOG_ERROR, LIB_NAME, __VA_ARGS__);

#endif // __JniHelpersCommon_h__
