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
  addNativeMethod("isInitialized", (void*)&ClassWrapperTest::nativeIsInitialized, kTypeVoid, NULL);
  addNativeMethod("getCanonicalName", (void*)&ClassWrapperTest::testGetCanonicalName, kTypeVoid, NULL);
  addNativeMethod("getSimpleName", (void*)&ClassWrapperTest::testGetSimpleName, kTypeVoid, NULL);
  addNativeMethod("merge", (void*)&ClassWrapperTest::testMerge, kTypeVoid, NULL);
  addNativeMethod("createPersistedObject", (void*)&ClassWrapperTest::createPersistedObject, persistedObjectName, NULL);
  addNativeMethod("getPersistedInstance", (void*)&ClassWrapperTest::getPersistedInstance, persistedObjectName, persistedObjectName, NULL);
  addNativeMethod("nativeIsPersistenceEnabled", nativeIsPersistenceEnabled, kTypeVoid, NULL);
  addNativeMethod("isPersistenceEnabledWithoutInit", isPersistenceEnabledWithoutInit, kTypeVoid, NULL);
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
  addNativeMethod("cacheInvalidMethod", (void*)&ClassWrapperTest::cacheInvalidMethod, kTypeVoid, NULL);

  registerNativeMethods(env);
}

void ClassWrapperTest::createClassWrapper(JNIEnv *env, jobject javaThis) {
  // For objects created with the no-arg constructor, we would not expect them
  // to have class info, but they should always return a valid canonical name.
  TestObject emptyObject;
  JUNIT_ASSERT_FALSE(emptyObject.isInitialized());
  JUNIT_ASSERT_NOT_NULL(emptyObject.getCanonicalName());
  JUNIT_ASSERT_NOT_NULL(emptyObject.getSimpleName());

  JUNIT_ASSERT_EQUALS_INT(0, emptyObject.i);
  JUNIT_ASSERT_EQUALS_INT(0, emptyObject.s);
  JUNIT_ASSERT_EQUALS_FLOAT(0.0f, emptyObject.f, DEFAULT_FLOAT_TOLERANCE);
  JUNIT_ASSERT_EQUALS_FLOAT(0.0, emptyObject.d, DEFAULT_FLOAT_TOLERANCE);
  JUNIT_ASSERT_EQUALS_STRING("", emptyObject.string.getValue());
  JUNIT_ASSERT_EQUALS_BOOL(false, emptyObject.z);
  JUNIT_ASSERT_EQUALS_INT(0, emptyObject.b);
  JUNIT_ASSERT_EQUALS_INT(0, emptyObject.c);

  // When creating with a JNIEnv*, initialize() should be called and class info
  // will be populated.
  TestObject infoObject(env);
  JUNIT_ASSERT_TRUE(infoObject.isInitialized());
  JUNIT_ASSERT_NOT_NULL(infoObject.getField("i"));
  JUNIT_ASSERT_NOT_NULL(infoObject.getMethod("getI"));
}

void ClassWrapperTest::nativeIsInitialized(JNIEnv *env, jobject javaThis) {
  LOG_INFO("Starting test: nativeIsInitialized");
  TestObject testObject;
  JUNIT_ASSERT_FALSE(testObject.isInitialized());
  testObject.initialize(env);
  JUNIT_ASSERT_TRUE(testObject.isInitialized());
}

void ClassWrapperTest::testGetCanonicalName(JNIEnv *env, jobject javaThis) {
  LOG_INFO("Starting test: testGetCanonicalName");
  // Should always be valid, even with no-arg ctor
  TestObject testObject;
  JUNIT_ASSERT_FALSE(testObject.isInitialized());
  JUNIT_ASSERT_NOT_NULL(testObject.getCanonicalName());
  JUNIT_ASSERT_EQUALS_CSTRING("com/spotify/jni/TestObject", testObject.getCanonicalName());
}

void ClassWrapperTest::testGetSimpleName(JNIEnv *env, jobject javaThis) {
  LOG_INFO("Starting test: testGetSimpleName");
  // Should always be valid, even with no-arg ctor
  TestObject testObject;
  JUNIT_ASSERT_FALSE(testObject.isInitialized());
  JUNIT_ASSERT_NOT_NULL(testObject.getSimpleName());
  JUNIT_ASSERT_EQUALS_CSTRING("TestObject", testObject.getSimpleName());
}

void ClassWrapperTest::testMerge(JNIEnv *env, jobject javaThis) {
  LOG_INFO("Starting test: testMerge");
  TestObject testObject(env);
  JUNIT_ASSERT_TRUE(testObject.isInitialized());
  JUNIT_ASSERT_NOT_NULL(testObject.getField("i"));
  JUNIT_ASSERT_NOT_NULL(testObject.getMethod("getI"));

  TestObject mergeObject;
  JUNIT_ASSERT_FALSE(mergeObject.isInitialized());
  mergeObject.merge(&testObject);
  JUNIT_ASSERT_TRUE(mergeObject.isInitialized());
  JUNIT_ASSERT_NOT_NULL(mergeObject.getField("i"));
  JUNIT_ASSERT_NOT_NULL(mergeObject.getMethod("getI"));
}

jobject ClassWrapperTest::createPersistedObject(JNIEnv *env, jobject javaThis) {
  LOG_INFO("Starting test: createPersistedObject");
  PersistedObject *persistedObject = new PersistedObject(env);
  persistedObject->i = TEST_INTEGER;
  // Persist should be called for us here. Note that the original object is leaked; it will
  // be cleaned up in destroyPersistedObject().
  return persistedObject->toJavaObject(env);
}

jobject ClassWrapperTest::getPersistedInstance(JNIEnv *env, jobject javaThis, jobject object) {
  LOG_INFO("Starting test: getPersistedInstance");
  ClassRegistry registry;
  registry.add(env, new PersistedObject(env));
  PersistedObject *persistedObject = registry.newInstance<PersistedObject>(env, object);
  JUNIT_ASSERT_EQUALS_INT(TEST_INTEGER, persistedObject->i);
  JUNIT_ASSERT_NOT_NULL(persistedObject->getCanonicalName());
  JUNIT_ASSERT_TRUE(persistedObject->isInitialized());
  return persistedObject->toJavaObject(env);
}

void ClassWrapperTest::nativeIsPersistenceEnabled(JNIEnv *env, jobject javaThis) {
  LOG_INFO("Starting test: nativeIsPersistenceEnabled");
  PersistedObject persistedObject(env);
  JUNIT_ASSERT_TRUE(persistedObject.isPersistenceEnabled());
  PersistedObject mergedObject;
  mergedObject.merge(&persistedObject);
  JUNIT_ASSERT_TRUE(mergedObject.isPersistenceEnabled());
  TestObject testObject(env);
  JUNIT_ASSERT_FALSE(testObject.isPersistenceEnabled());
}

void ClassWrapperTest::isPersistenceEnabledWithoutInit(JNIEnv *env, jobject javaThis) {
  PersistedObject persistedObject;
  JUNIT_ASSERT_FALSE(persistedObject.isPersistenceEnabled());
}

void ClassWrapperTest::destroyPersistedObject(JNIEnv *env, jobject javaThis, jobject object) {
  LOG_INFO("Starting test: destroyPersistedObject");
  ClassRegistry registry;
  registry.add(env, new PersistedObject(env));
  PersistedObject *persistedObject = registry.newInstance<PersistedObject>(env, object);
  persistedObject->destroy(env, object);
}

void ClassWrapperTest::persistInvalidClass(JNIEnv *env, jobject javaThis) {
  LOG_INFO("Starting test: persistInvalidClass");
  PersistedObject persistedObject(env);
  persistedObject.mapFields();
  persistedObject.persist(env, javaThis);
}

void ClassWrapperTest::persistNullObject(JNIEnv *env, jobject javaThis) {
  LOG_INFO("Starting test: persistNullObject");
  PersistedObject persistedObject(env);
  persistedObject.mapFields();
  JUNIT_ASSERT_FALSE(persistedObject.persist(env, NULL));
}

void ClassWrapperTest::destroyInvalidClass(JNIEnv *env, jobject javaThis) {
  // This test is almost impossible to replicate from Java, and frankly should
  // not happen from (responsible) C++ code either. It would be possible to catch
  // if we are willing to do fieldID lookups on the fly rather than cached, but
  // that assumes that performance is not an issue here. For that reason, this
  // test is excluded and the erroneous behavior will (and probably should) crash
  // the JVM if enabled.
#if 0
  LOG_INFO("Starting test: destroyInvalidClass");
  PersistedObject persistedObject(env);
  persistedObject.mapFields();
  persistedObject.destroy(env, javaThis);
#endif
}

void ClassWrapperTest::destroyNullObject(JNIEnv *env, jobject javaThis) {
  LOG_INFO("Starting test: destroyNullObject");
  PersistedObject persistedObject(env);
  persistedObject.mapFields();
  persistedObject.destroy(env, NULL);
}

void ClassWrapperTest::nativeSetJavaObject(JNIEnv *env, jobject javaThis, jobject object) {
  LOG_INFO("Starting test: nativeSetJavaObject");
  TestObject testObject(env);
  testObject.setJavaObject(env, object);
  JUNIT_ASSERT_EQUALS_STRING(TEST_STRING, testObject.string.getValue());
  JUNIT_ASSERT_EQUALS_INT(TEST_INTEGER, testObject.i);
  JUNIT_ASSERT_EQUALS_INT(TEST_SHORT, testObject.s);
  JUNIT_ASSERT_EQUALS_FLOAT(TEST_FLOAT, testObject.f, DEFAULT_FLOAT_TOLERANCE);
  JUNIT_ASSERT_EQUALS_FLOAT(TEST_DOUBLE, testObject.d, DEFAULT_FLOAT_TOLERANCE);
  JUNIT_ASSERT_EQUALS_BOOL(TEST_BOOLEAN, testObject.z);
  JUNIT_ASSERT_EQUALS_INT(TEST_BYTE, testObject.b);
#if HAS_RAW_STRING_LITERALS
  JUNIT_ASSERT_EQUALS_INT(TEST_CHAR, testObject.c);
#endif
}

jobject ClassWrapperTest::nativeToJavaObject(JNIEnv *env, jobject javaThis) {
  LOG_INFO("Starting test: nativeToJavaObject");
  TestObject testObject(env);
  testObject.string.setValue(TEST_STRING);
  testObject.i = TEST_INTEGER;
  testObject.s = TEST_SHORT;
  testObject.f = TEST_FLOAT;
  testObject.d = TEST_DOUBLE;
  testObject.z = TEST_BOOLEAN;
  testObject.b = TEST_BYTE;
#if HAS_RAW_STRING_LITERALS
  testObject.c = TEST_CHAR;
#endif
  return testObject.toJavaObject(env);
}

void ClassWrapperTest::getCachedMethod(JNIEnv *env, jobject javaThis) {
  LOG_INFO("Starting test: getCachedMethod");
  TestObject testObject(env);
  jmethodID method = testObject.getMethod("getString");
  JUNIT_ASSERT_NOT_NULL(method);
}

void ClassWrapperTest::getInvalidCachedMethod(JNIEnv *env, jobject javaThis) {
  LOG_INFO("Starting test: getInvalidCachedMethod");
  TestObject testObject(env);
  jmethodID method = testObject.getMethod("invalid");
  JUNIT_ASSERT_NULL(method);
}

void ClassWrapperTest::getCachedMethodOnUninitialized(JNIEnv *env, jobject javaThis) {
  LOG_INFO("Starting test: getCachedMethodOnUninitialized");
  TestObject testObject;
  jmethodID method = testObject.getMethod("getString");
  JUNIT_ASSERT_NULL(method);
}

void ClassWrapperTest::getCachedField(JNIEnv *env, jobject javaThis) {
  LOG_INFO("Starting test: getCachedField");
  TestObject testObject(env);
  jfieldID field = testObject.getField("string");
  JUNIT_ASSERT_NOT_NULL(field);
}

void ClassWrapperTest::getInvalidCachedField(JNIEnv *env, jobject javaThis) {
  LOG_INFO("Starting test: getInvalidCachedField");
  TestObject testObject(env);
  jfieldID field = testObject.getField("invalid");
  JUNIT_ASSERT_NULL(field);
}

void ClassWrapperTest::getCachedFieldOnUninitialized(JNIEnv *env, jobject javaThis) {
  LOG_INFO("Starting test: getCachedFieldOnUninitialized");
  TestObject testObject;
  jfieldID field = testObject.getField("string");
  JUNIT_ASSERT_NULL(field);
}

class CacheInvalidMethod : public TestObject {
public:
  CacheInvalidMethod() : TestObject() {}
  virtual void initialize(JNIEnv *env) {
    TestObject::initialize(env);
    cacheMethod(env, "invalid", kTypeVoid, NULL);
  }
};

void ClassWrapperTest::cacheInvalidMethod(JNIEnv *env, jobject javaThis) {
  LOG_INFO("Starting test: cacheInvalidMethod");
  CacheInvalidMethod testObject;
  testObject.initialize(env);
}
