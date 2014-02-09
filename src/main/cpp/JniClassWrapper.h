#ifndef __JniClassWrapper_h__
#define __JniClassWrapper_h__

#include "JniHelpersCommon.h"
#include <map>
#include <string>
#include <sstream>
#include <vector>

namespace spotify {
namespace jni {

class JniClassWrapper {
protected:
  JniClassWrapper(JNIEnv *env) {}
  JniClassWrapper(JNIEnv *env, jobject fromObject) {}
  virtual ~JniClassWrapper() {}

public:
  virtual void initialize(JNIEnv *env) = 0;
  virtual const char* getPackageName() const = 0;
  virtual const char* getSimpleName() const = 0;

  virtual void setJavaObject(JNIEnv *env, jobject javaObject) = 0;
  virtual jobject toJavaObject(JniClassWrapper *nativeObject) = 0;

public:
  EXPORT const char* getCanonicalName() const;

public:
  EXPORT jmethodID getMethod(const char *field_name);
  EXPORT jmethodID getField(const char* field_name);
  template<typename TypeName>
  EXPORT TypeName getFieldValue(jobject instance, const char* field_name);

protected:
  EXPORT void cacheMethod(const char* method_name, const char* return_type, ...);
  EXPORT void cacheField(const char* field_name);

  //template<typename FunctionPtr>
  EXPORT JNINativeMethod makeNativeMethod(const char *method_name, void *function, const char return_type, ...);
  //template<typename FunctionPtr>
  EXPORT JNINativeMethod makeNativeMethod(const char *method_name, void *function, const char *return_type, ...);
  EXPORT bool registerNativeMethods(JNIEnv *env, const std::string &class_name, const std::vector<JNINativeMethod> &methods);

protected:
  static const char kTypeVoid = 'V';
  static const char* kTypeString;

protected:
  static std::map<std::string, jmethodID> _methods;
  static std::map<std::string, jfieldID> _fields;
};

} // namespace jni
} // namespace spotify

#endif // __JniClassWrapper_h__
