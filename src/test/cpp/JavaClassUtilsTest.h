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
};

#endif // __JavaClassUtilsTest_h__
