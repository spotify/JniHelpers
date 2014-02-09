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

} // namespace jni
} // namespace spotify