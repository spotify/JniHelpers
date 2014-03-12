#include "JniHelpers.h"

using namespace spotify::jni;

JNIEnv* jniHelpersInitialize(JavaVM *jvm) {
  JNIEnv *env = JavaThreadUtils::initialize(jvm);
  JavaClassUtils::setJavaClassLoaderForCurrentThread(env, NULL);
  return env;
}

