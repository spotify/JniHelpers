#ifndef __JniGlobalRef_h__
#define __JniGlobalRef_h__

#include "JavaThreadUtils.h"
#include "JniLocalRef.h"

namespace spotify {
namespace jni {

// RAII helper to maintain global references automatically.
template<typename JniType>
class JniGlobalRef {
 public:
  JniGlobalRef() : _obj(NULL) {}
  JniGlobalRef(const JniGlobalRef<JniType> &ref) : _obj(NULL) { set(ref.get()); }
  JniGlobalRef(const JniLocalRef<JniType> &ref) : _obj(NULL) { set(ref.get()); }

  ~JniGlobalRef() { set(NULL); }

  JniType get() const { return _obj; }

  void set(JniType obj) {
    JNIEnv *env = JavaThreadUtils::getEnvForCurrentThread();
    if (!env) {
        _obj = NULL;
        return;
    }
    if (_obj) {
      env->DeleteGlobalRef(_obj);
      _obj = NULL;
    }
    if (obj) {
      _obj = (JniType)env->NewGlobalRef(obj);
    }
  }

  operator JniType() const { return _obj; }

  void operator=(const JniLocalRef<JniType> &ref) { set(ref.get()); }

 private:
  JniType _obj;
};

} // namespace jni
} // namespace spotify

#endif // __JniGlobalRef_h__
