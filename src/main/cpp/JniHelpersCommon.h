#ifndef __JniHelpersCommon_h__
#define __JniHelpersCommon_h__

#if WIN32
#define EXPORT __declspec(dllexport)
#endif

#include <jni.h>
#define _jvoid 'V'

#endif // __JniHelpersCommon_h__
