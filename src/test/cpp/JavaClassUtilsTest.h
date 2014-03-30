#ifndef __JavaClassUtilsTest_h__
#define __JavaClassUtilsTest_h__

#include "JniHelpersTest.h"

class JavaClassUtilsTest : public ClassWrapper {
public:
  JavaClassUtilsTest() : ClassWrapper() {}
  JavaClassUtilsTest(JNIEnv *env) : ClassWrapper(env) { initialize(env); }
  ~JavaClassUtilsTest() {}

  const char* getCanonicalName() const {
    return MAKE_CANONICAL_NAME(PACKAGE, JavaClassUtilsTest);
  }

  void initialize(JNIEnv *env);
  void mapFields() {}

public:
  static jclass findObjectClass(JNIEnv *env, jobject javaThis);
  static void findInvalidClass(JNIEnv *env, jobject javaThis);
  static jclass nativeFindClassWithLoader(JNIEnv *env, jobject javaThis);
  static void findInvalidClassWithLoader(JNIEnv *env, jobject javaThis);
  static void makeNameForSignatureWithNull(JNIEnv *env, jobject javaThis);
  static void makeNameForSignatureWithPrimitive(JNIEnv *env, jobject javaThis);
  static void makeNameForSignatureWithArray(JNIEnv *env, jobject javaThis);
  static void makeNameForSignatureWithObject(JNIEnv *env, jobject javaThis);
  static void makeNameForSignatureWithJniSignature(JNIEnv *env, jobject javaThis);
  static void makeNameForSignatureWithArrayOfObjects(JNIEnv *env, jobject javaThis);
};

#endif // __JavaClassUtilsTest_h__
