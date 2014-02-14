#include "JavaString.h"
#include "JavaExceptionUtils.h"

namespace spotify {
namespace jni {

JavaString::JavaString() {
  _value = "";
}

JavaString::JavaString(const std::string &string) {
  _value = string;
}

JavaString::JavaString(JNIEnv *env, jstring javaString) {
  setValue(env, javaString);
}

const std::string& JavaString::getValue() const {
  return _value;
}

JniLocalRef<jstring> JavaString::getJavaString(JNIEnv *env) const {
  return env->NewStringUTF(_value.c_str());
}

void JavaString::setValue(JNIEnv *env, jstring javaString) {
  if (javaString == NULL) {
    return;
  }

  const char *string = env->GetStringUTFChars(javaString, 0);
  JavaExceptionUtils::checkException(env);
  if (string == NULL) {
    return;
  }

  _value = string;
  env->ReleaseStringUTFChars(javaString, string);
  JavaExceptionUtils::checkException(env);
}


} // namespace jni
} // namespace spotify
