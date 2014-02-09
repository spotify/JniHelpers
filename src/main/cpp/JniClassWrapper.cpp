#include "JniClassWrapper.h"
#include <sstream>

namespace spotify {
namespace jni {

void JniClassWrapper::merge(JniClassWrapper *globalInstance) {
  _methods = globalInstance->_methods;
  _fields = globalInstance->_fields;
}

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

void JniClassWrapper::addNativeMethod(const char *method_name, void *function, const char *return_type, ...) {
  JNINativeMethod nativeMethod;
  nativeMethod.name = const_cast<char*>(method_name);
  nativeMethod.fnPtr = function;

  va_list arguments;
  va_start(arguments, return_type);
  std::stringstream stringstream;
  stringstream << "(";
  void *argument;
  while ((argument = va_arg(arguments, void*)) != NULL) {
    char *argumentAsString = static_cast<char*>(argument);
    if (argumentAsString != NULL) {
      if (strlen(argumentAsString) > 1) {
        // Class names must be proceeded with an "L" and have a semicolon afterwards
        stringstream << "L" << argument << ";";
      } else {
        // Primitive types can simply be appended
        stringstream << argument;
      }
      continue;
    }

    JniClassWrapper *argumentAsClass = static_cast<JniClassWrapper*>(argument);
    if (argumentAsClass != NULL) {
      stringstream << "L" << argumentAsClass->getCanonicalName() << ";";
      continue;
    }

    // TODO: Invalid object passed to function, error
  }
  va_end(arguments);
  nativeMethod.signature = const_cast<char*>(stringstream.str().c_str());

  _jni_methods.push_back(nativeMethod);
}

bool JniClassWrapper::registerNativeMethods(JNIEnv *env) {
  size_t numMethods = _jni_methods.size();
  JNINativeMethod *methods = new JNINativeMethod[numMethods];
  for (int i = 0; i < numMethods; ++i) {
    methods[i] = _jni_methods[i];
  }
  // TODO: Class loading is expensive, this should be cached
  jclass clazz = env->FindClass(getCanonicalName());
  env->RegisterNatives(clazz, methods, (jint)numMethods);
  delete[] methods;
  return true; // TODO: Real error checking
}

} // namespace jni
} // namespace spotify
