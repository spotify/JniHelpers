#include "ShortArray.h"
#include "JavaExceptionUtils.h"
#include <stdlib.h>

namespace spotify {
namespace jni {

ShortArray::ShortArray() : _data(NULL), _num_elements(0) {}

ShortArray::ShortArray(short *data, const size_t numElements, bool copyData) :
_data(NULL), _num_elements(0) {
  // In the rare (but possible) event that this constructor is called with
  // NULL but non-zero length data, correct the byte count so as to avoid
  // segfaults later on.
  if (data == NULL && numElements > 0) {
    _num_elements = 0;
  } else if (data != NULL && numElements > 0) {
    set(data, numElements, copyData);
  }
}

ShortArray::ShortArray(JNIEnv *env, jshortArray data) :
_data(NULL), _num_elements(0) {
  set(env, data);
}

ShortArray::~ShortArray() {
  if (_data != NULL) {
    free(_data);
    _data = NULL;
  }
}

short *ShortArray::leak() {
  short *result = _data;
  _data = NULL;
  _num_elements = 0;
  return result;
}

JniLocalRef<jshortArray> ShortArray::toJavaShortArray(JNIEnv *env) const {
  JniLocalRef<jshortArray> result = env->NewShortArray((jsize)_num_elements);
  JavaExceptionUtils::checkException(env);
  if (_num_elements == 0 || _data == NULL) {
    return result;
  }
  env->SetShortArrayRegion(result, 0, (jsize)_num_elements, (jshort *)_data);
  return result.leak();
}

void ShortArray::set(short *data, const size_t numElements, bool copyData) {
  if (data == NULL && numElements > 0) {
    JNIEnv *env = JavaThreadUtils::getEnvForCurrentThread();
    JavaExceptionUtils::throwExceptionOfType(env, kTypeIllegalArgumentException,
      "Cannot set data with non-zero size and NULL object");
    return;
  }

  // Make sure not to leak the old data if it exists
  if (_data != NULL) {
    free(_data);
  }

  if (copyData) {
    _data = (short *)malloc(numElements);
    memcpy(_data, data, numElements);
  } else {
    _data = data;
  }
  _num_elements = numElements;
}

void ShortArray::set(JNIEnv *env, jshortArray data) {
  if (_data != NULL) {
    free(_data);
  }

  if (data != NULL) {
    _num_elements = env->GetArrayLength(data);
    if (_num_elements == 0) {
      _data = NULL;
    } else {
      _data = (short *)malloc(_num_elements);
      env->GetShortArrayRegion(data, 0, (jsize)_num_elements, (jshort *)_data);
    }
  }
}

} // namespace jni
} // namespace spotify
