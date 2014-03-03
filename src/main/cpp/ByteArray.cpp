#include "ByteArray.h"
#include "JavaExceptionUtils.h"
#include <stdlib.h>

namespace spotify {
namespace jni {

ByteArray::ByteArray() : _data(NULL), _num_bytes(0) {}

ByteArray::ByteArray(void *data, const size_t numBytes) :
_data(data), _num_bytes(numBytes) {
  // In the rare (but possible) event that this constructor is called with
  // NULL but non-zero length data, correct the byte count so as to avoid
  // segfaults later on.
  if (_data == NULL && _num_bytes > 0) {
    _num_bytes = 0;
  }
}

ByteArray::ByteArray(JNIEnv *env, jbyteArray data) :
_data(NULL), _num_bytes(0) {
  setData(env, data);
}

ByteArray::~ByteArray() {
  if (_data != NULL) {
    free(_data);
  }
}

JniLocalRef<jbyteArray> ByteArray::getJavaByteArray(JNIEnv *env) const {
  JniLocalRef<jbyteArray> result = env->NewByteArray((jsize)_num_bytes);
  JavaExceptionUtils::checkException(env);
  if (_num_bytes == 0 || _data == NULL) {
    return result;
  }
  env->SetByteArrayRegion(result, 0, (jsize)_num_bytes, (jbyte *)_data);
  return result.leak();
}

void ByteArray::setData(void *data, const size_t numBytes) {
  if (data == NULL && numBytes > 0) {
    JNIEnv *env = JavaThreadUtils::getEnvForCurrentThread();
    JavaExceptionUtils::throwExceptionOfType(env, kTypeIllegalArgumentException, 
      "Cannot set data with non-zero size and NULL object");
    return;
  }

  // Make sure not to leak the old data if it exists
  if (_data != NULL) {
    free(_data);
  }

  _data = data;
  _num_bytes = numBytes;
}

void ByteArray::setData(JNIEnv *env, jbyteArray data) {
  if (_data != NULL) {
    free(_data);
  }

  if (data != NULL) {
    _num_bytes = env->GetArrayLength(data);
    if (_num_bytes == 0) {
      _data = NULL;
    } else {
      _data = malloc(_num_bytes);
      env->GetByteArrayRegion(data, 0, (jsize)_num_bytes, (jbyte *)_data);
    }
  }
}

} // namespace jni
} // namespace spotify
