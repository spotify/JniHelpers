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
  addNativeMethod("destroyPersistedObject", (void*)&ClassWrapperTest::destroyPersistedObject, kTypeVoid, persistedObjectName, NULL);
  addNativeMethod("persistInvalidClass", (void*)&ClassWrapperTest::persistInvalidClass, kTypeVoid, NULL);
  addNativeMethod("persistNullObject", (void*)&ClassWrapperTest::persistNullObject, kTypeVoid, NULL);
  addNativeMethod("destroyInvalidClass", (void*)&ClassWrapperTest::destroyInvalidClass, kTypeVoid, NULL);
  addNativeMethod("destroyNullObject", (void*)&ClassWrapperTest::destroyNullObject, kTypeVoid, NULL);
  addNativeMethod("nativeSetJavaObject", (void*)&ClassWrapperTest::nativeSetJavaObject, kTypeVoid, testObjectName, NULL);
  addNativeMethod("nativeToJavaObject", (void*)&ClassWrapperTest::nativeToJavaObject, testObjectName, NULL);
  addNativeMethod("getCachedMethod", (void*)&ClassWrapperTest::getCachedMethod, kTypeVoid, NULL);
  addNativeMethod("getInvalidCachedMethod", (void*)&ClassWrapperTest::getInvalidCachedMethod, kTypeVoid, NULL);
  addNativeMethod("getCachedMethodOnUninitialized", (void*)&ClassWrapperTest::getCachedMethodOnUninitialized, kTypeVoid, NULL);
  addNativeMethod("getCachedField", (void*)&ClassWrapperTest::getCachedField, kTypeVoid, NULL);
  addNativeMethod("getInvalidCachedField", (void*)&ClassWrapperTest::getInvalidCachedField, kTypeVoid, NULL);
  addNativeMethod("getCachedFieldOnUninitialized", (void*)&ClassWrapperTest::getCachedFieldOnUninitialized, kTypeVoid, NULL);

  registerNativeMethods(env);
}

void ClassWrapperTest::createClassWrapper(JNIEnv *env, jobject javaThis) {
  // For objects created with the no-arg constructor, we would not expect them
  // to have class info, but they should always return a valid canonical name.
  TestObject emptyObject;
  JUNIT_ASSERT_NULL(emptyObject.getClass().get());
  JUNIT_ASSERT_NOT_NULL(emptyObject.getCanonicalName());
  JUNIT_ASSERT_NOT_NULL(emptyObject.getSimpleName());

  JUNIT_ASSERT_EQUALS_INT(0, emptyObject.i);
  JUNIT_ASSERT_EQUALS_INT(0, emptyObject.s);
  JUNIT_ASSERT_EQUALS_FLOAT(0.0f, emptyObject.f, DEFAULT_FLOAT_TOLERANCE);
  JUNIT_ASSERT_EQUALS_FLOAT(0.0, emptyObject.d, DEFAULT_FLOAT_TOLERANCE);
  JUNIT_ASSERT_EQUALS_STRING("", emptyObject.string.getValue());
  JUNIT_ASSERT_EQUALS_BOOL(false, emptyObject.z);

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

jobject ClassWrapperTest::createPersistedObject(JNIEnv *env, jobject javaThis) {
  PersistedObject *persistedObject = new PersistedObject(env);
  persistedObject->i = TEST_INTEGER;
  // Persist should be called for us here. Note that the original object is leaked; it will
  // be cleaned up in destroyPersistedObject().
  return persistedObject->toJavaObject(env);
}

jobject ClassWrapperTest::getPersistedInstance(JNIEnv *env, jobject javaThis, jobject object) {
  PersistedObject persistedObjectInfo(env);
  ClassRegistry registry;
  registry.add(env, &persistedObjectInfo);
  PersistedObject *persistedObject = registry.newInstance<PersistedObject>(env, object);
  JUNIT_ASSERT_EQUALS_INT(TEST_INTEGER, persistedObject->i);
  JUNIT_ASSERT_NOT_NULL(persistedObject->getCanonicalName());
  JUNIT_ASSERT_NOT_NULL(persistedObject->getClass().get());
  return persistedObject->toJavaObject(env);
}

void ClassWrapperTest::destroyPersistedObject(JNIEnv *env, jobject javaThis, jobject object) {
  PersistedObject persistedObjectInfo(env);
  ClassRegistry registry;
  registry.add(env, &persistedObjectInfo);
  PersistedObject *persistedObject = registry.newInstance<PersistedObject>(env, object);
  persistedObject->destroy(env, object);
}

void ClassWrapperTest::persistInvalidClass(JNIEnv *env, jobject javaThis) {
  PersistedObject persistedObject(env);
  persistedObject.mapFields();
  persistedObject.persist(env, javaThis);
}

void ClassWrapperTest::persistNullObject(JNIEnv *env, jobject javaThis) {
  PersistedObject persistedObject(env);
  persistedObject.mapFields();
  JUNIT_ASSERT_FALSE(persistedObject.persist(env, NULL));
}

void ClassWrapperTest::destroyInvalidClass(JNIEnv *env, jobject javaThis) {
#if 0
  // This test is almost impossible to replicate from Java, and frankly should
  // not happen from (responsible) C++ code either. It would be possible to catch
  // if we are willing to do fieldID lookups on the fly rather than cached, but
  // that assumes that performance is not an issue here. For that reason, this
  // test is excluded and the erroneous behavior will (and probably should) crash
  // the JVM if enabled.
  PersistedObject persistedObject(env);
  persistedObject.mapFields();
  persistedObject.destroy(env, javaThis);
#endif
}

void ClassWrapperTest::destroyNullObject(JNIEnv *env, jobject javaThis) {
  PersistedObject persistedObject(env);
  persistedObject.mapFields();
  persistedObject.destroy(env, NULL);
}

void ClassWrapperTest::nativeSetJavaObject(JNIEnv *env, jobject javaThis, jobject object) {
  TestObject testObject(env);
  testObject.setJavaObject(env, object);
  JUNIT_ASSERT_EQUALS_STRING(TEST_STRING, testObject.string.getValue());
  JUNIT_ASSERT_EQUALS_INT(TEST_INTEGER, testObject.i);
  JUNIT_ASSERT_EQUALS_INT(TEST_SHORT, testObject.s);
  JUNIT_ASSERT_EQUALS_FLOAT(TEST_FLOAT, testObject.f, DEFAULT_FLOAT_TOLERANCE);
  JUNIT_ASSERT_EQUALS_FLOAT(TEST_DOUBLE, testObject.d, DEFAULT_FLOAT_TOLERANCE);
  JUNIT_ASSERT_EQUALS_BOOL(TEST_BOOLEAN, testObject.z);
}

jobject ClassWrapperTest::nativeToJavaObject(JNIEnv *env, jobject javaThis) {
  TestObject testObject(env);
  testObject.string.setValue(TEST_STRING);
  testObject.i = TEST_INTEGER;
  testObject.s = TEST_SHORT;
  testObject.f = TEST_FLOAT;
  testObject.d = TEST_DOUBLE;
  testObject.z = TEST_BOOLEAN;
  return testObject.toJavaObject(env);
}

void ClassWrapperTest::getCachedMethod(JNIEnv *env, jobject javaThis) {
  TestObject testObject(env);
  jmethodID method = testObject.getMethod("getString");
  JUNIT_ASSERT_NOT_NULL(method);
}

void ClassWrapperTest::getInvalidCachedMethod(JNIEnv *env, jobject javaThis) {
  TestObject testObject(env);
  jmethodID method = testObject.getMethod("invalid");
  JUNIT_ASSERT_NULL(method);
}

void ClassWrapperTest::getCachedMethodOnUninitialized(JNIEnv *env, jobject javaThis) {
  TestObject testObject;
  jmethodID method = testObject.getMethod("getString");
  JUNIT_ASSERT_NULL(method);
}

void ClassWrapperTest::getCachedField(JNIEnv *env, jobject javaThis) {
  TestObject testObject(env);
  jfieldID field = testObject.getField("string");
  JUNIT_ASSERT_NOT_NULL(field);
}

void ClassWrapperTest::getInvalidCachedField(JNIEnv *env, jobject javaThis) {
  TestObject testObject(env);
  jfieldID field = testObject.getField("invalid");
  JUNIT_ASSERT_NULL(field);
}

void ClassWrapperTest::getCachedFieldOnUninitialized(JNIEnv *env, jobject javaThis) {
  TestObject testObject;
  jfieldID field = testObject.getField("string");
  JUNIT_ASSERT_NULL(field);
}
