#ifndef __JniClassWrapperMap_h__
#define __JniClassWrapperMap_h__

#include "JniHelpersCommon.h"
#include "JniClassWrapper.h"
#include <string>
#include <map>

namespace spotify {
namespace jni {

template class EXPORT std::map<const char*, JniClassWrapper*>;

class EXPORT JniClassWrapperMap {
public:
  JniClassWrapperMap() {}
  virtual ~JniClassWrapperMap() {}

  virtual void add(JniClassWrapper *item) {
    _classes[item->getCanonicalName()] = item;
  }

  virtual JniClassWrapper* get(const char* name) {
    return _classes[name];
  }

  // Only objects of type JniClassWrapper may be instantiated here
  template<typename TypeName>
  TypeName* newInstance(JNIEnv *env, jobject fromObject) {
    TypeName *result = new TypeName();
    // TODO: Error checking here
    JniClassWrapper *globalInstance = get(result->getCanonicalName());
    result->merge(globalInstance);
    result->setJavaObject(env, fromObject);
    return result;
  }

protected:
  std::map<const char*, JniClassWrapper*> _classes;
};

} // namespace jni
} // namespace spotify

#endif // __JniClassWrapperMap_h__
