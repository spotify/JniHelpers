#include "JniHelpersTest.h"
#include "ClassRegistryTest.h"
#include "ClassWrapperTest.h"
#include "JavaClassUtilsTest.h"
#include "JavaExceptionUtilsTest.h"
#include "JavaStringTest.h"
#include "JavaThreadUtilsTest.h"

//const char *kPackageName = "com/spotify/jni";
const char *kSuccessClassName = "com/spotify/jni/util/Success";
const char *kSuccessMessage = "Success";

ClassRegistry gClasses;

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *jvm, void*) {
  JNIEnv *env = NULL;

  // TODO: Error checking here
  jvm->GetEnv((void**)&env, JNI_VERSION_1_6);

  jniHelpersInitialize(jvm, env);
  gClasses.add(new ClassRegistryTest(env));

  return JNI_VERSION_1_6;
}
