#ifndef __JavaExceptionUtilsTest_h__
#define __JavaExceptionUtilsTest_h__

#include "JniHelpersTest.h"

class JavaExceptionUtilsTest : public JavaClass {
public:
  JavaExceptionUtilsTest() : JavaClass() {}
  JavaExceptionUtilsTest(JNIEnv *env) : JavaClass(env) { initialize(env); }
  ~JavaExceptionUtilsTest() {}

  const char* getCanonicalName() const {
    return MAKE_CANONICAL_NAME(PACKAGE, JavaExceptionUtilsTest);
  }

  void initialize(JNIEnv *env);
  void mapFields() {}

private:
  static void nativeThrowException(JNIEnv *env, jobject javaThis);
  static void nativeThrowExceptionWithFormatString(JNIEnv *env, jobject javaThis);
  static void nativeThrowRuntimeException(JNIEnv *env, jobject javaThis);
  static void nativeThrowExceptionOfType(JNIEnv *env, jobject javaThis);
};

#endif // __JavaExceptionUtilsTest_h__
