#ifndef __ByteArray_h__
#define __ByteArray_h__

#include "JniHelpersCommon.h"
#include "JniLocalRef.h"

namespace spotify {
namespace jni {

class EXPORT ByteArray {
public:
  ByteArray();
  ByteArray(void *data, const size_t numBytes);
  ByteArray(JNIEnv *env, jbyteArray data);
  virtual ~ByteArray();

  const void* get() const { return _data; }
  JniLocalRef<jbyteArray> toJavaByteArray(JNIEnv *env) const;

  void *leak();

  void copy(void *data, const size_t numBytes);
  void copy(JNIEnv *env, jbyteArray data);
  void set(void *data, const size_t numBytes);
  void set(JNIEnv *env, jbyteArray data);

  const size_t size() const { return _num_bytes; }

private:
  void *_data;
  size_t _num_bytes;
};

} // namespace jni
} // namespace spotify

#endif // __ByteArray_h__
