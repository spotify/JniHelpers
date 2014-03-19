#ifndef __JavaStringUtils_h__
#define __JavaStringUtils_h__

#include "JniHelpersCommon.h"
#include "JniLocalRef.h"
#include <string>

namespace spotify {
namespace jni {

#if WIN32
// TODO: This is a MSVC thing, should refactor to use PIMPL instead (ugh)
template class EXPORT std::basic_string<char, std::char_traits<char>, std::allocator<char>>;
#endif

class EXPORT JavaString {
public:
  JavaString();
  JavaString(const std::string &string);
  JavaString(JNIEnv *env, jstring javaString);
  virtual ~JavaString() {}

  const std::string& getValue() const;
  JniLocalRef<jstring> toJavaString(JNIEnv *env) const;

  void setValue(const char *value);
  void setValue(const std::string &value);
  void setValue(JNIEnv *env, jstring javaString);

public:
  std::string _value;
};

} // namespace jni
} // namespace spotify

#endif // __JavaStringUtils_h__
