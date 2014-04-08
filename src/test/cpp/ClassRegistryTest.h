#ifndef __ClassRegistryTest_h__
#define __ClassRegistryTest_h__

#include "JniHelpersTest.h"
#include "JniHelpers.h"

class ClassWithName : public JavaClass {
public:
  ClassWithName(const char *name) : JavaClass(), _name(name) {}
  ClassWithName(JNIEnv *env) : JavaClass(env) { initialize(env); }
  ~ClassWithName() {}

  void initialize(JNIEnv *env) {}
  void mapFields() {}
  const char* getCanonicalName() const { return _name; }
  void setJavaObject(JNIEnv *env, jobject javaObject) {}
  jobject toJavaObject(JavaClass *nativeObject) { return NULL; }

private:
  const char* _name;
};

class EXPORT ClassRegistryTest : public JavaClass {
public:
  ClassRegistryTest() : JavaClass() {}
  ClassRegistryTest(JNIEnv *env) : JavaClass(env) { initialize(env); }
  ~ClassRegistryTest() {}

  void initialize(JNIEnv *env);
  void mapFields() {}
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
  static void getWithBracketOperator(JNIEnv *env, jobject javaThis);
  static void getNullClass(JNIEnv *env, jobject javaThis);
  static void getInvalidClass(JNIEnv *env, jobject javaThis);

  static void nativeNewInstance(JNIEnv *env, jobject javaThis, jobject javaTestObject);
  static void nativeNewInstanceWithNull(JNIEnv *env, jobject javaThis, jobject javaTestObject);
};

#endif // __ClassRegistryTest_h__
