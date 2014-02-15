#include "ClassWrapper.h"
#include "JavaClassUtils.h"
#include "JavaExceptionUtils.h"
#include <string.h>

namespace spotify {
namespace jni {

const char* ClassWrapper::getCanonicalName() const {
  return JavaClassUtils::makeCanonicalClassName(getPackageName(), getSimpleName());
}

void ClassWrapper::merge(const ClassWrapper *globalInstance) {
  _clazz = globalInstance->_clazz;
  _methods = globalInstance->_methods;
  _fields = globalInstance->_fields;
}

jmethodID ClassWrapper::getMethod(const char *method_name) {
  const std::string key(method_name);
  return _methods[key];
}

jfieldID ClassWrapper::getField(const char* field_name) {
  const std::string key(field_name);
  return _fields[key];
}

template<typename TypeName>
TypeName ClassWrapper::getFieldValue(JNIEnv *env, jobject instance, const char* field_name) {
  return NULL;
}

void ClassWrapper::setClass(JNIEnv *env) {
  _clazz = env->FindClass(getCanonicalName());
  JavaExceptionUtils::checkException(env);
}

void ClassWrapper::cacheMethod(JNIEnv *env, const char* method_name, const char* return_type, ...) {
  va_list arguments;
  va_start(arguments, return_type);
  const char *signature = JavaClassUtils::makeSignature(return_type, arguments);
  va_end(arguments);
  // TODO: Sanity check _class
  jmethodID method = env->GetMethodID(_clazz.get(), method_name, signature);
  JavaExceptionUtils::checkException(env);
  if (method != NULL) {
    _methods[method_name] = method;
  }
}

void ClassWrapper::cacheField(JNIEnv *env, const char *field_name, const char *field_type) {
  // TODO: Sanity check _class
  jfieldID field = env->GetFieldID(_clazz.get(), field_name, field_type);
  JavaExceptionUtils::checkException(env);
  if (field != NULL) {
    _fields[field_name] = field;
  }
}

void ClassWrapper::addNativeMethod(const char *method_name, void *function, const char *return_type, ...) {
  JNINativeMethod nativeMethod;
  nativeMethod.name = const_cast<char*>(method_name);
  nativeMethod.fnPtr = function;

  va_list arguments;
  va_start(arguments, return_type);
  nativeMethod.signature = const_cast<char*>(JavaClassUtils::makeSignature(return_type, arguments));
  va_end(arguments);

  _jni_methods.push_back(nativeMethod);
}

bool ClassWrapper::registerNativeMethods(JNIEnv *env) {
  if (_jni_methods.empty()) {
    return false;
  }

  if (_clazz.get() == NULL) {
    JavaExceptionUtils::throwRuntimeException(env, "Could not find cached class for %s", getCanonicalName());
    return false;
  }

  return (env->RegisterNatives(_clazz.get(), &_jni_methods[0], (jint)_jni_methods.size()) < 0);
}

} // namespace jni
} // namespace spotify
