#ifndef __JavaStringTest_h__
#define __JavaStringTest_h__

#include "JniHelpers.h"
#include "JniHelpersTest.h"

using namespace spotify::jni;

class EXPORT JavaStringTest : public ClassWrapper {
public:
  JavaStringTest() : ClassWrapper() {}
  JavaStringTest(JNIEnv *env) : ClassWrapper(env) {
    initialize(env);
  }
  ~JavaStringTest() {}

  void initialize(JNIEnv *env);
  void mapFields() {}
  const char* getCanonicalName() const {
    return MAKE_CANONICAL_NAME(PACKAGE, JavaStringTest);
  }

private:
  static bool supportsRawStringLiterals(JNIEnv *env);
  static void createJavaString(JNIEnv *env, jobject javaThis);
  static void createJavaStringFromStdString(JNIEnv *env, jobject javaThis);
  static void nativeCreateJavaStringFromJavaString(JNIEnv *env, jobject javaThis, jobject javaString);
  static jstring nativeGetJavaString(JNIEnv *env, jobject javaThis);
  static jstring nativeGetJavaStringWithNullChar(JNIEnv *env, jobject javaThis);
  static jstring nativeGetJavaStringUtf16(JNIEnv *env, jobject javaThis);
  static jstring nativeGetJavaStringUtf8(JNIEnv *env, jobject javaThis);
  static void nativeSetValue(JNIEnv *env, jobject javaThis, jobject javaString);
  static jstring nativeSetAndReturnValue(JNIEnv *env, jobject javaThis, jobject javaString);
};

#endif // __JavaStringTest_h__
