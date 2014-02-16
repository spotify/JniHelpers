#ifndef __TestObject_h__
#define __TestObject_h__

#include "JniHelpers.h"
#include "JniHelpersTest.h"

using namespace spotify::jni;

class TestObject : public ClassWrapper {
public:
  TestObject();
  TestObject(JNIEnv *env);
  ~TestObject() {}

  const char* getCanonicalName() const {
    return MAKE_CANONICAL_NAME(PACKAGE, TestObject);
  }

  void initialize(JNIEnv *env);
  void setJavaObject(JNIEnv *env, jobject javaObject);
  jobject toJavaObject(ClassWrapper *nativeObject);

private:
  JavaString s;
  int i;
  float f;
  // TODO: byte[] b;
};

#endif // __TestObject_h__
