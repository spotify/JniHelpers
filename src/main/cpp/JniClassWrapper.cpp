#include "JniClassWrapper.h"

namespace spotify {
namespace jni {

const char* JniClassWrapper::kTypeString = "Ljava/lang/String";

const char* JniClassWrapper::getCanonicalName() const  {
  std::stringstream stringstream;
  stringstream << getPackageName() << "/" << getSimpleName();
  return stringstream.str().c_str();
}

jmethodID JniClassWrapper::getMethod(const char *field_name) {
  return NULL;
}

jmethodID JniClassWrapper::getField(const char* field_name) {
  return NULL;
}

template<typename TypeName>
TypeName JniClassWrapper::getFieldValue(jobject instance, const char* field_name) {
  return NULL;
}

void JniClassWrapper::cacheMethod(const char* method_name, const char* return_type, ...) {

}

void JniClassWrapper::cacheField(const char* field_name) {

}

//template<typename FunctionPtr>
JNINativeMethod JniClassWrapper::makeNativeMethod(const char *method_name, void *function, const char return_type, ...) {
  return { 0 };
}

//template<typename FunctionPtr>
JNINativeMethod JniClassWrapper::makeNativeMethod(const char *method_name, void *function, const char *return_type, ...) {
  return { 0 };
}

bool JniClassWrapper::registerNativeMethods(JNIEnv *env, const std::string &class_name, const std::vector<JNINativeMethod> &methods) {
  return false;
}

} // namespace jni
} // namespace spotify
