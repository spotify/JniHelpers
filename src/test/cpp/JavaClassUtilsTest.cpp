#include "JavaClassUtilsTest.h"
#include "JavaClassUtils.h"

void JavaClassUtilsTest::initialize(JNIEnv *env) {
  setClass(env);
  addNativeMethod("findObjectClass", (void*)&findObjectClass, kTypeJavaClass(Class), NULL);
  addNativeMethod("nativeFindClassWithLoader", (void*)&nativeFindClassWithLoader, kTypeJavaClass(Class), NULL);
  registerNativeMethods(env);
}

jclass JavaClassUtilsTest::findObjectClass(JNIEnv *env, jobject javaThis) {
  LOG_INFO("Starting test findObjectClass");
  return JavaClassUtils::findClass(env, "java/lang/Object", false);
}

jclass JavaClassUtilsTest::nativeFindClassWithLoader(JNIEnv *env, jobject javaThis) {
  LOG_INFO("Starting test nativeFindClassWithLoader");
  JavaClassUtilsTest dummy;
  return JavaClassUtils::findClass(env, dummy.getCanonicalName(), true);
}
