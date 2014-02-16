#ifndef __JniHelpersTest_h__
#define __JniHelpersTest_h__

#include "JniHelpers.h"

using namespace spotify::jni;

#define kPackageName "com/spotify/jni"
//extern const char *kPackageName;
extern const char *kSuccessClassName;
extern const char *kSuccessMessage;

extern ClassRegistry gClasses;

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *jvm, void *reserved);

#endif // __JniHelpersTest_h__
