#ifndef __JniHelpersTestInit_h__
#define __JniHelpersTestInit_h__

#include "JniHelpers.h"

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *jvm, void *reserved);

using namespace spotify::jni;
static ClassRegistry gClasses;

#endif // __JniHelpersTestInit_h__
