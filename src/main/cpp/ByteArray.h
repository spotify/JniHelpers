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

  const void* getData() const { return _data; }
  JniLocalRef<jbyteArray> getJavaByteArray(JNIEnv *env) const;

  void *leak();

  //TODO add copyData functionality
  //void copyData(void *data, const size_t numBytes) {}
  //void copyData(JNIEnv *env, jbyteArray data);

  void setData(void *data, const size_t numBytes);
  void setData(JNIEnv *env, jbyteArray data);

  const size_t size() const { return _num_bytes; }

private:
  void *_data;
  size_t _num_bytes;
};

} // namespace jni
} // namespace spotify

#endif // __ByteArray_h__
