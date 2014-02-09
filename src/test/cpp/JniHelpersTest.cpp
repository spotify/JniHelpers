#include "JniHelpersTest.h"

void JniHelpersTest::initialize(JNIEnv *env) {
  addNativeMethod("createClassWrapper", &JniHelpersTest::createClassWrapper,
    kTypeVoid, kTypeVoid, NULL);
  registerNativeMethods(env);
}

void JniHelpersTest::setJavaObject(JNIEnv *env, jobject javaObject) {
}

jobject JniHelpersTest::toJavaObject(ClassWrapper *nativeObject) {
  return NULL;
}

void JniHelpersTest::createClassWrapper(JNIEnv *env, jobject object) {
  JniHelpersTest *instance = gClasses.newInstance<JniHelpersTest>(env, object);
  delete instance;
}
