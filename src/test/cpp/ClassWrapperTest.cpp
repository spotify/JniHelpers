#include "ClassWrapperTest.h"
#include "JUnitUtils.h"
#include "PersistedObject.h"
#include "TestObject.h"

void ClassWrapperTest::initialize(JNIEnv *env) {
  setClass(env);

  TestObject testObject;
  const char* testObjectName = testObject.getCanonicalName();
  PersistedObject persistedObject;
  const char* persistedObjectName = persistedObject.getCanonicalName();
  addNativeMethod("createClassWrapper", (void*)&ClassWrapperTest::createClassWrapper, kTypeVoid, NULL);
  addNativeMethod("getCanonicalName", (void*)&ClassWrapperTest::testGetCanonicalName, kTypeVoid, NULL);
  addNativeMethod("getSimpleName", (void*)&ClassWrapperTest::testGetSimpleName, kTypeVoid, NULL);
  addNativeMethod("merge", (void*)&ClassWrapperTest::testMerge, kTypeVoid, NULL);
  addNativeMethod("createPersistedObject", (void*)&ClassWrapperTest::createPersistedObject, persistedObjectName, NULL);
  addNativeMethod("getPersistedInstance", (void*)&ClassWrapperTest::getPersistedInstance, persistedObjectName, persistedObjectName, NULL);
  addNativeMethod("resetPersistedObject", (void*)&ClassWrapperTest::resetPersistedObject, kTypeVoid, persistedObjectName, NULL);
  addNativeMethod("nativePersistInvalidClass", (void*)&ClassWrapperTest::nativePersistInvalidClass, kTypeBool, testObjectName, NULL);
  addNativeMethod("persistNullObject", (void*)&ClassWrapperTest::persistNullObject, kTypeVoid, NULL);
  addNativeMethod("nativeResetPersistedObject", (void*)&ClassWrapperTest::nativeResetPersistedObject, kTypeVoid, persistedObjectName, NULL);
  addNativeMethod("nativeResetInvalidClass", (void*)&ClassWrapperTest::nativeResetInvalidClass, kTypeBool, testObjectName, NULL);
  addNativeMethod("resetNullObject", (void*)&ClassWrapperTest::resetNullObject, kTypeVoid, NULL);
  addNativeMethod("nativeSetJavaObject", (void*)&ClassWrapperTest::nativeSetJavaObject, kTypeVoid, testObjectName, NULL);
  addNativeMethod("nativeToJavaObject", (void*)&ClassWrapperTest::nativeToJavaObject, testObjectName, NULL);
  addNativeMethod("getCachedMethod", (void*)&ClassWrapperTest::getCachedMethod, kTypeVoid, NULL);
  addNativeMethod("getCachedField", (void*)&ClassWrapperTest::getCachedField, kTypeVoid, NULL);

  registerNativeMethods(env);
}

void ClassWrapperTest::createClassWrapper(JNIEnv *env, jobject javaThis) {
  // For objects created with the no-arg constructor, we would not expect them
  // to have class info, but they should always return a valid canonical name.
  TestObject emptyObject;
  JUNIT_ASSERT_NULL(emptyObject.getClass().get());
  JUNIT_ASSERT_NOT_NULL(emptyObject.getCanonicalName());
  JUNIT_ASSERT_NOT_NULL(emptyObject.getSimpleName());

  // When creating with a JNIEnv*, initialize() should be called and class info
  // will be populated.
  TestObject infoObject(env);
  JUNIT_ASSERT_NOT_NULL(infoObject.getClass().get());
  JUNIT_ASSERT_NOT_NULL(infoObject.getField("i"));
  JUNIT_ASSERT_NOT_NULL(infoObject.getMethod("getI"));
}

void ClassWrapperTest::testGetCanonicalName(JNIEnv *env, jobject javaThis) {
  // Should always be valid, even with no-arg ctor
  TestObject testObject;
  JUNIT_ASSERT_NULL(testObject.getClass().get());
  JUNIT_ASSERT_NOT_NULL(testObject.getCanonicalName());
  JUNIT_ASSERT_EQUALS_CSTRING("com/spotify/jni/TestObject", testObject.getCanonicalName());
}

void ClassWrapperTest::testGetSimpleName(JNIEnv *env, jobject javaThis) {
  // Should always be valid, even with no-arg ctor
  TestObject testObject;
  JUNIT_ASSERT_NULL(testObject.getClass().get());
  JUNIT_ASSERT_NOT_NULL(testObject.getSimpleName());
  JUNIT_ASSERT_EQUALS_CSTRING("TestObject", testObject.getSimpleName());
}

void ClassWrapperTest::testMerge(JNIEnv *env, jobject javaThis) {
  TestObject testObject(env);
  JUNIT_ASSERT_NOT_NULL(testObject.getClass().get());
  JUNIT_ASSERT_NOT_NULL(testObject.getField("i"));
  JUNIT_ASSERT_NOT_NULL(testObject.getMethod("getI"));

  TestObject mergeObject;
  JUNIT_ASSERT_NULL(mergeObject.getClass().get());
  mergeObject.merge(&testObject);
  JUNIT_ASSERT_NOT_NULL(mergeObject.getClass().get());
  JUNIT_ASSERT_NOT_NULL(mergeObject.getField("i"));
  JUNIT_ASSERT_NOT_NULL(mergeObject.getMethod("getI"));
}

JniLocalRef<jobject> ClassWrapperTest::createPersistedObject(JNIEnv *env, jobject javaThis) {
  PersistedObject persistedObjectInfo(env);
  PersistedObject *persistedObject = new PersistedObject();
  // Simulate merge to set up field mappings. This test case doesn't really reflect real-world
  // usage, since the ideal case would be to get an instance with ClassResolver::newInstance.
  persistedObject->merge(&persistedObjectInfo);
  persistedObject->i = 42;
  // Persist should be called for us here. Note that the original object is leaked; it will
  // be cleaned up in resetPersistedObject().
  jobject result = persistedObject->toJavaObject(env);
  return result;
}

JniLocalRef<jobject> ClassWrapperTest::getPersistedInstance(JNIEnv *env, jobject javaThis, jobject object) {
  PersistedObject persistedObjectInfo(env);
  ClassRegistry registry;
  registry.add(env, &persistedObjectInfo);
  PersistedObject *persistedObject = registry.newInstance<PersistedObject>(env, object);
  JUNIT_ASSERT_EQUALS_INT(42, persistedObject->i);
  JUNIT_ASSERT_NOT_NULL(persistedObject->getCanonicalName());
  JUNIT_ASSERT_NOT_NULL(persistedObject->getClass().get());
  return persistedObject->toJavaObject(env);
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
