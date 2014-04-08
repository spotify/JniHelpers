#include "PersistedObject.h"

PersistedObject::PersistedObject() : NativeObject(), i(0) {
}

PersistedObject::PersistedObject(JNIEnv *env) : NativeObject(env), i(0) {
  initialize(env);
}

void PersistedObject::initialize(JNIEnv *env) {
  setClass(env);
  cacheConstructor(env);
  cacheField(env, "i", kTypeInt);
}

void PersistedObject::mapFields() {
  mapField("i", kTypeInt, &i);
}
