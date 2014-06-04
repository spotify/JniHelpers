#ifndef __JniHelpers_h__
#define __JniHelpers_h__

#include "JniHelpersCommon.h"

#include "ByteArray.h"
#include "ClassWrapper.h"
#include "ClassRegistry.h"
#include "JavaClassUtils.h"
#include "JavaExceptionUtils.h"
#include "JavaString.h"
#include "JavaThreadUtils.h"
#include "JniGlobalRef.h"
#include "JniLocalRef.h"
#include "ShortArray.h"

EXPORT JNIEnv *jniHelpersInitialize(JavaVM *jvm);
EXPORT JNIEnv *JniCurrentEnv(JavaVM *jvm);

#endif // __JniHelpers_h__
