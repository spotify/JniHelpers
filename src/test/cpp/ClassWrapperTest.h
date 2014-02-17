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

  void initialize(JNIEnv *env);
  const char* getCanonicalName() const {
    return MAKE_CANONICAL_NAME(PACKAGE, ClassWrapperTest);
  }

private:
  static void createClassWrapper(JNIEnv *env, jobject javaThis);
  static void testGetCanonicalName(JNIEnv *env, jobject javaThis);
  static void testGetSimpleName(JNIEnv *env, jobject javaThis);
  static void testMerge(JNIEnv *env, jobject javaThis);
  static jobject createPersistedObject(JNIEnv *env, jobject javaThis);
  static jobject getPersistedInstance(JNIEnv *env, jobject javaThis, jobject object);
  static void resetPersistedObject(JNIEnv *env, jobject javaThis, jobject object);
  static jboolean nativePersistInvalidClass(JNIEnv *env, jobject javaThis, jobject testObject);
  static void persistNullObject(JNIEnv *env, jobject javaThis);
  static void nativeResetPersistedObject(JNIEnv *env, jobject javaThis, jobject object);
  static jboolean nativeResetInvalidClass(JNIEnv *env, jobject javaThis, jobject testObject);
  static void resetNullObject(JNIEnv *env, jobject javaThis);
  static void nativeSetJavaObject(JNIEnv *env, jobject javaThis, jobject object);
  static jobject nativeToJavaObject(JNIEnv *env, jobject javaThis);
  static void getCachedMethod(JNIEnv *env, jobject javaThis);
  static void getCachedField(JNIEnv *env, jobject javaThis);
};

#endif // __ClassWrapperTest_h__
