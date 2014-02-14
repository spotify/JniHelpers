#include "JniHelpers.h"

using namespace spotify::jni;

void jniHelpersInitialize(JavaVM *jvm, JNIEnv *env) {
  JavaThreadUtils::initialize(jvm);
  JavaClassUtils::setJavaClassLoaderForCurrentThread(env, NULL);
}
