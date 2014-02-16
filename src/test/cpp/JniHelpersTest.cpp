#include "JniHelpersTest.h"
#include "ClassRegistryTest.h"
#include "ClassWrapperTest.h"
#include "JavaClassUtilsTest.h"
#include "JavaExceptionUtilsTest.h"
#include "JavaStringTest.h"
#include "JavaThreadUtilsTest.h"

ClassRegistry gClasses;

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *jvm, void*) {
  JNIEnv *env = NULL;

  // TODO: Error checking here
  jvm->GetEnv((void**)&env, JNI_VERSION_1_6);

  jniHelpersInitialize(jvm, env);
  gClasses.add(env, new ClassRegistryTest(env));

  return JNI_VERSION_1_6;
}
