#include "ClassWrapper.h"
#include "JavaClassUtils.h"
#include "JavaExceptionUtils.h"
#include <string.h>

namespace spotify {
namespace jni {

ClassWrapper::~ClassWrapper() {
  // TODO: Delete mappings
}

const char* ClassWrapper::getSimpleName() const {
  const char* lastSlash = strrchr(getCanonicalName(), '/');
  return lastSlash != NULL ? lastSlash + 1 : getCanonicalName();
}

void ClassWrapper::merge(const ClassWrapper *globalInstance) {
  _clazz = globalInstance->_clazz;
  _methods = globalInstance->_methods;
  _fields = globalInstance->_fields;
  _constructor = globalInstance->_constructor;
}

bool ClassWrapper::persist(JNIEnv *env, jobject javaThis) {
  if (isPersisted()) {
    jlong resultPtr = reinterpret_cast<jlong>(this);
    env->SetLongField(javaThis, getField(PERSIST_FIELD_NAME), resultPtr);
    return true;
  }
  return false;
}

bool ClassWrapper::isPersisted() const {
  return getField(PERSIST_FIELD_NAME) != NULL;
}

ClassWrapper* ClassWrapper::getPersistedInstance(JNIEnv *env, jobject javaThis) const {
  if (isPersisted()) {
    jlong resultPtr = env->GetLongField(javaThis, getField(PERSIST_FIELD_NAME));
    return reinterpret_cast<ClassWrapper*>(resultPtr);
  } else {
    return NULL;
  }
}


void ClassWrapper::reset(JNIEnv *env, jobject javaThis) {

}

void ClassWrapper::setJavaObject(JNIEnv *env, jobject javaThis) {
  FieldMap::iterator iter;
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

jobject ClassWrapper::toJavaObject(JNIEnv *env) {
  if (_constructor == NULL) {
    JavaExceptionUtils::throwExceptionOfType(env, kTypeIllegalStateException,
      "Cannot call toJavaObject without a constructor");
    return NULL;
  } else if (_clazz.get() == NULL) {
    JavaExceptionUtils::throwExceptionOfType(env, kTypeIllegalStateException,
      "Cannot call toJavaObject without registering class info");
    return NULL;
  }

  // Create a new Java argument with the default constructor
  // TODO: It would be nice to remove the requirement for a no-arg ctor
  // However, I'm not really sure how to do that without cluttering the interface.
  // Maybe provide an extra argument to setClass()? However, then we would lack
  // the corresponding arguments we'd want to pass in here.
  jobject result = env->NewObject(_clazz.get(), _constructor);

  FieldMap::iterator iter;
  for (iter = _fields.begin(); iter != _fields.end(); ++iter) {
    std::string key = iter->first;
    jfieldID field = iter->second;
    FieldMapping *mapping = getFieldMapping(key);
    if (field != NULL && mapping != NULL) {
      if (TYPE_EQUALS(mapping->type, kTypeInt)) {
        int *address = static_cast<int*>(mapping->address);
        env->SetIntField(result, field, *address);
      } else if (TYPE_EQUALS(mapping->type, kTypeFloat)) {
        float *address = static_cast<float*>(mapping->address);
        env->SetFloatField(result, field, *address);
      } else if (TYPE_EQUALS(mapping->type, kTypeString)) {
        JavaString *address = static_cast<JavaString*>(mapping->address);
        JniLocalRef<jstring> string = address->getJavaString(env);
        env->SetObjectField(result, field, string.get());
      } else {
        // TODO throw
      }
    }
  }

  // Persist the current object address to the Java instance
  persist(env, result);
  return result;
}

JniGlobalRef<jclass> ClassWrapper::getClass() const {
  return _clazz;
}

jmethodID ClassWrapper::getMethod(const char *method_name) const {
  const std::string key(method_name);
  MethodMap::const_iterator mapFindIter = _methods.find(key);
  return mapFindIter != _methods.end() ? mapFindIter->second : NULL;
}

jfieldID ClassWrapper::getField(const char* field_name) const {
  const std::string key(field_name);
  FieldMap::const_iterator mapFindIter = _fields.find(key);
  return mapFindIter != _fields.end() ? mapFindIter->second : NULL;
}

void ClassWrapper::setClass(JNIEnv *env) {
  _clazz.set(env->FindClass(getCanonicalName()));
  JavaExceptionUtils::checkException(env);
  std::string signature;
  JavaClassUtils::makeSignature(signature, kTypeVoid, NULL);
  _constructor = env->GetMethodID(_clazz.get(), "<init>", signature.c_str());
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

FieldMapping* ClassWrapper::getFieldMapping(const std::string &key) const {
  std::map<std::string, FieldMapping*>::const_iterator findMapIter = _field_mappings.find(key);
  return findMapIter != _field_mappings.end() ? findMapIter->second : NULL;
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
