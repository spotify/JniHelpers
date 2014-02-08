#include "JniHelper.h"

static JniHelper *gInstance = NULL;

JniHelper::JniHelper(JavaVM *jvm) {
  _jvm = jvm;
}

JniHelper::~JniHelper() {
  gInstance = NULL;
}

void JniHelper::initialize(JavaVM *jvm) {
  gInstance = new JniHelper(jvm);
}

JniHelper* JniHelper::get() {
  return gInstance;
}

JNIEnv* JniHelper::getEnvForCurrentThread() {
  return NULL;
}

JNIEnv* JniHelper::attachCurrentThreadToJVM(const std::string &thread_name) {
  return NULL;
}

void JniHelper::detatchCurrentThreadFromJVM() {

}

void JniHelper::setJavaClassLoaderForCurrentThread(JNIEnv *env, jobject class_loader) {

}

jobject JniHelper::lookupJavaClass(JNIEnv *env, const std::string &class_name) {
  return NULL;
}

void JniHelper::checkException(JNIEnv *env) {

}

JniLocalRef<jobject> JniHelper::newThrowable(JNIEnv *env, const char *message, ...) {
  JniLocalRef<jobject> result;
  return result;
}

void JniHelper::throwRuntimeException(JNIEnv *env, const char *message, ...) {

}

bool JniHelper::addClassWrapper(JniClassWrapper *jniClass) {
  return false;
}

JniClassWrapper* JniHelper::getClassWrapper(const char* name) const {
  return NULL;
}
