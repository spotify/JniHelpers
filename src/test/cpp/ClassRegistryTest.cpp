#include "ClassRegistryTest.h"
#include "JUnitUtils.h"
#include "TestObject.h"

void ClassRegistryTest::initialize(JNIEnv *env) {
  setClass(env);

  addNativeMethod("addClass", &ClassRegistryTest::addClass, kTypeVoid, NULL);
  addNativeMethod("addNullItem", &ClassRegistryTest::addNullItem, kTypeVoid, NULL);
  addNativeMethod("addItemWithEmptyName", &ClassRegistryTest::addItemWithEmptyName, kTypeVoid, NULL);
  addNativeMethod("addItemWithoutJavaClass", &ClassRegistryTest::addItemWithoutJavaClass, kTypeVoid, NULL);
  addNativeMethod("addItemMultipleTimes", &ClassRegistryTest::addItemMultipleTimes, kTypeVoid, NULL);

  registerNativeMethods(env);
}

void ClassRegistryTest::addClass(JNIEnv *env, jobject javaThis) {
  ClassRegistry registry;
  JUNIT_ASSERT_EQUALS_INT(0, registry.size());
  TestObject obj;
  registry.add(&obj);
  JUNIT_ASSERT_EQUALS_INT(1, registry.size());
  const TestObject *result = dynamic_cast<const TestObject*>(registry.get(obj.getCanonicalName()));
  JUNIT_ASSERT_NOT_NULL(result);
  // Sanity check just to make sure that the returned result matches our test object
  JUNIT_ASSERT_EQUALS_STRING(obj.getCanonicalName(), result->getCanonicalName());
}

void ClassRegistryTest::addNullItem(JNIEnv *env, jobject javaThis) {

}

void ClassRegistryTest::addItemWithEmptyName(JNIEnv *env, jobject javaThis) {

}

void ClassRegistryTest::addItemWithoutJavaClass(JNIEnv *env, jobject javaThis) {

}

void ClassRegistryTest::addItemMultipleTimes(JNIEnv *env, jobject javaThis) {

}
