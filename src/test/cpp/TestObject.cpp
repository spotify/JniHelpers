#include "TestObject.h"

TestObject::TestObject() : ClassWrapper(),
s(), i(0), f(0.0f) {
}

TestObject::TestObject(JNIEnv *env) : ClassWrapper(env),
s(), i(0), f(0.0f) {
  initialize(env);
}

void TestObject::initialize(JNIEnv *env) {
  setClass(env);

  cacheField(env, "s", kTypeJavaString);
  cacheField(env, "i", kTypeInt);
  cacheField(env, "f", kTypeFloat);
  // cacheField(env, "b", kTypeArray(kTypeByte));

  cacheMethod(env, "getS", kTypeJavaString, NULL);
  cacheMethod(env, "setS", kTypeVoid, kTypeJavaString, NULL);
  cacheMethod(env, "getI", kTypeInt, NULL);
  cacheMethod(env, "setI", kTypeVoid, kTypeInt, NULL);
  cacheMethod(env, "getF", kTypeFloat, NULL);
  cacheMethod(env, "setF", kTypeVoid, kTypeFloat, NULL);
  // TODO: Getters/setters for byte array
}

void TestObject::merge(const ClassWrapper *globalInstance) {
  ClassWrapper::merge(globalInstance);
  mapField("i", kTypeInt, &i);
  mapField("f", kTypeFloat, &f);
  mapField("s", kTypeJavaString, &s);
}
