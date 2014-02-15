#include "JavaClassUtils.h"
#include "ClassWrapper.h"
#include "JavaExceptionUtils.h"
#include <sstream>
#include <string.h>

namespace spotify {
namespace jni {

static jobject sJavaClassLoader = NULL;

void JavaClassUtils::setJavaClassLoaderForCurrentThread(JNIEnv *env, jobject class_loader) {
  if (sJavaClassLoader != NULL) {
    env->DeleteGlobalRef(class_loader);
  }

  sJavaClassLoader = env->NewGlobalRef(class_loader);
}

jclass JavaClassUtils::findJavaClass(JNIEnv *env, const char *class_name) {
  jclass result = env->FindClass(class_name);
  JavaExceptionUtils::checkException(env);
  return result;
}

jclass JavaClassUtils::findAndLoadJavaClass(JNIEnv *env, const char *class_name) {
  if (sJavaClassLoader == NULL) {
    return NULL;
  }

  jclass cls = env->FindClass(kTypeJavaClass("ClassLoader"));
  JavaExceptionUtils::checkException(env);

  const char *signature = makeSignature(kTypeJavaClass("Class"), kTypeJavaString, NULL);
  jmethodID methodLoadClass = env->GetMethodID(cls, "loadClass", signature);
  JavaExceptionUtils::checkException(env);

  jstring className = env->NewStringUTF(class_name);
  JavaExceptionUtils::checkException(env);

  jclass result = (jclass)env->CallObjectMethod(sJavaClassLoader, methodLoadClass, className);
  JavaExceptionUtils::checkException(env);

  return result;
}

const char* JavaClassUtils::makeCanonicalClassName(const char *package_name, const char *class_name) {
  std::stringstream stringstream;
  stringstream << package_name << "/" << class_name;
  return stringstream.str().c_str();
}

const char* JavaClassUtils::makeSignature(const char *return_type, ...) {
  va_list arguments;
  va_start(arguments, return_type);
  const char *result = makeSignature(return_type, arguments);
  va_end(arguments);
  return result;
}

const char* JavaClassUtils::makeSignature(const char *return_type, va_list arguments) {
  std::stringstream stringstream;
  stringstream << "(";
  char *argument;
  while ((argument = va_arg(arguments, char*)) != NULL) {
    if (strlen(argument) > 1) {
      // Class names must be proceeded with an "L" and have a semicolon afterwards
      stringstream << "L" << argument << ";";
    } else {
      // Primitive types can simply be appended
      stringstream << argument;
    }
  }
  stringstream << ")";

  // TODO: Shameful copy/paste job here...
  if (return_type == NULL) {
    stringstream << kTypeVoid;
  } else if (strlen(return_type) > 1) {
    // Class names must be proceeded with an "L" and have a semicolon afterwards
    stringstream << "L" << return_type << ";";
  } else {
    // Primitive types can simply be appended
    stringstream << return_type;
  }

  return stringstream.str().c_str();
}

} // namespace jni
} // namespace spotify
