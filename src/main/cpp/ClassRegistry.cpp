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

template<typename TypeName>
TypeName* ClassRegistry::newInstance(JNIEnv *env, jobject fromObject) {
  TypeName *result = new TypeName();
  const char *name = result->getCanonicalName();
  if (name == NULL || strlen(name) == 0) {
    JavaExceptionUtils::throwExceptionOfType(env, kTypeIllegalArgumentException,
      "Could not find canonical name for class");
    return NULL;
  }
  const TypeName *classInfo = dynamic_cast<const TypeName*>(get(name));
  if (classInfo == NULL) {
    JavaExceptionUtils::throwExceptionOfType(env, kTypeIllegalStateException,
      "No class information registered for '%s'", name);
    return NULL;
  }
  result->merge(classInfo);
  result->setJavaObject(env, fromObject);
  return result;
}


} // namespace jni
} // namespace spotify
