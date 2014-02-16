#include "ClassRegistryTest.h"

void ClassRegistryTest::initialize(JNIEnv *env) {
  setClass(env);

  addNativeMethod("addClass", &ClassRegistryTest::addClass, kTypeVoid, NULL);
  addNativeMethod("addNullItem", &ClassRegistryTest::addNullItem, kTypeVoid, NULL);
  addNativeMethod("addItemWithEmptyName", &ClassRegistryTest::addItemWithEmptyName, kTypeVoid, NULL);
  addNativeMethod("addItemWithoutJavaClass", &ClassRegistryTest::addItemWithoutJavaClass, kTypeVoid, NULL);
  addNativeMethod("addItemMultipleTimes", &ClassRegistryTest::addItemMultipleTimes, kTypeVoid, NULL);

  registerNativeMethods(env);
}

void ClassRegistryTest::addClass(JNIEnv *env, jobject javaThis) {
  //JavaExceptionUtils::throwRuntimeException(env, kSuccessMessage);
  JavaExceptionUtils::throwExceptionOfType(env, kSuccessClassName, kSuccessMessage);
}

void ClassRegistryTest::addNullItem(JNIEnv *env, jobject javaThis) {

}

void ClassRegistryTest::addItemWithEmptyName(JNIEnv *env, jobject javaThis) {

}

void ClassRegistryTest::addItemWithoutJavaClass(JNIEnv *env, jobject javaThis) {

}

void ClassRegistryTest::addItemMultipleTimes(JNIEnv *env, jobject javaThis) {

}
