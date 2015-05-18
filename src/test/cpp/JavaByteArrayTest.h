#ifndef __JavaByteArrayTest_h__
#define __JavaByteArrayTest_h__

#include "JniHelpers.h"
#include "JniHelpersTest.h"

using namespace spotify::jni;

class JavaByteArrayTest : public JavaClass {
public:
  JavaByteArrayTest() : JavaClass() {}
  JavaByteArrayTest(JNIEnv *env) : JavaClass(env) {}
  ~JavaByteArrayTest() {}

  const char *getCanonicalName() const {
    return MAKE_CANONICAL_NAME(PACKAGE, JavaByteArrayTest);
  }
  void initialize(JNIEnv *env);
  void mapFields() {}
};

#endif // __JavaByteArrayTest_h__
