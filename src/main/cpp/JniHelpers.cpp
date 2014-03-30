#include "JniHelpers.h"

using namespace spotify::jni;

JNIEnv* jniHelpersInitialize(JavaVM *jvm) {
  return JavaThreadUtils::initialize(jvm);
}

