#ifndef __JniThreadUtils_h__
#define __JniThreadUtils_h__

#include "JniHelpers.h"

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

  static EXPORT void checkException(JNIEnv *env);
  static EXPORT JniLocalRef<jobject> newException(JNIEnv *env, const char *message, ...);
  static EXPORT void throwRuntimeException(JNIEnv *env, const char *message, ...);
};

} // namespace jni
} // namespace spotify

#endif // __JniThreadUtils_h__
