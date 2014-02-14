#ifndef __ClassWrapper_h__
#define __ClassWrapper_h__

#include "JniHelpersCommon.h"
#include "JniGlobalRef.h"
#include <map>
#include <string>
#include <sstream>
#include <vector>
#include <stdarg.h>

namespace spotify {
namespace jni {

#if WIN32
// TODO: Should only be defined for dynamic lib builds
template class EXPORT std::vector<JNINativeMethod>;
template class EXPORT std::map<std::string, jmethodID>;
template class EXPORT std::map<std::string, jfieldID>;
#endif

class EXPORT ClassWrapper {
public:
  ClassWrapper() {}
  ClassWrapper(JNIEnv *env) {
    // TODO: Can't call initialize here because of symbol visibility :(
    // initialize(env);
  }
  virtual ~ClassWrapper() {}

  virtual void initialize(JNIEnv *env) = 0;
  virtual const char* getPackageName() const = 0;
  virtual const char* getSimpleName() const = 0;

  virtual void setJavaObject(JNIEnv *env, jobject javaObject) = 0;
  virtual jobject toJavaObject(ClassWrapper *nativeObject) = 0;

public:
  const char* getCanonicalName() const;
  void merge(const ClassWrapper *globalInstance);

public:
  jmethodID getMethod(const char *method_name);
  jfieldID getField(const char* field_name);
  // TODO: I'm not really sure if this will work
  template<typename TypeName>
  TypeName getFieldValue(JNIEnv *env, jobject instance, const char* field_name);

protected:
  void setClass(JNIEnv *env);
  void cacheMethod(JNIEnv *env, const char *method_name, const char *return_type, ...);
  void cacheField(JNIEnv *env, const char *field_name, const char *field_type);

  void addNativeMethod(const char *method_name, void *function, const char *return_type, ...);
  bool registerNativeMethods(JNIEnv *env);

protected:
  JniGlobalRef<jclass> _clazz;
  std::map<std::string, jmethodID> _methods;
  std::map<std::string, jfieldID> _fields;

private:
  std::vector<JNINativeMethod> _jni_methods;
};

} // namespace jni
} // namespace spotify

#endif // __ClassWrapper_h__
