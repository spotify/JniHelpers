#ifndef __JavaCharArray_h__
#define __JavaCharArray_h__

#include "JniHelpersCommon.h"
#include "JavaArray.h"

namespace spotify {
namespace jni {
namespace detail {

template<class JavaType, class NativeType>
class EXPORT JavaCharArray : public JavaArray<JavaType, NativeType> {
public:
  JniLocalRef<JavaType> toJavaArray(JNIEnv *env) const {
    JniLocalRef<jcharArray> result = env->NewCharArray((jsize)_num_elements);
    JavaExceptionUtils::checkException(env);
    if (_num_elements == 0 || _data == NULL) {
      return result;
    }
    env->SetCharArrayRegion(result, 0, (jsize)_num_elements, (jchar *)_data);
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
        _data = malloc(_num_elements * sizeof(jchar));
        env->GetCharArrayRegion(data, 0, (jsize)_num_elements, (jchar *)_data);
      }
    }
  }
};

} // namespace detail

typedef detail::JavaCharArray<jcharArray, unsigned short *> JavaCharArray;

} // namespace jni
} // namespace spotify

#endif // __JavaCharArray_h__