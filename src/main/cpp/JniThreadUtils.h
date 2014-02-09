#ifndef __JniThreadUtils_h__
#define __JniThreadUtils_h__

#include "JniHelpersCommon.h"
#include <string>

namespace spotify {
namespace jni {

class JniThreadUtils {
private:
  // Direct instantiation not allowed
  JniThreadUtils() {}
  JniThreadUtils(const JniThreadUtils&) {}
  virtual ~JniThreadUtils() {}

public:
  static EXPORT JNIEnv* getEnvForCurrentThread();
  static EXPORT JNIEnv* attachCurrentThreadToJVM(const std::string &thread_name);
  static EXPORT void detatchCurrentThreadFromJVM();
};

} // namespace jni
} // namespace spotify

#endif // __JniThreadUtils_h__
