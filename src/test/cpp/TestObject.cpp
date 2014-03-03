#include "TestObject.h"

TestObject::TestObject() : ClassWrapper(),
string(), i(0), s(0), f(0.0f), d(0.0) {
}

TestObject::TestObject(JNIEnv *env) : ClassWrapper(env),
string(), i(0), s(0), f(0.0f), d(0.0) {
  initialize(env);
  // Set up field mappings for the instance. Normally this isn't necessary,
  // since the global instance is typically used for merging into other objects.
  // If you want to actually use the initialized instance in combination with
  // setJavaObject(), you should call this though.
  merge(this);
}

void TestObject::initialize(JNIEnv *env) {
  setClass(env);

  cacheField(env, "string", kTypeString);
  cacheField(env, "i", kTypeInt);
  cacheField(env, "s", kTypeShort);
  cacheField(env, "f", kTypeFloat);
  cacheField(env, "d", kTypeDouble);
  // cacheField(env, "b", kTypeArray(kTypeByte));

  cacheMethod(env, "getString", kTypeString, NULL);
  cacheMethod(env, "setString", kTypeVoid, kTypeString, NULL);
  cacheMethod(env, "getI", kTypeInt, NULL);
  cacheMethod(env, "setI", kTypeVoid, kTypeInt, NULL);
  cacheMethod(env, "getS", kTypeShort, NULL);
  cacheMethod(env, "setS", kTypeVoid, kTypeShort, NULL);
  cacheMethod(env, "getF", kTypeFloat, NULL);
  cacheMethod(env, "setF", kTypeVoid, kTypeFloat, NULL);
  cacheMethod(env, "setD", kTypeVoid, kTypeDouble, NULL);
  // TODO: Getters/setters for byte array
}

void TestObject::mapFields() {
  mapField("i", kTypeInt, &i);
  mapField("s", kTypeShort, &s);
  mapField("f", kTypeFloat, &f);
  mapField("d", kTypeDouble, &d);
  mapField("string", kTypeString, &string);
}
