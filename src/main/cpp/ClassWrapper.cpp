#include "ClassWrapper.h"
#include "JavaClassUtils.h"
#include "JavaExceptionUtils.h"
#include <string.h>

namespace spotify {
namespace jni {

const char* ClassWrapper::getSimpleName() const {
  const char* lastSlash = strrchr(getCanonicalName(), '/');
  return lastSlash != NULL ? lastSlash + 1 : getCanonicalName();
}

void ClassWrapper::merge(const ClassWrapper *globalInstance) {
  _clazz = globalInstance->_clazz;
  _methods = globalInstance->_methods;
  _fields = globalInstance->_fields;
}

void ClassWrapper::setJavaObject(JNIEnv *env, jobject javaThis) {
  setFieldsFrom(env, javaThis);
}

jobject ClassWrapper::toJavaObject(JNIEnv *env) {
  jobject result = env->NewObject(_clazz, _constructor);
  setFieldsTo(env, result);
  return result;
}

JniGlobalRef<jclass> ClassWrapper::getClass() const {
  return _clazz;
}

jmethodID ClassWrapper::getMethod(const char *method_name) {
  const std::string key(method_name);
  return _methods[key];
}

jfieldID ClassWrapper::getField(const char* field_name) {
  const std::string key(field_name);
  return _fields[key];
}

void ClassWrapper::setClass(JNIEnv *env) {
  _clazz = env->FindClass(getCanonicalName());
  JavaExceptionUtils::checkException(env);
  std::string signature;
  JavaClassUtils::makeSignature(signature, kTypeVoid);
  _constructor = env->GetMethodID(_clazz, "<init>", signature.c_str());
}

void ClassWrapper::cacheMethod(JNIEnv *env, const char* method_name, const char* return_type, ...) {
  va_list arguments;
  va_start(arguments, return_type);
  std::string signature;
  JavaClassUtils::makeSignature(signature, return_type, arguments);
  va_end(arguments);
  if (_clazz.get() == NULL) {
    JavaExceptionUtils::throwExceptionOfType(env, kTypeIllegalStateException,
      "Attempt to call cacheMethod without having set class info");
    return;
  }

  jmethodID method = env->GetMethodID(_clazz.get(), method_name, signature.c_str());
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

void ClassWrapper::mapField(const char *field_name, const char *field_type, void *field_ptr) {
  FieldMapping *mapping = new FieldMapping;
  mapping->type = field_type;
  mapping->address = field_ptr;
  _field_mappings[field_name] = mapping;
}

void ClassWrapper::setFieldsFrom(JNIEnv *env, jobject javaThis) {
  std::map<std::string, jfieldID>::iterator iter;
  for (iter = _fields.begin(); iter != _fields.end(); ++iter) {
    std::string key = iter->first;
    jfieldID field = iter->second;
    FieldMapping *mapping = _field_mappings[key]; // TODO: Should use getter here, if not null will be inserted
    if (field != NULL && mapping != NULL) {
      if (TYPE_EQUALS(mapping->type, kTypeInt)) {
        int *address = static_cast<int*>(mapping->address);
        *address = env->GetIntField(javaThis, field);
      } else if (TYPE_EQUALS(mapping->type, kTypeFloat)) {
        float *address = static_cast<float*>(mapping->address);
        *address = env->GetFloatField(javaThis, field);
      } else if (TYPE_EQUALS(mapping->type, kTypeString)) {
        jstring string = (jstring)env->GetObjectField(javaThis, field);
        JavaString *address = static_cast<JavaString*>(mapping->address);
        address->setValue(env, string);
      } else {
        // TODO throw
      }
    }
  }
}

void ClassWrapper::setFieldsTo(JNIEnv *env, jobject javaThis) {
  std::map<std::string, jfieldID>::iterator iter;
  for (iter = _fields.begin(); iter != _fields.end(); ++iter) {
    std::string key = iter->first;
    jfieldID field = iter->second;
    FieldMapping *mapping = _field_mappings[key]; // TODO: Should use getter here, if not null will be inserted
    if (field != NULL && mapping != NULL) {
      if (TYPE_EQUALS(mapping->type, kTypeInt)) {
        int *address = static_cast<int*>(mapping->address);
        env->SetIntField(javaThis, field, *address);
      } else if (TYPE_EQUALS(mapping->type, kTypeFloat)) {
        float *address = static_cast<float*>(mapping->address);
        env->SetFloatField(javaThis, field, *address);
      } else if (TYPE_EQUALS(mapping->type, kTypeString)) {
        JavaString *address = static_cast<JavaString*>(mapping->address);
        JniLocalRef<jstring> string = address->getJavaString(env);
        env->SetObjectField(javaThis, field, string.get());
      } else {
        // TODO throw
      }
    }
  }
}

void ClassWrapper::addNativeMethod(const char *method_name, void *function, const char *return_type, ...) {
  JNINativeMethod nativeMethod;
  nativeMethod.name = const_cast<char*>(method_name);
  nativeMethod.fnPtr = function;

  va_list arguments;
  va_start(arguments, return_type);
  std::string signature;
  JavaClassUtils::makeSignature(signature, return_type, arguments);
  nativeMethod.signature = const_cast<char*>(strdup(signature.c_str()));
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
