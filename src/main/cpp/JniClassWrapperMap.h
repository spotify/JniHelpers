#ifndef __JniClassWrapperMap_h__
#define __JniClassWrapperMap_h__

#include "JniHelpersCommon.h"
#include "JniClassWrapper.h"
#include <string>
#include <map>

namespace spotify {
namespace jni {

class JniClassWrapperMap {
public:
  JniClassWrapperMap() {}
  virtual ~JniClassWrapperMap() {}

  virtual EXPORT void add(JniClassWrapper *item) {
    _classes[item->getSimpleName()] = item;
  }

protected:
  std::map<std::string, JniClassWrapper*> _classes;
};

} // namespace jni
} // namespace spotify

#endif // __JniClassWrapperMap_h__
