#include "ClassWrapper.h"
#include "JavaClassUtils.h"
#include "JavaExceptionUtils.h"
#include <string.h>

namespace spotify {
namespace jni {

ClassWrapper::ClassWrapper() :
_clazz_global(NULL), _clazz(NULL),
_methods(NULL), _fields(NULL),
_constructor(NULL) {
  LOG_DEBUG("Creating new empty instance of class");
}

ClassWrapper::ClassWrapper(JNIEnv *env) :
_clazz_global(NULL), _clazz(NULL),
_methods(NULL), _fields(NULL),
_constructor(NULL) {
  // Ideally, we would like to call initialize() from the ClassWrapper() ctor.
  // However this won't work because initialize() is pure virtual, and such methods
  // cannot be called here because the object is in an incomplete state. So instead,
  // one needs to make sure that initialize(env) is called in the subclass' ctor.
  LOG_DEBUG("Initializing new instance of class");
  _methods = &_methods_global;
  _fields = &_fields_global;
}

ClassWrapper::~ClassWrapper() {
  LOG_DEBUG("Destroying instance of class");
}

bool ClassWrapper::isInitialized() const {
  return _clazz != NULL;
}

const char* ClassWrapper::getSimpleName() const {
  const char* lastSlash = strrchr(getCanonicalName(), '/');
  return lastSlash != NULL ? lastSlash + 1 : getCanonicalName();
}

void ClassWrapper::merge(const ClassWrapper *globalInstance) {
  LOG_DEBUG("Merging instance of '%s' with global class info", getSimpleName());
  _clazz = globalInstance->_clazz_global.get();
  _methods = &globalInstance->_methods_global;
  _fields = &globalInstance->_fields_global;
  _constructor = globalInstance->_constructor;
}

bool ClassWrapper::persist(JNIEnv *env, jobject javaThis) {
  if (isPersistenceEnabled()) {
    LOG_DEBUG("Persisting instance of '%s' to Java object", getSimpleName());
    if (javaThis == NULL) {
      JavaExceptionUtils::throwExceptionOfType(env, kTypeIllegalArgumentException,
        "Cannot persist object without corresponding Java instance");
      return false;
    }
    jlong resultPtr = reinterpret_cast<jlong>(this);
    env->SetLongField(javaThis, getField(PERSIST_FIELD_NAME), resultPtr);
    JavaExceptionUtils::checkException(env);
    LOG_DEBUG("Persist was successful");
    return true;
  }
  return false;
}

bool ClassWrapper::isPersistenceEnabled() const {
  // TODO: Need test for this
  if (!isInitialized()) {
    JavaExceptionUtils::throwExceptionOfType(JavaThreadUtils::getEnvForCurrentThread(),
      kTypeIllegalStateException,
      "Cannot call isPersistenceEnabled without class info (forgot to merge?)");
    return false;
  }

  // We expect the persisted field to be cached, otherwise searching for persisted
  // fields in non-persisted classes will throw java.lang.NoSuchField exception. :(
  const std::string key(PERSIST_FIELD_NAME);
  FieldMap::const_iterator mapFindIter = _fields->find(key);
  return mapFindIter != _fields->end();
}

ClassWrapper* ClassWrapper::getPersistedInstance(JNIEnv *env, jobject javaThis) const {
  if (isPersistenceEnabled()) {
    LOG_DEBUG("Retrieving persisted instance of '%s'", getSimpleName());
    jlong resultPtr = env->GetLongField(javaThis, getField(PERSIST_FIELD_NAME));
    return reinterpret_cast<ClassWrapper*>(resultPtr);
  } else {
    return NULL;
  }
}

void ClassWrapper::destroy(JNIEnv *env, jobject javaThis) {
  if (isPersistenceEnabled()) {
    LOG_DEBUG("Destroying persisted instance of '%s'", getSimpleName());
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
  LOG_DEBUG("Setting fields from Java object of type '%s' to native instance", getSimpleName());

  // Set up field mappings, if this has not already been done
  if (_field_mappings.empty()) {
    mapFields();
  }

   for (FieldMap::const_iterator iter = _fields->begin(); iter != _fields->end(); ++iter) {
    std::string key = iter->first;
    LOG_DEBUG("Copying field '%s'", key.c_str());

    jfieldID field = iter->second;
    const FieldMapping *mapping = getFieldMapping(key.c_str());
    if (field != NULL && mapping != NULL) {
      if (TYPE_EQUALS(mapping->type, kTypeInt)) {
        int *address = static_cast<int*>(mapping->address);
        *address = env->GetIntField(javaThis, field);
      } else if (TYPE_EQUALS(mapping->type, kTypeShort)) {
        short *address = static_cast<short*>(mapping->address);
        *address = env->GetShortField(javaThis, field);
      } else if (TYPE_EQUALS(mapping->type, kTypeBool)) {
        bool *address = static_cast<bool*>(mapping->address);
        // The somewhat odd "? true : false" fixes a performance warning on windows,
        // since GetBooleanField actually returns jboolean, not bool.
        *address = env->GetBooleanField(javaThis, field) ? true : false;
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
        LOG_ERROR("Unable to copy data to field '%s'", key.c_str());
      }
    }
  }
}

jobject ClassWrapper::toJavaObject(JNIEnv *env) {
  LOG_DEBUG("Converting native instance of '%s' to Java instance", getSimpleName());

  if (_constructor == NULL) {
    JavaExceptionUtils::throwExceptionOfType(env, kTypeIllegalStateException,
      "Cannot call toJavaObject without a constructor (did you forget to call cacheConstructor() in initialize()?");
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
  result.set(env->NewObject(_clazz, _constructor));
  for (FieldMap::const_iterator iter = _fields->begin(); iter != _fields->end(); ++iter) {
    std::string key = iter->first;
    LOG_DEBUG("Copying field %s", key.c_str());

    jfieldID field = iter->second;
    const FieldMapping *mapping = getFieldMapping(key.c_str());
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
        LOG_ERROR("Unable to copy data to field '%s'", key.c_str());
      }
    }
  }

  // Persist the current object address to the Java instance
  persist(env, result);
  return result.leak();
}

jmethodID ClassWrapper::getMethod(const char *method_name) const {
  if (!isInitialized()) {
    JavaExceptionUtils::throwExceptionOfType(JavaThreadUtils::getEnvForCurrentThread(),
      kTypeIllegalStateException,
      "Cannot call getMethod without class info (forgot to merge?)");
    return NULL;
  }

  const std::string key(method_name);
  MethodMap::const_iterator mapFindIter = _methods->find(key);
  if (mapFindIter == _methods->end()) {
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
  FieldMap::const_iterator mapFindIter = _fields->find(key);
  if (mapFindIter == _fields->end()) {
    JavaExceptionUtils::throwExceptionOfType(JavaThreadUtils::getEnvForCurrentThread(),
      kTypeIllegalArgumentException,
      "Field '%s' is not cached in class '%s'", field_name, getCanonicalName());
    return NULL;
  }

  return mapFindIter->second;
}

void ClassWrapper::setClass(JNIEnv *env) {
  LOG_INFO("Looking up corresponding Java class for '%s'", getCanonicalName());
  _clazz_global.set(env->FindClass(getCanonicalName()));
  JavaExceptionUtils::checkException(env);
  _clazz = _clazz_global.get();
}

void ClassWrapper::cacheConstructor(JNIEnv *env) {
  LOG_DEBUG("Caching constructor in class '%s'", getSimpleName());
  if (!isInitialized()) {
    JavaExceptionUtils::throwExceptionOfType(env, kTypeIllegalStateException,
      "Attempt to call cacheMethod without having set class info");
    return;
  }

  std::string signature;
  JavaClassUtils::makeSignature(signature, kTypeVoid, NULL);
  _constructor = env->GetMethodID(_clazz_global.get(), "<init>", signature.c_str());
  JavaExceptionUtils::checkException(env);
}

void ClassWrapper::cacheMethod(JNIEnv *env, const char* method_name, const char* return_type, ...) {
  LOG_DEBUG("Caching method '%s' in class '%s'", method_name, getSimpleName());
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

  jmethodID method = env->GetMethodID(_clazz_global.get(), method_name, signature.c_str());
  JavaExceptionUtils::checkException(env);
  if (method != NULL) {
    _methods_global[method_name] = method;
  } else {
    JavaExceptionUtils::throwExceptionOfType(env, kTypeJavaClass(NoSuchMethodError),
      "Method '%s' (signature: %s) not found on class '%s'",
      method_name, signature.c_str(), getCanonicalName());
  }
}

void ClassWrapper::cacheField(JNIEnv *env, const char *field_name, const char *field_type) {
  LOG_DEBUG("Caching field '%s' (type %s) in class '%s'", field_name, field_type, getSimpleName());
  if (!isInitialized()) {
    JavaExceptionUtils::throwExceptionOfType(env, kTypeIllegalStateException,
      "Attempt to call cacheField without having set class info");
    return;
  }

  //TODO copy/pasted from JavaClassUtils. Should be DRYed up.
  std::stringstream signatured_type;
  if (strlen(field_type) == 1) {
    // Primitive type, can be directly appended
    signatured_type << field_type;
  } else if (field_type[0] == '[') {
    // Array types can also be directly appended
    signatured_type << field_type;
  } else {
    // Class names must be proceeded with an "L" and have a semicolon at the end,
    // however the canonical signatures provided in classes like ClassWrapper are
    // not expected to provide these. So check to see if this is a proper class
    // signature, and make one if not.
    if (field_type[0] == 'L' && field_type[strlen(field_type) - 1] == ';') {
      signatured_type << field_type;
    } else {
      signatured_type << "L" << field_type << ";";
    }
  }

  jfieldID field = env->GetFieldID(_clazz_global.get(), field_name, signatured_type.str().c_str());
  JavaExceptionUtils::checkException(env);
  if (field != NULL) {
    _fields_global[field_name] = field;
  }
}

void ClassWrapper::mapField(const char *field_name, const char *field_type, void *field_ptr) {
  FieldMapping *mapping = new FieldMapping;
  mapping->type = field_type;
  mapping->address = field_ptr;
  _field_mappings[field_name] = mapping; // .reset(mapping);
}

const FieldMapping* ClassWrapper::getFieldMapping(const char *key) const {
  std::string keyString(key);
  FieldMappingMap::const_iterator findMapIter = _field_mappings.find(keyString);
  return findMapIter != _field_mappings.end() ? findMapIter->second : NULL;
}

void ClassWrapper::addNativeMethod(const char *method_name, void *function, const char *return_type, ...) {
  LOG_DEBUG("Adding native method '%s' to class '%s'", method_name, getSimpleName());
  JNINativeMethod nativeMethod;
  nativeMethod.name = const_cast<char*>(method_name);
  nativeMethod.fnPtr = function;

  va_list arguments;
  va_start(arguments, return_type);
  std::string signature;
  JavaClassUtils::makeSignatureWithList(signature, return_type, arguments);
  nativeMethod.signature = const_cast<char*>(strdup(signature.c_str()));
  LOG_DEBUG("Native signature is '%s'", nativeMethod.signature);
  va_end(arguments);

  _jni_methods.push_back(nativeMethod);
}

bool ClassWrapper::registerNativeMethods(JNIEnv *env) {
  LOG_DEBUG("Registering native methods on class '%s'", getSimpleName());
  if (_jni_methods.empty()) {
    return false;
  }

  if (!isInitialized()) {
    JavaExceptionUtils::throwRuntimeException(env, "Could not find cached class for %s", getCanonicalName());
    return false;
  }

  return (env->RegisterNatives(_clazz_global.get(), &_jni_methods[0], (jint)_jni_methods.size()) < 0);
}

} // namespace jni
} // namespace spotify
