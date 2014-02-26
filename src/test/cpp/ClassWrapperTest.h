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
  void mapFields() {}
  const char* getCanonicalName() const {
    return MAKE_CANONICAL_NAME(PACKAGE, ClassWrapperTest);
  }

private:
  static void createClassWrapper(JNIEnv *env, jobject javaThis);
  static void testGetCanonicalName(JNIEnv *env, jobject javaThis);
  static void testGetSimpleName(JNIEnv *env, jobject javaThis);
  static void testMerge(JNIEnv *env, jobject javaThis);
  static JniLocalRef<jobject> createPersistedObject(JNIEnv *env, jobject javaThis);
  static JniLocalRef<jobject> getPersistedInstance(JNIEnv *env, jobject javaThis, jobject object);
  static void destroyPersistedObject(JNIEnv *env, jobject javaThis, jobject object);
  static jboolean nativePersistInvalidClass(JNIEnv *env, jobject javaThis, jobject testObject);
  static void persistNullObject(JNIEnv *env, jobject javaThis);
  static void nativeDestroyPersistedObject(JNIEnv *env, jobject javaThis, jobject object);
  static jboolean nativeDestroyInvalidClass(JNIEnv *env, jobject javaThis, jobject testObject);
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
