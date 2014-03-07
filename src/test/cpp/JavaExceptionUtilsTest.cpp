#include "JavaExceptionUtilsTest.h"
#include "TestConstants.h"

void JavaExceptionUtilsTest::initialize(JNIEnv *env) {
  setClass(env);
  addNativeMethod("nativeThrowException", (void*)&JavaExceptionUtilsTest::nativeThrowException, kTypeVoid, NULL);
  addNativeMethod("nativeThrowExceptionWithFormatString", (void*)&JavaExceptionUtilsTest::nativeThrowExceptionWithFormatString, kTypeVoid, NULL);
  addNativeMethod("nativeThrowRuntimeException", (void*)&JavaExceptionUtilsTest::nativeThrowRuntimeException, kTypeVoid, NULL);
  addNativeMethod("nativeThrowExceptionOfType", (void*)&JavaExceptionUtilsTest::nativeThrowExceptionOfType, kTypeVoid, NULL);
  registerNativeMethods(env);
}

void JavaExceptionUtilsTest::nativeThrowException(JNIEnv *env, jobject javaThis) {
  JavaExceptionUtils::throwException(env, TEST_STRING);
}

void JavaExceptionUtilsTest::nativeThrowExceptionWithFormatString(JNIEnv *env, jobject javaThis) {
  JavaExceptionUtils::throwException(env, "%s, %d", TEST_STRING, TEST_INTEGER);
}

void JavaExceptionUtilsTest::nativeThrowRuntimeException(JNIEnv *env, jobject javaThis) {
  JavaExceptionUtils::throwRuntimeException(env, TEST_STRING);
}

void JavaExceptionUtilsTest::nativeThrowExceptionOfType(JNIEnv *env, jobject javaThis) {
  JavaExceptionUtils::throwExceptionOfType(env, kTypeUnsupportedOperationException, TEST_STRING);
}
