#ifndef __ClassRegistry_h__
#define __ClassRegistry_h__

#include "JniHelpersCommon.h"
#include "ClassWrapper.h"
#include "JavaExceptionUtils.h"
#include <map>
#include <string>
#include <string.h>

namespace spotify {
namespace jni {

#if WIN32
// TODO: Should only be exported for DLL builds
template class EXPORT std::map<const char*, ClassWrapper*>;
#endif

class EXPORT ClassRegistry {
public:
  ClassRegistry() {}
  virtual ~ClassRegistry() {}

  virtual void add(const ClassWrapper *item) {
    if (item == NULL) {
      JNIEnv *env = JavaThreadUtils::getEnvForCurrentThread();
      if (env != NULL) {
        JavaExceptionUtils::throwRuntimeException(env, "Can't add null item to map");
      }
      return;
    } else if (item->getCanonicalName() == NULL || strlen(item->getCanonicalName()) == 0) {
      JNIEnv *env = JavaThreadUtils::getEnvForCurrentThread();
      if (env != NULL) {
        JavaExceptionUtils::throwRuntimeException(env, "Can't item with empty canonical name to map");
      }
      return;
    } else {
    _classes[item->getCanonicalName()] = item;
    }
  }

  virtual const ClassWrapper* get(const char* name) {
    std::map<std::string, const ClassWrapper*>::iterator iter = _classes.find(name);
    return iter != _classes.end() ? _classes[name] : NULL;
  }

  // Only objects of type ClassWrapper may be instantiated here
  template<typename TypeName>
  TypeName* newInstance(JNIEnv *env, jobject fromObject) {
    TypeName *result = new TypeName();
    const char *name = result->getCanonicalName();
    if (name == NULL || strlen(name) == 0) {
      JavaExceptionUtils::throwRuntimeException(env, "Could not find canonical name for class");
      return NULL;
    }
    const TypeName *classInfo = dynamic_cast<const TypeName*>(get(name));
    if (classInfo == NULL) {
      JavaExceptionUtils::throwRuntimeException(env, "No class information registered for '%s'", name);
      return NULL;
    }
    result->merge(classInfo);
    result->setJavaObject(env, fromObject);
    return result;
  }

  virtual const size_t size() const {
    return _classes.size();
  }

protected:
  std::map<std::string, const ClassWrapper*> _classes;
};

} // namespace jni
} // namespace spotify

#endif // __ClassRegistry_h__
