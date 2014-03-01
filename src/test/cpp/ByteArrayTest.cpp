#include "ByteArrayTest.h"
#include "JUnitUtils.h"
#include "ByteArray.h"

void ByteArrayTest::initialize(JNIEnv *env) {
  setClass(env);

  addNativeMethod("createNewByteArray", (void*)&ByteArrayTest::createNewByteArray, kTypeVoid, NULL);
  addNativeMethod("createNewByteArrayWithData", (void*)&ByteArrayTest::createNewByteArrayWithData, kTypeVoid, NULL);
  addNativeMethod("nativeCreateNewByteArrayWithJavaData", (void*)&ByteArrayTest::nativeCreateNewByteArrayWithJavaData, kTypeVoid, kTypeArray(kTypeByte), NULL);
  addNativeMethod("createNewByteArrayWithNull", (void*)&ByteArrayTest::createNewByteArrayWithNull, kTypeVoid, NULL);
  addNativeMethod("createNewByteArrayWithNullAndNonZeroLength", (void*)&ByteArrayTest::createNewByteArrayWithNullAndNonZeroLength, kTypeVoid, NULL);
  addNativeMethod("nativeGetTestJavaByteArray", (void*)&ByteArrayTest::nativeGetTestJavaByteArray, kTypeArray(kTypeByte), NULL);
  addNativeMethod("setData", (void*)&ByteArrayTest::setData, kTypeVoid, NULL);
  addNativeMethod("nativeSetJavaByteArray", (void*)&ByteArrayTest::nativeSetJavaByteArray, kTypeVoid, kTypeArray(kTypeByte), kTypeInt, NULL);

  registerNativeMethods(env);
}

void* ByteArrayTest::getTestData() {
  char *result = (char*)malloc(getTestDataSize());
  for (int i = 0; i < getTestDataSize(); i++) {
    result[i] = i;
  }
  return result;
}

size_t ByteArrayTest::getTestDataSize() {
  return 10;
}

void ByteArrayTest::createNewByteArray(JNIEnv *env, jobject javaThis) {
  ByteArray byteArray;
  JUNIT_ASSERT_EQUALS_INT(0, byteArray.size());
  JUNIT_ASSERT_NULL(byteArray.getData());
}

void ByteArrayTest::createNewByteArrayWithData(JNIEnv *env, jobject javaThis) {
  void *data = getTestData();
  ByteArray byteArray(data, getTestDataSize());
  JUNIT_ASSERT_EQUALS_INT(getTestDataSize(), byteArray.size());
  JUNIT_ASSERT_EQUALS_ARRAY(data, byteArray.getData(), byteArray.size());
}

void ByteArrayTest::nativeCreateNewByteArrayWithJavaData(JNIEnv *env, jobject javaThis, jbyteArray javaData) {
  void *data = getTestData();
  ByteArray byteArray(env, javaData);
  JUNIT_ASSERT_EQUALS_INT(getTestDataSize(), byteArray.size());
  JUNIT_ASSERT_EQUALS_ARRAY(data, byteArray.getData(), byteArray.size());
}

void ByteArrayTest::createNewByteArrayWithNull(JNIEnv *env, jobject javaThis) {
  ByteArray byteArray(env, NULL);
  JUNIT_ASSERT_EQUALS_INT(0, byteArray.size());
  JUNIT_ASSERT_NULL(byteArray.getData());
}

void ByteArrayTest::createNewByteArrayWithNullAndNonZeroLength(JNIEnv *env, jobject javaThis) {
  ByteArray byteArray(NULL, 1);
  JUNIT_ASSERT_EQUALS_INT(0, byteArray.size());
  JUNIT_ASSERT_NULL(byteArray.getData());
}

jbyteArray ByteArrayTest::nativeGetTestJavaByteArray(JNIEnv *env, jobject javaThis) {
  void *data = getTestData();
  ByteArray byteArray(data, getTestDataSize());
  JUNIT_ASSERT_EQUALS_INT(getTestDataSize(), byteArray.size());
  JUNIT_ASSERT_EQUALS_ARRAY(data, byteArray.getData(), byteArray.size());
  JniLocalRef<jbyteArray> result = byteArray.getJavaByteArray(env);
  return result.leak();
}

void ByteArrayTest::setData(JNIEnv *env, jobject javaThis) {
  void *data = getTestData();
  ByteArray byteArray;
  byteArray.setData(data, getTestDataSize());
  JUNIT_ASSERT_EQUALS_INT(getTestDataSize(), byteArray.size());
  JUNIT_ASSERT_EQUALS_ARRAY(data, byteArray.getData(), byteArray.size());
}

void ByteArrayTest::nativeSetJavaByteArray(JNIEnv *env, jobject javaThis, jbyteArray javaData, jint expectedSize) {
  void *data = getTestData();
  ByteArray byteArray;
  byteArray.setData(env, javaData);
  JUNIT_ASSERT_EQUALS_INT(expectedSize, byteArray.size());
  JUNIT_ASSERT_EQUALS_ARRAY(data, byteArray.getData(), byteArray.size());
}
