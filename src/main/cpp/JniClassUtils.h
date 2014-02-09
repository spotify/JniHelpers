#ifndef __JniClassUtils_h__
#define __JniClassUtils_h__

#include "JniHelpers.h"
#include <string>

namespace spotify {
namespace jni {

class JniClassUtils {
private:
  // Direct instantiation not allowed for this class
  JniClassUtils() {}
  JniClassUtils(const JniClassUtils&) {}
  virtual ~JniClassUtils() {}

public:
  static EXPORT void setJavaClassLoaderForCurrentThread(JNIEnv *env, jobject class_loader);
  static EXPORT jobject lookupJavaClass(JNIEnv *env, const std::string &class_name);
};

} // namespace jni
} // namespace spotify

#endif // __JniClassUtils_h__
