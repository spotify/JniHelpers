#include "JavaStringTest.h"
#include "JUnitUtils.h"

void JavaStringTest::initialize(JNIEnv *env) {
  setClass(env);

  addNativeMethod("createJavaString", (void*)&JavaStringTest::createJavaString, kTypeVoid, NULL);
  //addNativeMethod("createJavaStringFromJavaString", (void*)&JavaStringTest::createJavaStringFromJavaString, kTypeVoid, kTypeString, NULL);
  addNativeMethod("nativeGetJavaString", (void*)&JavaStringTest::nativeGetJavaString, kTypeString, NULL);
  addNativeMethod("nativeSetValue", (void*)&JavaStringTest::nativeSetValue, kTypeVoid, kTypeString, NULL);

  registerNativeMethods(env);
}

void JavaStringTest::createJavaString(JNIEnv *env, jobject javaThis) {
  JavaString javaString;
  JUNIT_ASSERT_EQUALS_STRING("", javaString.getValue());
}

void JavaStringTest::createJavaStringFromJavaString(JNIEnv *env, jobject javaThis, jobject javaString) {
  JavaString helloString(env, (jstring)javaString);
  JUNIT_ASSERT_EQUALS_STRING(TEST_STRING, helloString.getValue());
}

jstring JavaStringTest::nativeGetJavaString(JNIEnv *env, jobject javaThis) {
  JavaString javaString(TEST_STRING);
  return javaString.getJavaString(env).leak();
}

void JavaStringTest::nativeSetValue(JNIEnv *env, jobject javaThis, jobject javaString) {
  JavaString helloString;
  helloString.setValue(env, (jstring)javaString);
  JUNIT_ASSERT_EQUALS_STRING(TEST_STRING, helloString.getValue());
}
