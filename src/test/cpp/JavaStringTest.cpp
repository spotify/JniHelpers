#include "JavaStringTest.h"
#include "JUnitUtils.h"

void JavaStringTest::initialize(JNIEnv *env) {
  setClass(env);

  addNativeMethod("supportsRawStringLiterals", (void*)supportsRawStringLiterals, kTypeBool, NULL);

  addNativeMethod("createJavaString", (void*)&JavaStringTest::createJavaString, kTypeVoid, NULL);
  addNativeMethod("createJavaStringFromStdString", (void*)&JavaStringTest::createJavaStringFromStdString, kTypeVoid, NULL);
  addNativeMethod("nativeCreateJavaStringFromJavaString", (void*)&JavaStringTest::nativeCreateJavaStringFromJavaString, kTypeVoid, kTypeString, NULL);

  addNativeMethod("nativeGetJavaString", (void*)&JavaStringTest::nativeGetJavaString, kTypeString, NULL);
  addNativeMethod("nativeGetJavaStringWithNullChar", (void*)&JavaStringTest::nativeGetJavaStringWithNullChar, kTypeString, NULL);
  addNativeMethod("nativeGetJavaStringUtf16", (void*)&JavaStringTest::nativeGetJavaStringUtf16, kTypeString, NULL);
  addNativeMethod("nativeGetJavaStringUtf8", (void*)&JavaStringTest::nativeGetJavaStringUtf8, kTypeString, NULL);

  addNativeMethod("nativeSetValue", (void*)&JavaStringTest::nativeSetValue, kTypeVoid, kTypeString, NULL);
  addNativeMethod("nativeSetValueWithOperator", (void*)nativeSetValueWithOperator, kTypeVoid, NULL);
  addNativeMethod("nativeSetAndReturnValue", (void*)&JavaStringTest::nativeSetAndReturnValue, kTypeString, kTypeString, NULL);

  registerNativeMethods(env);
}

bool JavaStringTest::supportsRawStringLiterals(JNIEnv *env) {
#if HAS_RAW_STRING_LITERALS
  return true;
#else
  return false;
#endif
}

void JavaStringTest::createJavaString(JNIEnv *env, jobject javaThis) {
  JavaString javaString;
  JUNIT_ASSERT_EQUALS_STRING("", javaString.get());
}

void JavaStringTest::createJavaStringFromStdString(JNIEnv *env, jobject javaThis) {
  std::string stdString = TEST_STRING;
  JavaString javaString(stdString);
  JUNIT_ASSERT_EQUALS_STRING(TEST_STRING, javaString.get());
}

void JavaStringTest::nativeCreateJavaStringFromJavaString(JNIEnv *env, jobject javaThis, jobject javaString) {
  JavaString testString(env, (jstring)javaString);
  JUNIT_ASSERT_EQUALS_STRING(TEST_STRING, testString.get());
}

jstring JavaStringTest::nativeGetJavaString(JNIEnv *env, jobject javaThis) {
  JavaString javaString(TEST_STRING);
  return javaString.toJavaString(env).leak();
}

jstring JavaStringTest::nativeGetJavaStringWithNullChar(JNIEnv *env, jobject javaThis) {
  JavaString javaString(TEST_STRING_WITH_NULL_CHAR);
  return javaString.toJavaString(env).leak();
}

jstring JavaStringTest::nativeGetJavaStringUtf16(JNIEnv *env, jobject javaThis) {
  // This test is disabled on the Java side.
  // We can't construct from utf16 strings yet.
  JavaString javaString; //(TEST_UTF16_STRING);
  return javaString.toJavaString(env).leak();
}

jstring JavaStringTest::nativeGetJavaStringUtf8(JNIEnv *env, jobject javaThis) {
#if HAS_RAW_STRING_LITERALS
  JavaString javaString(TEST_UTF8_STRING);
  return javaString.toJavaString(env).leak();
#else
  return NULL;
#endif
}

void JavaStringTest::nativeSetValue(JNIEnv *env, jobject javaThis, jobject javaString) {
  JavaString testString;
  testString.set(env, (jstring)javaString);
  JUNIT_ASSERT_EQUALS_STRING(TEST_STRING, testString.get());
}

void JavaStringTest::nativeSetValueWithOperator(JNIEnv *env, jobject javaThis) {
  JavaString testString;
  testString = TEST_STRING;
  JUNIT_ASSERT_EQUALS_STRING(TEST_STRING, testString.get());
}

jstring JavaStringTest::nativeSetAndReturnValue(JNIEnv *env, jobject javaThis, jobject javaString) {
  JavaString testString;
  testString.set(env, (jstring)javaString);
  return testString.toJavaString(env).leak();
}
