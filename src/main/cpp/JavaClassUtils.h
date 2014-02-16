#ifndef __JavaClassUtils_h__
#define __JavaClassUtils_h__

#include "JniHelpersCommon.h"
#include <string>

namespace spotify {
namespace jni {

#define MAKE_CANONICAL_NAME(_PACKAGE, _CLASS) _PACKAGE "/" #_CLASS

class JavaClassUtils {
private:
  // Direct instantiation not allowed for this class
  JavaClassUtils() {}
  JavaClassUtils(const JavaClassUtils&) {}
  virtual ~JavaClassUtils() {}

public:
  static EXPORT void setJavaClassLoaderForCurrentThread(JNIEnv *env, jobject class_loader);
  static EXPORT jclass findJavaClass(JNIEnv *env, const char *class_name);
  // TODO: Possibly wrong
  static EXPORT jclass findAndLoadJavaClass(JNIEnv *env, const char *class_name);

  static EXPORT void makeSignature(std::string &receiver, const char *return_type, ...);
  static EXPORT void makeSignature(std::string &receiver, const char *return_type, va_list arguments);
};

} // namespace jni
} // namespace spotify

#endif // __JavaClassUtils_h__
