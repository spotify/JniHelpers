#include "JavaByteArrayTest.h"
#include "JavaByteArray.h"

void JavaByteArrayTest::initialize(JNIEnv *env) {
  setClass(env);
  JavaByteArray foo;
}