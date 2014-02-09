#ifndef __JniException_h__
#define __JniException_h__

#include "JniHelpersCommon.h"
#include "JniLocalRef.h"

namespace spotify {
namespace jni {

  class JniException {
  private:
    // Direct instantiation not allowed
    JniException() {}
    JniException(const JniException&) {}
    virtual ~JniException() {}

  public:
    static EXPORT void checkException(JNIEnv *env);
    static EXPORT JniLocalRef<jobject> newException(JNIEnv *env, const char *message, ...);
    static EXPORT void throwRuntimeException(JNIEnv *env, const char *message, ...);
  };
}
}

#endif // __JniException_h__
