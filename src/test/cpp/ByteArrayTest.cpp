#include "ByteArrayTest.h"
#include "JUnitUtils.h"
#include "ByteArray.h"

void ByteArrayTest::initialize(JNIEnv *env) {
  setClass(env);

  addNativeMethod("createNewByteArray", (void*)createNewByteArray, kTypeVoid, NULL);
  addNativeMethod("createNewByteArrayWithData", (void*)createNewByteArrayWithData, kTypeVoid, NULL);
  addNativeMethod("createNewByteArrayWithDataCopy", (void*)createNewByteArrayWithData, kTypeVoid, NULL);
  addNativeMethod("nativeCreateNewByteArrayWithJavaData", (void*)nativeCreateNewByteArrayWithJavaData, kTypeVoid, kTypeArray(kTypeByte), NULL);
  addNativeMethod("createNewByteArrayWithNull", (void*)createNewByteArrayWithNull, kTypeVoid, NULL);
  addNativeMethod("createNewByteArrayWithNullAndNonZeroLength", (void*)createNewByteArrayWithNullAndNonZeroLength, kTypeVoid, NULL);
  addNativeMethod("nativeGetTestJavaByteArray", (void*)nativeGetTestJavaByteArray, kTypeArray(kTypeByte), NULL);
  addNativeMethod("setData", (void*)setData, kTypeVoid, NULL);
  addNativeMethod("setDataWithCopy", (void*)setData, kTypeVoid, NULL);
  addNativeMethod("nativeSetJavaByteArray", (void*)nativeSetJavaByteArray, kTypeVoid, kTypeArray(kTypeByte), kTypeInt, NULL);

  registerNativeMethods(env);
}

void* ByteArrayTest::getTestData() {
  char *result = (char*)malloc(getTestDataSize());
  for (size_t i = 0; i < getTestDataSize(); i++) {
    result[i] = (char)i;
  }
  return result;
}

size_t ByteArrayTest::getTestDataSize() {
  return 10;
}

void ByteArrayTest::createNewByteArray(JNIEnv *env, jobject javaThis) {
  ByteArray byteArray;
  JUNIT_ASSERT_EQUALS_INT(0, byteArray.size());
  JUNIT_ASSERT_NULL(byteArray.get());
}

void ByteArrayTest::createNewByteArrayWithData(JNIEnv *env, jobject javaThis) {
  void *data = getTestData();
  ByteArray byteArray(data, getTestDataSize(), false);
  JUNIT_ASSERT_EQUALS_INT(getTestDataSize(), byteArray.size());
  JUNIT_ASSERT_EQUALS_ARRAY(data, byteArray.get(), byteArray.size());
}

void ByteArrayTest::createNewByteArrayWithDataCopy(JNIEnv *env, jobject javaThis) {
  void *data = getTestData();
  ByteArray byteArray(data, getTestDataSize(), true);
  free(data);
  void *expected = getTestData();
  JUNIT_ASSERT_EQUALS_INT(getTestDataSize(), byteArray.size());
  JUNIT_ASSERT_EQUALS_ARRAY(expected, byteArray.get(), byteArray.size());
}

void ByteArrayTest::nativeCreateNewByteArrayWithJavaData(JNIEnv *env, jobject javaThis, jbyteArray javaData) {
  void *data = getTestData();
  ByteArray byteArray(env, javaData);
  JUNIT_ASSERT_EQUALS_INT(getTestDataSize(), byteArray.size());
  JUNIT_ASSERT_EQUALS_ARRAY(data, byteArray.get(), byteArray.size());
}

void ByteArrayTest::createNewByteArrayWithNull(JNIEnv *env, jobject javaThis) {
  ByteArray byteArray(env, (jbyteArray)NULL);
  JUNIT_ASSERT_EQUALS_INT(0, byteArray.size());
  JUNIT_ASSERT_NULL(byteArray.get());
}

void ByteArrayTest::createNewByteArrayWithNullAndNonZeroLength(JNIEnv *env, jobject javaThis) {
  ByteArray byteArray(NULL, 1, false);
  JUNIT_ASSERT_EQUALS_INT(0, byteArray.size());
  JUNIT_ASSERT_NULL(byteArray.get());
}

jbyteArray ByteArrayTest::nativeGetTestJavaByteArray(JNIEnv *env, jobject javaThis) {
  void *data = getTestData();
  ByteArray byteArray(data, getTestDataSize(), true);
  JUNIT_ASSERT_EQUALS_INT(getTestDataSize(), byteArray.size());
  JUNIT_ASSERT_EQUALS_ARRAY(data, byteArray.get(), byteArray.size());
  JniLocalRef<jbyteArray> result = byteArray.toJavaByteArray(env);
  return result.leak();
}

void ByteArrayTest::setData(JNIEnv *env, jobject javaThis) {
  void *data = getTestData();
  ByteArray byteArray;
  byteArray.set(data, getTestDataSize(), false);
  JUNIT_ASSERT_EQUALS_INT(getTestDataSize(), byteArray.size());
  JUNIT_ASSERT_EQUALS_ARRAY(data, byteArray.get(), byteArray.size());
}

void ByteArrayTest::setDataWithCopy(JNIEnv *env, jobject javaThis) {
  void *data = getTestData();
  ByteArray byteArray;
  byteArray.set(data, getTestDataSize(), true);
  // Write 0's over the original data to make sure that a false positive
  // doesn't cause the test to pass.
  memcpy(data, 0, getTestDataSize());
  void *expectedData = getTestData();
  JUNIT_ASSERT_EQUALS_INT(getTestDataSize(), byteArray.size());
  JUNIT_ASSERT_EQUALS_ARRAY(expectedData, byteArray.get(), byteArray.size());
  free(expectedData);
}

void ByteArrayTest::nativeSetJavaByteArray(JNIEnv *env, jobject javaThis, jbyteArray javaData, jint expectedSize) {
  void *data = getTestData();
  ByteArray byteArray;
  byteArray.set(env, javaData);
  JUNIT_ASSERT_EQUALS_INT(expectedSize, byteArray.size());
  JUNIT_ASSERT_EQUALS_ARRAY(data, byteArray.get(), byteArray.size());
}
