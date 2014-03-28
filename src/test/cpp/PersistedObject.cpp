#include "PersistedObject.h"

PersistedObject::PersistedObject() : ClassWrapper(), i(0) {
}

PersistedObject::PersistedObject(JNIEnv *env) : ClassWrapper(env), i(0) {
  initialize(env);
}

void PersistedObject::initialize(JNIEnv *env) {
  setClass(env);
  cacheConstructor(env);
  cacheField(env, "i", kTypeInt);
  enablePersistence(env);
}

void PersistedObject::mapFields() {
  mapField("i", kTypeInt, &i);
}
