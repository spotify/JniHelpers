#ifndef __JavaStringUtils_h__
#define __JavaStringUtils_h__

#include "JniHelpersCommon.h"
#include "JniLocalRef.h"
#include <string>

namespace spotify {
namespace jni {

#if WIN32
// TODO: Should only be defined for dynamic lib builds
  template class EXPORT std::basic_string<char, std::char_traits<char>, std::allocator<char>>;
#endif

class EXPORT JavaString {
public:
  JavaString();
  JavaString(const std::string &string);
  JavaString(JNIEnv *env, jstring javaString);
  virtual ~JavaString() {}

  const std::string& getValue() const;
  JniLocalRef<jstring> getJavaString(JNIEnv *env) const;
  void setValue(JNIEnv *env, jstring javaString);

public:
  std::string _value;
};

} // namespace jni
} // namespace spotify

#endif // __JavaStringUtils_h__
