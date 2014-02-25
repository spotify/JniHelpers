#ifndef __JavaClassUtils_h__
#define __JavaClassUtils_h__

#include "JniHelpersCommon.h"
#include <string>
#include <stdarg.h>

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
  // TODO: Not sure if this is the best interface here...
  static EXPORT jclass findClass(JNIEnv *env, const char *class_name, bool useClassLoader);

  /**
    * @brief Make a method signature from a variable list of arguments
    * @param receiver String to receive the generated signature
    * @param return_type Return type of function (see JniTypes.h)
    * @param ... List of arguments which function takes, ending with NULL. If the method
    *            takes no arguments, just pass NULL here. If you do not pass NULL as the
    *            last argument to this method, unexpected behavior will occur!
    */
  static EXPORT void makeSignature(std::string &receiver, const char *return_type, ...);
  static EXPORT void makeSignature(std::string &receiver, const char *return_type, va_list arguments);

protected:
  static EXPORT void appendTypeToSignature(std::stringstream &stringstream, const char *argument);
};

} // namespace jni
} // namespace spotify

#endif // __JavaClassUtils_h__
