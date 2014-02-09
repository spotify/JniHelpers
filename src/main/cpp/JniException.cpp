#include "JniException.h"

namespace spotify {
namespace jni {

void JniException::checkException(JNIEnv *env) {

}

JniLocalRef<jobject> JniException::newException(JNIEnv *env, const char *message, ...) {
  JniLocalRef<jobject> result;
  return result;
}

void JniException::throwRuntimeException(JNIEnv *env, const char *message, ...) {

}

} // namespace jni
} // namespace spotify
