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
  static EXPORT JavaVM* getJavaVM();
  static EXPORT JNIEnv* initialize(JavaVM* jvm);
  static EXPORT JNIEnv* getEnvForCurrentThread();  
  static EXPORT JNIEnv* getEnvForCurrentThread(JavaVM *jvm);
  static EXPORT JNIEnv* attachCurrentThreadToJVM(const char* thread_name);
  static EXPORT void detatchCurrentThreadFromJVM();
};

} // namespace jni
} // namespace spotify

#endif // __JavaThreadUtils_h__
