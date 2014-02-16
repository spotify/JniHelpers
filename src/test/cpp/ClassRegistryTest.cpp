#include "ClassRegistryTest.h"
#include "JUnitUtils.h"
#include "TestObject.h"

void ClassRegistryTest::initialize(JNIEnv *env) {
  setClass(env);

  addNativeMethod("createRegistry", &ClassRegistryTest::createRegistry, kTypeVoid, NULL);

  addNativeMethod("addClass", &ClassRegistryTest::addClass, kTypeVoid, NULL);
  addNativeMethod("addNullClass", &ClassRegistryTest::addNullClass, kTypeVoid, NULL);
  addNativeMethod("addClassWithEmptyName", &ClassRegistryTest::addClassWithEmptyName, kTypeVoid, NULL);
  addNativeMethod("addClassWithNullName", &ClassRegistryTest::addClassWithNullName, kTypeVoid, NULL);
  addNativeMethod("addClassWithoutInfo", &ClassRegistryTest::addClassWithoutInfo, kTypeVoid, NULL);
  addNativeMethod("addClassMultipleTimes", &ClassRegistryTest::addClassMultipleTimes, kTypeVoid, NULL);

  addNativeMethod("get", &ClassRegistryTest::get, kTypeVoid, NULL);
  addNativeMethod("getNullClass", &ClassRegistryTest::getNullClass, kTypeVoid, NULL);
  addNativeMethod("getInvalidClass", &ClassRegistryTest::getInvalidClass, kTypeVoid, NULL);

  TestObject testObject;
  addNativeMethod("nativeNewInstance", &ClassRegistryTest::nativeNewInstance, kTypeVoid, testObject.getCanonicalName(), NULL);
  addNativeMethod("nativeNewInstanceWithNull", &ClassRegistryTest::nativeNewInstanceWithNull, kTypeVoid, testObject.getCanonicalName(), NULL);

  registerNativeMethods(env);
}

void ClassRegistryTest::createRegistry(JNIEnv *env, jobject javaThis) {
  ClassRegistry registry;
  JUNIT_ASSERT_EQUALS_INT(0, registry.size());
}

void ClassRegistryTest::addClass(JNIEnv *env, jobject javaThis) {
  ClassRegistry registry;
  JUNIT_ASSERT_EQUALS_INT(0, registry.size());
  TestObject obj(env);
  registry.add(env, &obj);
  JUNIT_ASSERT_EQUALS_INT(1, registry.size());
}

void ClassRegistryTest::addNullClass(JNIEnv *env, jobject javaThis) {
  ClassRegistry registry;
  JUNIT_ASSERT_EQUALS_INT(0, registry.size());
  registry.add(env, NULL);
  JUNIT_ASSERT_EQUALS_INT(0, registry.size());
}

void ClassRegistryTest::addClassWithEmptyName(JNIEnv *env, jobject javaThis) {
  ClassRegistry registry;
  ClassWithName emptyName("");
  registry.add(env, &emptyName);
}

void ClassRegistryTest::addClassWithNullName(JNIEnv *env, jobject javaThis) {
  ClassRegistry registry;
  ClassWithName nullName(static_cast<const char*>(NULL));
  registry.add(env, &nullName);
}

void ClassRegistryTest::addClassWithoutInfo(JNIEnv *env, jobject javaThis) {
  ClassRegistry registry;
  ClassWithName nameButNoClassInfo("invalid");
  registry.add(env, &nameButNoClassInfo);
}

void ClassRegistryTest::addClassMultipleTimes(JNIEnv *env, jobject javaThis) {
  ClassRegistry registry;
  JUNIT_ASSERT_EQUALS_INT(0, registry.size());
  TestObject obj(env);
  registry.add(env, &obj);
  JUNIT_ASSERT_EQUALS_INT(1, registry.size());
  registry.add(env, &obj);
  // Should not be inserted multiple times
  JUNIT_ASSERT_EQUALS_INT(1, registry.size());
}

void ClassRegistryTest::get(JNIEnv *env, jobject javaThis) {
  ClassRegistry registry;
  TestObject obj(env);

  JUNIT_ASSERT_EQUALS_INT(0, registry.size());
  const TestObject *shouldBeNull = dynamic_cast<const TestObject*>(registry.get(obj.getCanonicalName()));
  JUNIT_ASSERT_NULL(shouldBeNull);

  registry.add(env, &obj);
  JUNIT_ASSERT_EQUALS_INT(1, registry.size());
  const TestObject *result = dynamic_cast<const TestObject*>(registry.get(obj.getCanonicalName()));
  JUNIT_ASSERT_NOT_NULL(result);
  // Sanity check just to make sure that the returned result matches our test object
  JUNIT_ASSERT_EQUALS_CSTRING(obj.getCanonicalName(), result->getCanonicalName());
}

void ClassRegistryTest::getNullClass(JNIEnv *env, jobject javaThis) {
  ClassRegistry registry;
  // Should throw
  JUNIT_ASSERT_NULL(registry.get(NULL));
}

void ClassRegistryTest::getInvalidClass(JNIEnv *env, jobject javaThis) {
  ClassRegistry registry;
  // Should throw
  JUNIT_ASSERT_NULL(registry.get("invalid"));
}

void ClassRegistryTest::nativeNewInstance(JNIEnv *env, jobject javaThis, jobject javaTestObject) {
  ClassRegistry registry;
  TestObject obj(env);
  registry.add(env, &obj);
  TestObject *result = registry.newInstance<TestObject>(env, javaTestObject);
  JUNIT_ASSERT_NOT_NULL(result);
  JUNIT_ASSERT_EQUALS_INT(1, result->i);
  std::string expected = "hello";
  JUNIT_ASSERT_EQUALS_STRING(expected, result->s.getValue());
  // TODO: Floats, bytes
}

void ClassRegistryTest::nativeNewInstanceWithNull(JNIEnv *env, jobject javaThis, jobject javaTestObject) {
}
