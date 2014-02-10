#ifndef __ClassRegistry_h__
#define __ClassRegistry_h__

#include "JniHelpersCommon.h"
#include "ClassWrapper.h"
#include <string>
#include <map>

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

  virtual void add(ClassWrapper *item) {
    _classes[item->getCanonicalName()] = item;
  }

  virtual ClassWrapper* get(const char* name) {
    return _classes[name];
  }

  // Only objects of type ClassWrapper may be instantiated here
  template<typename TypeName>
  TypeName* newInstance(JNIEnv *env, jobject fromObject) {
    TypeName *result = new TypeName();
    // TODO: Error checking here
    ClassWrapper *globalInstance = get(result->getCanonicalName());
    result->merge(globalInstance);
    result->setJavaObject(env, fromObject);
    return result;
  }

protected:
  std::map<const char*, ClassWrapper*> _classes;
};

} // namespace jni
} // namespace spotify

#endif // __ClassRegistry_h__
