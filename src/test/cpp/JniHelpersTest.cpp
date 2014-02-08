#include "JniHelpersTest.h"

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *jvm, void*) {
  JniHelper::initialize(jvm);
  JNIEnv *env;
  jvm->GetEnv((void**)&env, JNI_VERSION_1_6);
  JniHelper::get()->addClassWrapper(new JniHelperTest(env));
  return JNI_VERSION_1_6;
}

JniHelperTest::JniHelperTest(JNIEnv *env) : JniClassWrapper(env) {
}

void JniHelperTest::initialize(JNIEnv *env) {

}

const char* JniHelperTest::getClassName() const {
  return "com/spotify/jnihelper/JniHelpersTest";
}

JniClassWrapper* JniHelperTest::fromJavaObject(JNIEnv *env, jobject javaObject) const {
  return NULL;
}

jobject JniHelperTest::toJavaObject(JniClassWrapper *nativeObject) {
  return NULL;
}
