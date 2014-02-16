#ifndef __ClassWrapper_h__
#define __ClassWrapper_h__

#include "JniHelpersCommon.h"
#include "JavaString.h"
#include "JniGlobalRef.h"
#include <map>
#include <string>
#include <sstream>
#include <vector>
#include <stdarg.h>

namespace spotify {
namespace jni {

typedef struct {
  const char* type;
  void* address;
} FieldMapping;

#if WIN32
// TODO: Should only be defined for dynamic lib builds
template class EXPORT std::vector<JNINativeMethod>;
template class EXPORT std::map<std::string, jmethodID>;
template class EXPORT std::map<std::string, jfieldID>;
template class EXPORT std::map<std::string, FieldMapping*>; 
#endif

class EXPORT ClassWrapper {
public:
  ClassWrapper() {}
  ClassWrapper(JNIEnv *env) {
    // TODO: Can't call initialize here because of symbol visibility :(
    // initialize(env);
  }
  virtual ~ClassWrapper() {
    // TODO: Delete mappings
  }

  virtual void initialize(JNIEnv *env) = 0;
  virtual const char* getCanonicalName() const = 0;

public:
  const char* getSimpleName() const;
  void merge(const ClassWrapper *globalInstance);
  void setJavaObject(JNIEnv *env, jobject javaThis);
  jobject toJavaObject(JNIEnv *env);

  JniGlobalRef<jclass> getClass() const;
  jmethodID getMethod(const char *method_name);
  jfieldID getField(const char* field_name);

protected:
  void setClass(JNIEnv *env);
  void cacheMethod(JNIEnv *env, const char *method_name, const char *return_type, ...);
  void cacheField(JNIEnv *env, const char *field_name, const char *field_type);

  void mapField(const char *field_name, const char *field_type, void *field_ptr);
  void setFieldsFrom(JNIEnv *env, jobject javaThis);
  void setFieldsTo(JNIEnv *env, jobject javaThis);

  void addNativeMethod(const char *method_name, void *function, const char *return_type, ...);
  bool registerNativeMethods(JNIEnv *env);

protected:
  JniGlobalRef<jclass> _clazz;
  jmethodID _constructor;
  std::map<std::string, jmethodID> _methods;
  std::map<std::string, jfieldID> _fields;
  std::map<std::string, FieldMapping*> _field_mappings;

private:
  std::vector<JNINativeMethod> _jni_methods;
};

} // namespace jni
} // namespace spotify

#endif // __ClassWrapper_h__
