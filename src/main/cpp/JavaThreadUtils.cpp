#include "JavaThreadUtils.h"

namespace spotify {
namespace jni {

JNIEnv* JavaThreadUtils::getEnvForCurrentThread() {
  return NULL;
}

JNIEnv* JavaThreadUtils::attachCurrentThreadToJVM(const std::string &thread_name) {
  return NULL;
}

void JavaThreadUtils::detatchCurrentThreadFromJVM() {

}

} // namespace jni
} // namespace spotify