#ifndef __JavaExceptionUtils_h__
#define __JavaExceptionUtils_h__

#include "JniHelpersCommon.h"
#include "JniLocalRef.h"

namespace spotify {
namespace jni {

class JavaExceptionUtils {
private:
  // Direct instantiation not allowed
  JavaExceptionUtils() {}
  JavaExceptionUtils(const JavaExceptionUtils&) {}
  virtual ~JavaExceptionUtils() {}

public:
  static EXPORT JniLocalRef<jobject> newThrowable(JNIEnv *env, const char *message, ...);

  static EXPORT void checkException(JNIEnv *env);
  static EXPORT void throwException(JNIEnv *env, const char *message, ...);
  static EXPORT void throwRuntimeException(JNIEnv *env, const char *message, ...);
  static EXPORT void throwExceptionOfType(JNIEnv *env, const char *exception_class_name, const char *message, ...);

protected:
  static EXPORT void throwExceptionOfType(JNIEnv *env, const char *exception_class_name, const char *message, va_list arguments);
};
}
}

#endif // __JavaExceptionUtils_h__
