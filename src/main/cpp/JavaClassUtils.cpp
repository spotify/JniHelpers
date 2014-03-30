#include "JavaClassUtils.h"
#include "ClassWrapper.h"
#include "JavaExceptionUtils.h"
#include <sstream>
#include <string.h>

namespace spotify {
namespace jni {

class JavaClassLoader : public ClassWrapper {
public:
  JavaClassLoader(JNIEnv *env) : ClassWrapper(env) { initialize(env); }
  ~JavaClassLoader() {}

  const char* getCanonicalName() const { return kTypeJavaClass(ClassLoader); }

  void initialize(JNIEnv *env) {
    setClass(env);
    cacheMethod(env, "loadClass", kTypeJavaClass(Class), kTypeString, NULL);
    std::string signature;
    JavaClassUtils::makeSignature(signature, getCanonicalName(), NULL);
    methodGetSystemClassLoader = env->GetStaticMethodID(_clazz, "getSystemClassLoader", signature.c_str());
  }

  void mapFields() {}

  jobject getClassLoader(JNIEnv *env) {
    jobject classLoader = env->CallStaticObjectMethod(_clazz, methodGetSystemClassLoader);
    JavaExceptionUtils::checkException(env);
    return classLoader;
  }

  jclass loadClass(JNIEnv *env, const char *class_name) {
    std::string binaryName = class_name;
    for (int i = 0; i < binaryName.length(); ++i) {
      if (binaryName[i] == '/') {
        binaryName[i] = '.';
      }
    }

    LOG_DEBUG("Using ClassLoader to look up '%s'", binaryName.c_str());
    JavaString className(binaryName);
    jobject classLoader = getClassLoader(env);
    jclass result = (jclass)env->CallObjectMethod(classLoader, getMethod("loadClass"),
      className.toJavaString(env).get());
    JavaExceptionUtils::checkException(env);
    return result;
  }

private:
  jmethodID methodGetSystemClassLoader;
};

// Yes, I hate global static data as much as the next guy, however the alternative here would
// be to keep the ClassLoader instance in the ClassRegistry. While that isn't such a bad idea,
// it would require findClass() to be altered to take a ClassRegistry parameter, which looks a
// bit weird. The ClassLoader is provided by the JVM and will live for the lifetime of the
// application.
static JavaClassLoader* sJavaClassLoader = NULL;

void JavaClassUtils::setJavaClassLoader(JNIEnv *env) {
  LOG_DEBUG("Finding Java ClassLoader");
  if (sJavaClassLoader == NULL) {
    sJavaClassLoader = new JavaClassLoader(env);
  }
}

jclass JavaClassUtils::findClass(JNIEnv *env, const char *class_name, bool useClassLoader) {
  jclass result = NULL;
  if (useClassLoader) {
    LOG_DEBUG("Finding class '%s' with thread ClassLoader", class_name);
    if (sJavaClassLoader == NULL) {
      LOG_DEBUG("Java ClassLoader is null, doing lazy initialization");
      setJavaClassLoader(env);
    }

    jobject classLoader = sJavaClassLoader->getClassLoader(env);
    if (classLoader == NULL) {
      // If we can't get the ClassLoader then we're out of luck and must bail out...
      JavaExceptionUtils::throwExceptionOfType(env, kTypeIllegalStateException,
        "Could not find ClassLoader for thread");
      return NULL;
    }

    result = sJavaClassLoader->loadClass(env, class_name);
  } else {
    LOG_DEBUG("Finding class '%s' with direct JNI call", class_name);
    result = env->FindClass(class_name);
    JavaExceptionUtils::checkException(env);
  }

  LOG_DEBUG("Lookup of class was %s", result != NULL ? "successful" : "unsuccessful");
  return result;
}

void JavaClassUtils::makeNameForSignature(std::string &receiver, const char *name) {
  if (name == NULL) {
    JavaExceptionUtils::throwExceptionOfType(JavaThreadUtils::getEnvForCurrentThread(),
      kTypeIllegalArgumentException, "Attempt to call makeNameForSignature with NULL name");
    return;
  } else if (strlen(name) == 1) {
    // Primitive type, can be directly appended
    receiver = name;
  } else if (name[0] == '[') {
    // Array types can also be directly appended
    receiver = name;
  } else {
    // Class names must be proceeded with an "L" and have a semicolon at the end,
    // however the canonical signatures provided in classes like ClassWrapper are
    // not expected to provide these. So check to see if this is a proper class
    // signature, and make one if not.
    if (name[0] == 'L' && name[strlen(name) - 1] == ';') {
      receiver = name;
    } else {
      std::stringstream stream;
      stream << "L" << name << ";";
      receiver = stream.str();
    }
  }
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
    std::string argumentSignature;
    makeNameForSignature(argumentSignature, argument);
    stringstream << argumentSignature;
  }
  stringstream << ")";

  if (return_type == NULL) {
    stringstream << kTypeVoid;
  } else {
    std::string returnTypeSignature;
    makeNameForSignature(returnTypeSignature, return_type);
    stringstream << returnTypeSignature;
  }

  receiver = stringstream.str();
}

} // namespace jni
} // namespace spotify
