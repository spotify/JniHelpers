#ifndef __ClassRegistryTest_h__
#define __ClassRegistryTest_h__

#include "JniHelpersTest.h"
#include "JniHelpers.h"

class EXPORT ClassRegistryTest : public ClassWrapper {
public:
  ClassRegistryTest() : ClassWrapper() {}
  ClassRegistryTest(JNIEnv *env) : ClassWrapper(env) {
    initialize(env);
  }
  ~ClassRegistryTest() {}

  void initialize(JNIEnv *env);
  const char* getCanonicalName() const {
    return MAKE_CANONICAL_NAME(PACKAGE, ClassRegistryTest);
  }

  void setJavaObject(JNIEnv *env, jobject javaObject) {}
  jobject toJavaObject(ClassWrapper *nativeObject) {
    return NULL;
  }

private:
  static void addClass(JNIEnv *env, jobject javaThis);
  static void addNullItem(JNIEnv *env, jobject javaThis);
  static void addItemWithEmptyName(JNIEnv *env, jobject javaThis);
  static void addItemWithoutJavaClass(JNIEnv *env, jobject javaThis);
  static void addItemMultipleTimes(JNIEnv *env, jobject javaThis);
};

#endif // __ClassRegistryTest_h__
