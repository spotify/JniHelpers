#include "JniHelpersTest.h"
#include "ByteArrayTest.h"
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
  gClasses.add(env, new ByteArrayTest(env));
  gClasses.add(env, new ClassRegistryTest(env));
  gClasses.add(env, new ClassWrapperTest(env));
  gClasses.add(env, new JavaClassUtilsTest(env));
  gClasses.add(env, new JavaExceptionUtilsTest(env));
  gClasses.add(env, new JavaStringTest(env));

  return JNI_VERSION_1_6;
}
