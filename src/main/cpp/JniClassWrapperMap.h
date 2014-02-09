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
    _classes[item->getSimpleName()] = item;
  }

  virtual JniClassWrapper* get(const char* name) {
    return _classes[name];
  }

protected:
  std::map<const char*, JniClassWrapper*> _classes;
};

} // namespace jni
} // namespace spotify

#endif // __JniClassWrapperMap_h__
