#ifndef __JavaThreadUtils_h__
#define __JavaThreadUtils_h__

#include "JniHelpersCommon.h"
#include <string>

namespace spotify {
namespace jni {

class JavaThreadUtils {
private:
  // Direct instantiation not allowed
  JavaThreadUtils() {}
  JavaThreadUtils(const JavaThreadUtils&) {}
  virtual ~JavaThreadUtils() {}

public:
  static EXPORT JNIEnv* getEnvForCurrentThread();
  static EXPORT JNIEnv* attachCurrentThreadToJVM(const std::string &thread_name);
  static EXPORT void detatchCurrentThreadFromJVM();
};

} // namespace jni
} // namespace spotify

#endif // __JavaThreadUtils_h__
