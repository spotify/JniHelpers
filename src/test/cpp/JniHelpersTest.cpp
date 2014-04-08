#include "JniHelpersTest.h"
#include "ByteArrayTest.h"
#include "ClassRegistryTest.h"
#include "JavaClassTest.h"
#include "JavaClassUtilsTest.h"
#include "JavaExceptionUtilsTest.h"
#include "JavaStringTest.h"
#include "JavaThreadUtilsTest.h"

ClassRegistry gClasses;

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *jvm, void*) {
  LOG_INFO("Initializing JNI");
  JNIEnv *env = jniHelpersInitialize(jvm);
  if (env == NULL) {
    return -1;
  }

  gClasses.add(env, new ByteArrayTest(env));
  gClasses.add(env, new ClassRegistryTest(env));
  gClasses.add(env, new JavaClassTest(env));
  gClasses.add(env, new JavaClassUtilsTest(env));
  gClasses.add(env, new JavaExceptionUtilsTest(env));
  gClasses.add(env, new JavaStringTest(env));

  LOG_INFO("Initialization complete");
  return JAVA_VERSION;
}
