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
    static EXPORT void checkException(JNIEnv *env);
    static EXPORT JniLocalRef<jobject> newException(JNIEnv *env, const char *message, ...);
    static EXPORT void throwRuntimeException(JNIEnv *env, const char *message, ...);
  };
}
}

#endif // __JavaExceptionUtils_h__
