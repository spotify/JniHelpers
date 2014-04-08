#ifndef __ByteArrayTest_h__
#define __ByteArrayTest_h__

#include "JniHelpers.h"
#include "JniHelpersTest.h"

using namespace spotify::jni;

class ByteArrayTest : public JavaClass {
public:
  ByteArrayTest() : JavaClass() {}
  ByteArrayTest(JNIEnv *env) : JavaClass(env) { initialize(env); }
  ~ByteArrayTest() {}

  const char* getCanonicalName() const {
    return MAKE_CANONICAL_NAME(PACKAGE, ByteArrayTest);
  }
  void initialize(JNIEnv *env);
  void mapFields() {}

private:
  static void* getTestData();
  static size_t getTestDataSize();

  static void createNewByteArray(JNIEnv *env, jobject javaThis);
  static void createNewByteArrayWithData(JNIEnv *env, jobject javaThis);
  static void createNewByteArrayWithDataCopy(JNIEnv *env, jobject javaThis);
  static void nativeCreateNewByteArrayWithJavaData(JNIEnv *env, jobject javaThis, jbyteArray javaData);
  static void createNewByteArrayWithNull(JNIEnv *env, jobject javaThis);
  static void createNewByteArrayWithNullAndNonZeroLength(JNIEnv *env, jobject javaThis);
  static jbyteArray nativeGetTestJavaByteArray(JNIEnv *env, jobject javaThis);
  static void setData(JNIEnv *env, jobject javaThis);
  static void setDataWithCopy(JNIEnv *env, jobject javaThis);
  static void nativeSetJavaByteArray(JNIEnv *env, jobject javaThis, jbyteArray javaData, jint expectedSize);
};

#endif // __ByteArrayTest_h__
