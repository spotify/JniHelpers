#ifndef __JniHelpersTest_h__
#define __JniHelpersTest_h__

#include "JniHelpers.h"
#define PACKAGE "com/spotify/jni"

using namespace spotify::jni;
extern ClassRegistry gClasses;

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *jvm, void *reserved);

#endif // __JniHelpersTest_h__
