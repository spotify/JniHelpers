#include "ClassWrapper.h"
#include "JavaClassUtils.h"
#include "JavaExceptionUtils.h"
#include "JniWeakGlobalRef.h"
#include <string.h>

namespace spotify {
namespace jni {

ClassWrapper::~ClassWrapper() {
  // TODO: Delete mappings
}

bool ClassWrapper::isInitialized() const {
  return _clazz.get() != NULL;
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
    if (javaThis == NULL) {
      JavaExceptionUtils::throwExceptionOfType(env, kTypeIllegalArgumentException,
        "Cannot persist object without corresponding Java instance");
      return false;
    }
    jlong resultPtr = reinterpret_cast<jlong>(this);
    env->SetLongField(javaThis, getField(PERSIST_FIELD_NAME), resultPtr);
    return true;
  }
  return false;
}

bool ClassWrapper::isPersisted() const {
  // TODO: Need test for this
  if (!isInitialized()) {
    JavaExceptionUtils::throwExceptionOfType(JavaThreadUtils::getEnvForCurrentThread(),
      kTypeIllegalStateException,
      "Cannot call isPersisted without class info (forgot to merge?)");
    return false;
  }

  // We expect the persisted field to be cached, otherwise searching for persisted
  // fields in non-persisted classes will throw java.lang.NoSuchField exception. :(
  const std::string key(PERSIST_FIELD_NAME);
  FieldMap::const_iterator mapFindIter = _fields.find(key);
  return mapFindIter != _fields.end();
}

ClassWrapper* ClassWrapper::getPersistedInstance(JNIEnv *env, jobject javaThis) const {
  if (isPersisted()) {
    jlong resultPtr = env->GetLongField(javaThis, getField(PERSIST_FIELD_NAME));
    return reinterpret_cast<ClassWrapper*>(resultPtr);
  } else {
    return NULL;
  }
}

void ClassWrapper::destroy(JNIEnv *env, jobject javaThis) {
  if (isPersisted()) {
    if (javaThis == NULL) {
      JavaExceptionUtils::throwExceptionOfType(env, kTypeIllegalArgumentException,
        "Cannot destroy persisted object without corresponding Java instance");
      return;
    }

    jfieldID persistField = getField(PERSIST_FIELD_NAME);
    if (persistField == NULL) {
      JavaExceptionUtils::throwExceptionOfType(env, kTypeIllegalStateException,
        "Cannot destroy, object lacks persist field");
      return;
    }

    jlong resultPtr = env->GetLongField(javaThis, persistField);
    ClassWrapper *instance = reinterpret_cast<ClassWrapper*>(resultPtr);
    if (instance != NULL) {
      delete instance;
      env->SetLongField(javaThis, persistField, 0);
    }
  }
}

void ClassWrapper::setJavaObject(JNIEnv *env, jobject javaThis) {
  // Set up field mappings, if this has not already been done
  if (_field_mappings.empty()) {
    mapFields();
  }

  FieldMap::iterator iter;
  for (iter = _fields.begin(); iter != _fields.end(); ++iter) {
    std::string key = iter->first;
    jfieldID field = iter->second;
    FieldMapping *mapping = _field_mappings[key]; // TODO: Should use getter here, if not null will be inserted
    if (field != NULL && mapping != NULL) {
      if (TYPE_EQUALS(mapping->type, kTypeInt)) {
        int *address = static_cast<int*>(mapping->address);
        *address = env->GetIntField(javaThis, field);
      } else if (TYPE_EQUALS(mapping->type, kTypeShort)) {
        short *address = static_cast<short*>(mapping->address);
        *address = env->GetShortField(javaThis, field);
      } else if (TYPE_EQUALS(mapping->type, kTypeBool)) {
        bool *address = static_cast<bool*>(mapping->address);
        *address = env->GetBooleanField(javaThis, field);
      } else if (TYPE_EQUALS(mapping->type, kTypeFloat)) {
        float *address = static_cast<float*>(mapping->address);
        *address = env->GetFloatField(javaThis, field);
      } else if (TYPE_EQUALS(mapping->type, kTypeDouble)) {
        double *address = static_cast<double*>(mapping->address);
        *address = env->GetDoubleField(javaThis, field);
      } else if (TYPE_EQUALS(mapping->type, kTypeString)) {
        jstring string = (jstring)env->GetObjectField(javaThis, field);
        JavaString *address = static_cast<JavaString*>(mapping->address);
        address->setValue(env, string);
      } else if (TYPE_EQUALS(mapping->type, kTypeByte)) {
        unsigned char *address = static_cast<unsigned char*>(mapping->address);
        *address = env->GetByteField(javaThis, field);
      } else if (TYPE_EQUALS(mapping->type, kTypeChar)) {
        wchar_t *address = static_cast<wchar_t*>(mapping->address);
        *address = env->GetCharField(javaThis, field);
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
  } else if (!isInitialized()) {
    JavaExceptionUtils::throwExceptionOfType(env, kTypeIllegalStateException,
      "Cannot call toJavaObject without registering class info");
    return NULL;
  }

  // Set up field mappings, if this has not already been done
  if (_field_mappings.empty()) {
    mapFields();
  }

  // Create a new Java argument with the default constructor
  // TODO: It would be nice to remove the requirement for a no-arg ctor
  // However, I'm not really sure how to do that without cluttering the interface.
  // Maybe provide an extra argument to setClass()? However, then we would lack
  // the corresponding arguments we'd want to pass in here.
  JniLocalRef<jobject> result;
  result.set(env->NewObject(_clazz.get(), _constructor));

  FieldMap::iterator iter;
  for (iter = _fields.begin(); iter != _fields.end(); ++iter) {
    std::string key = iter->first;
    jfieldID field = iter->second;
    FieldMapping *mapping = getFieldMapping(key.c_str());
    if (field != NULL && mapping != NULL) {
      if (TYPE_EQUALS(mapping->type, kTypeInt)) {
        int *address = static_cast<int*>(mapping->address);
        env->SetIntField(result, field, *address);
      } else if (TYPE_EQUALS(mapping->type, kTypeShort)) {
        short *address = static_cast<short*>(mapping->address);
        env->SetShortField(result, field, *address);
      } else if (TYPE_EQUALS(mapping->type, kTypeBool)) {
        bool *address = static_cast<bool*>(mapping->address);
        env->SetBooleanField(result, field, *address);
      } else if (TYPE_EQUALS(mapping->type, kTypeFloat)) {
        float *address = static_cast<float*>(mapping->address);
        env->SetFloatField(result, field, *address);
      } else if (TYPE_EQUALS(mapping->type, kTypeDouble)) {
        double *address = static_cast<double*>(mapping->address);
        env->SetDoubleField(result, field, *address);
      } else if (TYPE_EQUALS(mapping->type, kTypeString)) {
        JavaString *address = static_cast<JavaString*>(mapping->address);
        JniLocalRef<jstring> string = address->getJavaString(env);
        env->SetObjectField(result, field, string.get());
      } else if (TYPE_EQUALS(mapping->type, kTypeByte)) {
        unsigned char *address = static_cast<unsigned char*>(mapping->address);
        env->SetByteField(result, field, *address);
      } else if (TYPE_EQUALS(mapping->type, kTypeChar)) {
        wchar_t *address = static_cast<wchar_t*>(mapping->address);
        env->SetCharField(result, field, *address);
      } else {
        // TODO throw
      }
    }
  }

  // Persist the current object address to the Java instance
  persist(env, result);
  return result.leak();
}

JniGlobalRef<jclass> ClassWrapper::getClass() const {
  return _clazz;
}

jmethodID ClassWrapper::getMethod(const char *method_name) const {
  if (!isInitialized()) {
    JavaExceptionUtils::throwExceptionOfType(JavaThreadUtils::getEnvForCurrentThread(),
      kTypeIllegalStateException,
      "Cannot call getMethod without class info (forgot to merge?)");
    return NULL;
  }

  const std::string key(method_name);
  MethodMap::const_iterator mapFindIter = _methods.find(key);
  if (mapFindIter == _methods.end()) {
    JavaExceptionUtils::throwExceptionOfType(JavaThreadUtils::getEnvForCurrentThread(),
      kTypeIllegalArgumentException,
      "Method '%s' is not cached in class '%s'", method_name, getCanonicalName());
    return NULL;
  }

  return mapFindIter->second;
}

jfieldID ClassWrapper::getField(const char* field_name) const {
  if (!isInitialized()) {
    JavaExceptionUtils::throwExceptionOfType(JavaThreadUtils::getEnvForCurrentThread(),
      kTypeIllegalStateException,
      "Cannot call getField without class info (forgot to merge?)");
    return NULL;
  }

  const std::string key(field_name);
  FieldMap::const_iterator mapFindIter = _fields.find(key);
  if (mapFindIter == _fields.end()) {
    JavaExceptionUtils::throwExceptionOfType(JavaThreadUtils::getEnvForCurrentThread(),
      kTypeIllegalArgumentException,
      "Field '%s' is not cached in class '%s'", field_name, getCanonicalName());
    return NULL;
  }

  return mapFindIter->second;
}

void ClassWrapper::setClass(JNIEnv *env) {
  _clazz.set(env->FindClass(getCanonicalName()));
  JavaExceptionUtils::checkException(env);
  std::string signature;
  JavaClassUtils::makeSignature(signature, kTypeVoid, NULL);
  _constructor = env->GetMethodID(_clazz.get(), "<init>", signature.c_str());
}

void ClassWrapper::cacheMethod(JNIEnv *env, const char* method_name, const char* return_type, ...) {
  if (!isInitialized()) {
    JavaExceptionUtils::throwExceptionOfType(env, kTypeIllegalStateException,
      "Attempt to call cacheMethod without having set class info");
    return;
  }

  va_list arguments;
  va_start(arguments, return_type);
  std::string signature;
  JavaClassUtils::makeSignatureWithList(signature, return_type, arguments);
  va_end(arguments);

  jmethodID method = env->GetMethodID(_clazz.get(), method_name, signature.c_str());
  JavaExceptionUtils::checkException(env);
  if (method != NULL) {
    _methods[method_name] = method;
  }
}

void ClassWrapper::cacheField(JNIEnv *env, const char *field_name, const char *field_type) {
  if (!isInitialized()) {
    JavaExceptionUtils::throwExceptionOfType(env, kTypeIllegalStateException,
      "Attempt to call cacheField without having set class info");
    return;
  }

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

FieldMapping* ClassWrapper::getFieldMapping(const char *key) const {
  std::string keyString(key);
  std::map<std::string, FieldMapping*>::const_iterator findMapIter = _field_mappings.find(keyString);
  return findMapIter != _field_mappings.end() ? findMapIter->second : NULL;
}

void ClassWrapper::addNativeMethod(const char *method_name, void *function, const char *return_type, ...) {
  JNINativeMethod nativeMethod;
  nativeMethod.name = const_cast<char*>(method_name);
  nativeMethod.fnPtr = function;

  va_list arguments;
  va_start(arguments, return_type);
  std::string signature;
  JavaClassUtils::makeSignatureWithList(signature, return_type, arguments);
  nativeMethod.signature = const_cast<char*>(strdup(signature.c_str()));
  va_end(arguments);

  _jni_methods.push_back(nativeMethod);
}

bool ClassWrapper::registerNativeMethods(JNIEnv *env) {
  if (_jni_methods.empty()) {
    return false;
  }

  if (!isInitialized()) {
    JavaExceptionUtils::throwRuntimeException(env, "Could not find cached class for %s", getCanonicalName());
    return false;
  }

  return (env->RegisterNatives(_clazz.get(), &_jni_methods[0], (jint)_jni_methods.size()) < 0);
}

} // namespace jni
} // namespace spotify
