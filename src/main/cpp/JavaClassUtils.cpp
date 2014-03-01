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

jclass JavaClassUtils::findClass(JNIEnv *env, const char *class_name, bool useClassLoader) {
  jclass result = NULL;
  if (useClassLoader) {
    if (sJavaClassLoader == NULL) {
      return NULL;
    }

    jclass classLoader = env->FindClass(kTypeJavaClass("ClassLoader"));
    JavaExceptionUtils::checkException(env);

    std::string signature;
    makeSignature(signature, kTypeJavaClass("Class"), kTypeString, NULL);
    jmethodID methodLoadClass = env->GetMethodID(classLoader, "loadClass", signature.c_str());
    JavaExceptionUtils::checkException(env);

    jstring className = env->NewStringUTF(class_name);
    JavaExceptionUtils::checkException(env);

    jclass result = (jclass)env->CallObjectMethod(sJavaClassLoader, methodLoadClass, className);
    JavaExceptionUtils::checkException(env);

  } else {
    result = env->FindClass(class_name);
    JavaExceptionUtils::checkException(env);
  }

  return result;
}

void JavaClassUtils::makeSignature(std::string &receiver, const char *return_type, ...) {
  va_list arguments;
  va_start(arguments, return_type);
  makeSignatureWithList(receiver, return_type, arguments);
  va_end(arguments);
}

void JavaClassUtils::makeSignatureWithList(std::string &receiver, const char *return_type, va_list arguments) {
  std::stringstream stringstream;
  stringstream << "(";
  char *argument;
  while ((argument = va_arg(arguments, char*)) != NULL) {
    appendTypeToSignature(stringstream, argument);
  }
  stringstream << ")";

  if (return_type == NULL) {
    stringstream << kTypeVoid;
  } else {
    appendTypeToSignature(stringstream, return_type);
  }

  receiver = stringstream.str();
}

void JavaClassUtils::appendTypeToSignature(std::stringstream &stringstream, const char *argument) {
  if (strlen(argument) == 1) {
    // Primitive type, can be directly appended
    stringstream << argument;
  } else if (argument[0] == '[') {
    // Array types can also be directly appended
    stringstream << argument;
  } else {
    // Class names must be proceeded with an "L" and have a semicolon at the end,
    // however the canonical signatures provided in classes like ClassWrapper are
    // not expected to provide these. So check to see if this is a proper class
    // signature, and make one if not.
    if (argument[0] == 'L' && argument[strlen(argument) - 1] == ';') {
      stringstream << argument;
    } else {
      stringstream << "L" << argument << ";";
    }
  }
}

} // namespace jni
} // namespace spotify
