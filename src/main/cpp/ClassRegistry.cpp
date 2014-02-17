#include "ClassRegistry.h"

namespace spotify {
namespace jni {

ClassRegistry::~ClassRegistry() {
  // TODO: Clear map, delete objects, etc.
}

void ClassRegistry::add(JNIEnv *env, const ClassWrapper *item) {
  if (item == NULL) {
    JavaExceptionUtils::throwExceptionOfType(env, kTypeIllegalArgumentException,
      "Can't add null item to registry");
    return;
  } else if (item->getCanonicalName() == NULL || strlen(item->getCanonicalName()) == 0) {
    JavaExceptionUtils::throwExceptionOfType(env, kTypeIllegalArgumentException,
      "Can't add item with empty canonical name to registry");
  } else if (item->getClass().get() == NULL) {
    JavaExceptionUtils::throwExceptionOfType(env, kTypeIllegalArgumentException,
      "Can't add uninitialized ClassWrapper to registry");
  } else {
    _classes[item->getCanonicalName()] = item;
  }
}

const ClassWrapper* ClassRegistry::get(const char* name) {
  if (name == NULL) {
    JNIEnv *env = JavaThreadUtils::getEnvForCurrentThread();
    JavaExceptionUtils::throwExceptionOfType(env, kTypeIllegalArgumentException,
      "Can't call get() with NULL");
    return NULL;
  }

  std::map<std::string, const ClassWrapper*>::iterator iter = _classes.find(name);
  return iter != _classes.end() ? _classes[name] : NULL;
}

} // namespace jni
} // namespace spotify
