#include "JavaClassUtilsTest.h"
#include "JUnitUtils.h"
#include "JavaClassUtils.h"
#include "TestObject.h"

void JavaClassUtilsTest::initialize(JNIEnv *env) {
  setClass(env);
  addNativeMethod("findObjectClass", (void*)&findObjectClass, kTypeJavaClass(Class), NULL);
  addNativeMethod("findInvalidClass", (void*)&findInvalidClass, kTypeVoid, NULL);
  addNativeMethod("nativeFindClassWithLoader", (void*)&nativeFindClassWithLoader, kTypeJavaClass(Class), NULL);
  addNativeMethod("findInvalidClassWithLoader", (void*)&findInvalidClass, kTypeVoid, NULL);
  registerNativeMethods(env);
}

jclass JavaClassUtilsTest::findObjectClass(JNIEnv *env, jobject javaThis) {
  LOG_INFO("Starting test findObjectClass");
  return JavaClassUtils::findClass(env, "java/lang/Object", false);
}

void JavaClassUtilsTest::findInvalidClass(JNIEnv *env, jobject javaThis) {
  JavaClassUtils::findClass(env, "invalid", false);
}

jclass JavaClassUtilsTest::nativeFindClassWithLoader(JNIEnv *env, jobject javaThis) {
  LOG_INFO("Starting test nativeFindClassWithLoader");
  JavaClassUtilsTest dummy;
  return JavaClassUtils::findClass(env, dummy.getCanonicalName(), true);
}

void JavaClassUtilsTest::findInvalidClassWithLoader(JNIEnv *env, jobject javaThis) {
  JavaClassUtils::findClass(env, "invalid", true);
}
