#include "ClassWrapperTest.h"
#include "TestObject.h"

void ClassWrapperTest::initialize(JNIEnv *env) {
  setClass(env);

  TestObject testObject;
  const char* testObjectName = testObject.getCanonicalName();
  // TODO: Replace with real class
  const char* persistedObjectName = "com/spotify/jni/PersistedObject";
  addNativeMethod("createClassWrapper", (void*)ClassWrapperTest::createClassWrapper, kTypeVoid, NULL);
  addNativeMethod("getCanonicalName", (void*)ClassWrapperTest::testGetCanonicalName, kTypeVoid, NULL);
  addNativeMethod("getSimpleName", (void*)ClassWrapperTest::testGetSimpleName, kTypeVoid, NULL);
  addNativeMethod("merge", (void*)ClassWrapperTest::testMerge, kTypeVoid, NULL);
  addNativeMethod("createPersistedObject", (void*)ClassWrapperTest::createPersistedObject, persistedObjectName, NULL);
  addNativeMethod("getPersistedInstance", (void*)ClassWrapperTest::getPersistedInstance, persistedObjectName, persistedObjectName, NULL);
  addNativeMethod("resetPersistedObject", (void*)ClassWrapperTest::resetPersistedObject, kTypeVoid, persistedObjectName, NULL);
  addNativeMethod("nativePersistInvalidClass", (void*)ClassWrapperTest::nativePersistInvalidClass, kTypeBool, testObjectName, NULL);
  addNativeMethod("persistNullObject", (void*)ClassWrapperTest::persistNullObject, kTypeVoid, NULL);
  addNativeMethod("nativeResetPersistedObject", (void*)ClassWrapperTest::nativeResetPersistedObject, kTypeVoid, persistedObjectName, NULL);
  addNativeMethod("nativeResetInvalidClass", (void*)ClassWrapperTest::nativeResetInvalidClass, kTypeBool, testObjectName, NULL);
  addNativeMethod("resetNullObject", (void*)ClassWrapperTest::resetNullObject, kTypeVoid, NULL);
  addNativeMethod("nativeSetJavaObject", (void*)ClassWrapperTest::nativeSetJavaObject, kTypeVoid, testObjectName, NULL);
  addNativeMethod("nativeToJavaObject", (void*)ClassWrapperTest::nativeToJavaObject, testObjectName, NULL);
  addNativeMethod("getCachedMethod", (void*)ClassWrapperTest::getCachedMethod, kTypeVoid, NULL);
  addNativeMethod("getCachedField", (void*)ClassWrapperTest::getCachedField, kTypeVoid, NULL);

  registerNativeMethods(env);
}

void ClassWrapperTest::createClassWrapper(JNIEnv *env, jobject javaThis) {
}

void ClassWrapperTest::testGetCanonicalName(JNIEnv *env, jobject javaThis) {
}

void ClassWrapperTest::testGetSimpleName(JNIEnv *env, jobject javaThis) {
}

void ClassWrapperTest::testMerge(JNIEnv *env, jobject javaThis) {
}

jobject ClassWrapperTest::createPersistedObject(JNIEnv *env, jobject javaThis) {
  return NULL;
}

jobject ClassWrapperTest::getPersistedInstance(JNIEnv *env, jobject javaThis, jobject object) {
  return NULL;
}

void ClassWrapperTest::resetPersistedObject(JNIEnv *env, jobject javaThis, jobject object) {
}

jboolean ClassWrapperTest::nativePersistInvalidClass(JNIEnv *env, jobject javaThis, jobject testObject) {
  return false;
}

void ClassWrapperTest::persistNullObject(JNIEnv *env, jobject javaThis) {
}

void ClassWrapperTest::nativeResetPersistedObject(JNIEnv *env, jobject javaThis, jobject object) {
}

jboolean ClassWrapperTest::nativeResetInvalidClass(JNIEnv *env, jobject javaThis, jobject testObject) {
  return false;
}

void ClassWrapperTest::resetNullObject(JNIEnv *env, jobject javaThis) {
}

void ClassWrapperTest::nativeSetJavaObject(JNIEnv *env, jobject javaThis, jobject object) {
}

jobject ClassWrapperTest::nativeToJavaObject(JNIEnv *env, jobject javaThis) {
  return NULL;
}

void ClassWrapperTest::getCachedMethod(JNIEnv *env, jobject javaThis) {
}

void ClassWrapperTest::getCachedField(JNIEnv *env, jobject javaThis) {
}
