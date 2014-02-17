#ifndef __ClassRegistryTest_h__
#define __ClassRegistryTest_h__

#include "JniHelpersTest.h"
#include "JniHelpers.h"

class ClassWithName : public ClassWrapper {
public:
  ClassWithName(const char *name) : ClassWrapper(), _name(name) {}
  ClassWithName(JNIEnv *env) : ClassWrapper(env) { initialize(env); }
  ~ClassWithName() {}

  void initialize(JNIEnv *env) {}
  const char* getCanonicalName() const { return _name; }
  void setJavaObject(JNIEnv *env, jobject javaObject) {}
  jobject toJavaObject(ClassWrapper *nativeObject) { return NULL; }

private:
  const char* _name;
};

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

private:
  static void createRegistry(JNIEnv *env, jobject javaThis);

  static void addClass(JNIEnv *env, jobject javaThis);
  static void addNullClass(JNIEnv *env, jobject javaThis);
  static void addClassWithEmptyName(JNIEnv *env, jobject javaThis);
  static void addClassWithNullName(JNIEnv *env, jobject javaThis);
  static void addClassWithoutInfo(JNIEnv *env, jobject javaThis);
  static void addClassMultipleTimes(JNIEnv *env, jobject javaThis);

  static void get(JNIEnv *env, jobject javaThis);
  static void getNullClass(JNIEnv *env, jobject javaThis);
  static void getInvalidClass(JNIEnv *env, jobject javaThis);

  static void nativeNewInstance(JNIEnv *env, jobject javaThis, jobject javaTestObject);
  static void nativeNewInstanceWithNull(JNIEnv *env, jobject javaThis, jobject javaTestObject);
};

#endif // __ClassRegistryTest_h__
