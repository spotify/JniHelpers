#include "ClassRegistry.h"

namespace spotify {
namespace jni {

ClassRegistry::ClassRegistry() {
  LOG_DEBUG("Creating new class registry");
}

ClassRegistry::~ClassRegistry() {
  LOG_DEBUG("Destroying class registry");
}

void ClassRegistry::add(JNIEnv *env, const ClassWrapper *item) {
  if (item == NULL) {
    JavaExceptionUtils::throwExceptionOfType(env, kTypeIllegalArgumentException,
      "Can't add null item to registry");
    return;
  } else if (item->getCanonicalName() == NULL || strlen(item->getCanonicalName()) == 0) {
    JavaExceptionUtils::throwExceptionOfType(env, kTypeIllegalArgumentException,
      "Can't add item with empty canonical name to registry");
  } else if (!item->isInitialized()) {
    JavaExceptionUtils::throwExceptionOfType(env, kTypeIllegalArgumentException,
      "Can't add uninitialized ClassWrapper to registry");
  } else {
    LOG_INFO("Adding class instance '%s' to registry", item->getCanonicalName());
    _classes[item->getCanonicalName()].reset(item);
  }
}

const ClassWrapper* ClassRegistry::get(const char* name) {
  if (name == NULL) {
    JNIEnv *env = JavaThreadUtils::getEnvForCurrentThread();
    JavaExceptionUtils::throwExceptionOfType(env, kTypeIllegalArgumentException,
      "Can't call get() with NULL");
    return NULL;
  }

  ClassRegistryMap::iterator iter = _classes.find(name);
  return iter != _classes.end() ? _classes[name] : NULL;
}

} // namespace jni
} // namespace spotify
