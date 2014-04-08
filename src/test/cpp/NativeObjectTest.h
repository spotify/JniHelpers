#ifndef __NativeObjectTest_h__
#define __NativeObjectTest_h__

#include "JniHelpers.h"
#include "JniHelpersTest.h"

using namespace spotify::jni;

class NativeObjectTest : public JavaClass {
public:
  NativeObjectTest() : JavaClass() {}
  NativeObjectTest(JNIEnv *env) : JavaClass(env) { initialize(env); }
  ~NativeObjectTest() {}

  const char* getCanonicalName() const {
    return MAKE_CANONICAL_NAME(PACKAGE, NativeObjectTest);
  }
  void initialize(JNIEnv *env);
  void mapFields() {}

private:
  static jobject createPersistedObject(JNIEnv *env, jobject javaThis);
  static jobject getPersistedInstance(JNIEnv *env, jobject javaThis, jobject object);
  static void nativeIsPersistenceEnabled(JNIEnv *env, jobject javaThis);
  static void isPersistenceEnabledWithoutInit(JNIEnv *env, jobject javaThis);
  static void destroyPersistedObject(JNIEnv *env, jobject javaThis, jobject object);
  static void persistInvalidClass(JNIEnv *env, jobject javaThis);
  static void persistNullObject(JNIEnv *env, jobject javaThis);
  static void destroyInvalidClass(JNIEnv *env, jobject javaThis);
  static void destroyNullObject(JNIEnv *env, jobject javaThis);
};

#endif // __NativeObjectTest_h__
