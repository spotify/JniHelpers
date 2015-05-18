#ifndef __JavaByteArray_h__
#define __JavaByteArray_h__

#include "JniHelpersCommon.h"
#include "JavaArray.h"

namespace spotify {
namespace jni {
namespace detail {

template<class JavaType, class NativeType>
class EXPORT JavaByteArray : public JavaArray<JavaType, NativeType> {
public:
  JniLocalRef<JavaType> toJavaArray(JNIEnv *env) const {
    JniLocalRef<jbyteArray> result = env->NewByteArray((jsize)_num_elements);
    JavaExceptionUtils::checkException(env);
    if (_num_elements == 0 || _data == NULL) {
      return result;
    }
    env->SetByteArrayRegion(result, 0, (jsize)_num_elements, (jbyte *)_data);
    return result.leak();
  }

  void set(JNIEnv *env, JavaType data) {
    if (_data != NULL) {
      free(_data);
    }

    if (data != NULL) {
      _num_elements = env->GetArrayLength(data);
      if (_num_elements == 0) {
        _data = NULL;
      } else {
        _data = malloc(_num_elements * sizeof(jbyte));
        env->GetByteArrayRegion(data, 0, (jsize)_num_elements, (jbyte *)_data);
      }
    }
  }
};

} // namespace detail

typedef detail::JavaByteArray<jbyteArray, void *> JavaByteArray;

} // namespace jni
} // namespace spotify

#endif // __JavaByteArray_h__