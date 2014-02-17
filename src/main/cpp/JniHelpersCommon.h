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

#endif // __JniHelpersCommon_h__
