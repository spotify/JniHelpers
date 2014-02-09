#include "JniHelpersTest.h"

JniHelpersTest::JniHelpersTest(JNIEnv *env) : JniClassWrapper(env) {
  initialize(env);
}

JniHelpersTest::JniHelpersTest(JNIEnv *env, jobject fromObject) : JniClassWrapper(env, fromObject) {
  setJavaObject(env, fromObject);
}

void JniHelpersTest::initialize(JNIEnv *env) {
  addNativeMethod("createClassWrapper", &JniHelpersTest::createClassWrapper,
    kTypeVoid, kTypeVoid, NULL);
  registerNativeMethods(env);
}

void JniHelpersTest::setJavaObject(JNIEnv *env, jobject javaObject) {
}

jobject JniHelpersTest::toJavaObject(JniClassWrapper *nativeObject) {
  return NULL;
}

void JniHelpersTest::createClassWrapper(JNIEnv *env, jobject object) {
  JniHelpersTest *instance = JniHelpersTest::newInstance();
  instance->setJavaObject(env, object);
  delete instance;
}
