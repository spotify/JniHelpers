#include "JniHelpersTestInit.h"
#include "JniHelpersTest.h"

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *jvm, void*) {
  JNIEnv *env = NULL;

  // TODO: Error checking here
  jvm->GetEnv((void**)&env, JNI_VERSION_1_6);
  // TODO: Must set this:
  // JniClassUtils::setJavaClassLoaderForCurrentThread(env, NULL);
  gClasses.add(new JniHelpersTest(env));

  return JNI_VERSION_1_6;
}
