#include "JavaExceptionUtils.h"

namespace spotify {
namespace jni {

void JavaExceptionUtils::checkException(JNIEnv *env) {

}

JniLocalRef<jobject> JavaExceptionUtils::newException(JNIEnv *env, const char *message, ...) {
  JniLocalRef<jobject> result;
  return result;
}

void JavaExceptionUtils::throwRuntimeException(JNIEnv *env, const char *message, ...) {

}

} // namespace jni
} // namespace spotify
