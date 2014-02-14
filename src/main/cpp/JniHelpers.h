#ifndef __JniHelpers_h__
#define __JniHelpers_h__

#include "JniHelpersCommon.h"

#include "ClassWrapper.h"
#include "ClassRegistry.h"
#include "JavaClassUtils.h"
#include "JavaExceptionUtils.h"
#include "JavaString.h"
#include "JavaThreadUtils.h"
#include "JniGlobalRef.h"
#include "JniLocalRef.h"

EXPORT void jniHelpersInitialize(JavaVM *jvm, JNIEnv *env);

#endif // __JniHelpers_h__
