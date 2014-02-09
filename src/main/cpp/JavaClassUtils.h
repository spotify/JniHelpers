#ifndef __JavaClassUtils_h__
#define __JavaClassUtils_h__

#include "JniHelpersCommon.h"
#include <string>

namespace spotify {
namespace jni {

class JavaClassUtils {
private:
  // Direct instantiation not allowed for this class
  JavaClassUtils() {}
  JavaClassUtils(const JavaClassUtils&) {}
  virtual ~JavaClassUtils() {}

public:
  static EXPORT void setJavaClassLoaderForCurrentThread(JNIEnv *env, jobject class_loader);
  static EXPORT jobject lookupJavaClass(JNIEnv *env, const std::string &class_name);
};

} // namespace jni
} // namespace spotify

#endif // __JavaClassUtils_h__
