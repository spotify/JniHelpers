#ifndef __ClassWrapperTest_h__
#define __ClassWrapperTest_h__

#include "JniHelpers.h"
#include "JniHelpersTest.h"

using namespace spotify::jni;

class ClassWrapperTest : public ClassWrapper {
public:
  ClassWrapperTest() : ClassWrapper() {}
  ClassWrapperTest(JNIEnv *env) : ClassWrapper(env) { initialize(env); }
  ~ClassWrapperTest() {}

  const char* getCanonicalName() const {
    return MAKE_CANONICAL_NAME(PACKAGE, ClassWrapperTest);
  }
  void initialize(JNIEnv *env);
  void mapFields() {}

private:
  static void createClassWrapper(JNIEnv *env, jobject javaThis);
  static void testGetCanonicalName(JNIEnv *env, jobject javaThis);
  static void testGetSimpleName(JNIEnv *env, jobject javaThis);
  static void testMerge(JNIEnv *env, jobject javaThis);
  static jobject createPersistedObject(JNIEnv *env, jobject javaThis);
  static jobject getPersistedInstance(JNIEnv *env, jobject javaThis, jobject object);
  static void destroyPersistedObject(JNIEnv *env, jobject javaThis, jobject object);
  static void persistInvalidClass(JNIEnv *env, jobject javaThis);
  static void persistNullObject(JNIEnv *env, jobject javaThis);
  static void destroyInvalidClass(JNIEnv *env, jobject javaThis);
  static void destroyNullObject(JNIEnv *env, jobject javaThis);
  static void nativeSetJavaObject(JNIEnv *env, jobject javaThis, jobject object);
  static jobject nativeToJavaObject(JNIEnv *env, jobject javaThis);
  static void getCachedMethod(JNIEnv *env, jobject javaThis);
  static void getInvalidCachedMethod(JNIEnv *env, jobject javaThis);
  static void getCachedMethodOnUninitialized(JNIEnv *env, jobject javaThis);
  static void getCachedField(JNIEnv *env, jobject javaThis);
  static void getInvalidCachedField(JNIEnv *env, jobject javaThis);
  static void getCachedFieldOnUninitialized(JNIEnv *env, jobject javaThis);
};

#endif // __ClassWrapperTest_h__
