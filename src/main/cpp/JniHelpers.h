#ifndef __JniHelpers_h__
#define __JniHelpers_h__

#if WIN32
#define EXPORT __declspec(dllexport)
#endif

#include <jni.h>
#include "JniClassUtils.h"
#include "JniClassWrapper.h"
#include "JniGlobalRef.h"
#include "JniLocalRef.h"
#include "JniThreadUtils.h"

#endif // __JniHelpers_h__
