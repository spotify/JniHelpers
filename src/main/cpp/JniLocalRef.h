#ifndef __JniLocalRef_h__
#define __JniLocalRef_h__

namespace spotify {
namespace jni {

// RAII helper to maintain local references automatically
template<typename JniType>
class JniLocalRef {
public:
  JniLocalRef() : _obj(NULL) {}
  JniLocalRef(JniType obj) : _obj(NULL) { set(obj); }
  JniLocalRef(const JniLocalRef<JniType> &ref) : _obj(NULL) {
    JNIEnv *env = JniHelper::get()->getEnvForCurrentThread();
    set((JniType)env->NewLocalRef(ref.get()));
  }

  ~JniLocalRef() { set(NULL); }

  JniType get() const { return _obj; }
  void set(JniType obj) {
    JNIEnv *env = JniHelper::get()->getEnvForCurrentThread();
    if (_obj) env->DeleteLocalRef(_obj);
    _obj = obj;
  }

  JniType leak() {
    JniType obj = _obj;
    _obj = NULL;
    return obj;
  }

  operator JniType() const { return _obj; }

  void operator=(JniType obj) { set(obj); }
  void operator=(const JniLocalRef<JniType> &ref) {
    set((JniType)JniCurrentEnv()->NewLocalRef(ref.get()));
  }

private:
  JniType _obj;
};

} // namespace jni
} // namespace spotify

#endif // __JniLocalRef_h__
