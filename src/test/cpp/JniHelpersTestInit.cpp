#include "JniHelpersTestInit.h"
#include "JniHelpersTest.h"

ClassRegistry gClasses;

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *jvm, void*) {
  JNIEnv *env = NULL;

  // TODO: Error checking here
  jvm->GetEnv((void**)&env, JNI_VERSION_1_6);

  jniHelpersInitialize(jvm, env);
  gClasses.add(new JniHelpersTest(env));

  return JNI_VERSION_1_6;
}
