#include "JniThreadUtils.h"

namespace spotify {
namespace jni {

JNIEnv* JniThreadUtils::getEnvForCurrentThread() {
  return NULL;
}

JNIEnv* JniThreadUtils::attachCurrentThreadToJVM(const std::string &thread_name) {
  return NULL;
}

void JniThreadUtils::detatchCurrentThreadFromJVM() {

}

void JniThreadUtils::checkException(JNIEnv *env) {

}

JniLocalRef<jobject> JniThreadUtils::newException(JNIEnv *env, const char *message, ...) {
  JniLocalRef<jobject> result;
  return result;
}

void JniThreadUtils::throwRuntimeException(JNIEnv *env, const char *message, ...) {

}

} // namespace jni
} // namespace spotify