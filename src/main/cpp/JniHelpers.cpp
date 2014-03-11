#include "JniHelpers.h"

using namespace spotify::jni;

void jniHelpersInitialize(JavaVM *jvm, JNIEnv *env) {
  JavaThreadUtils::initialize(jvm);
  JavaClassUtils::setJavaClassLoaderForCurrentThread(env, NULL);
}

JNIEnv *JniCurrentEnv(JavaVM *jvm) {
  JNIEnv *env;

  if (jvm == NULL) {
    return NULL;
  }

  if (jvm->GetEnv((void**) &env, JNI_VERSION_1_4) != JNI_OK) {
    // The current thread isn't attached to a JNIEnv, return NULL.
    return NULL;
  }

  return env;
}
