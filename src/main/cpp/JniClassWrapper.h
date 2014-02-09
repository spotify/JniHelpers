#ifndef __JniClassWrapper_h__
#define __JniClassWrapper_h__

#include <jni.h>
#include <map>
#include <string>
#include <vector>

namespace spotify {
namespace jni {

class JniClassWrapper {
public:
  /**
  * Create a new JNI class wrapper.
  * @param env JVM environment
  */
  explicit __declspec(dllexport) JniClassWrapper(JNIEnv *env);

private:
  // Disallow no-arg constructor
  JniClassWrapper() {}
  // Disallow copy constructor
  JniClassWrapper(const JniClassWrapper&) {}

public:
  __declspec(dllexport)
  virtual ~JniClassWrapper();

  virtual void initialize(JNIEnv *env) = 0;
  virtual const char* getClassName() const = 0;

  virtual JniClassWrapper* fromJavaObject(JNIEnv *env, jobject javaObject) const = 0;
  virtual jobject toJavaObject(JniClassWrapper *nativeObject) = 0;

public:
  jmethodID __declspec(dllexport) getMethod(const char *field_name);
  jmethodID __declspec(dllexport) getField(const char* field_name);
  template<typename TypeName> __declspec(dllexport)
  TypeName getFieldValue(jobject instance, const char* field_name);

protected:
  void __declspec(dllexport) cacheMethod(const char* method_name, const char* return_type, ...);
  void __declspec(dllexport) cacheField(const char* field_name);

  template<typename FunctionPtr> __declspec(dllexport)
  JNINativeMethod makeNativeMethod(const char *method_name, FunctionPtr *function, const char *return_type, ...);
  bool __declspec(dllexport)
  registerNativeMethods(JNIEnv *env, const std::string &class_name, const std::vector<JNINativeMethod> &methods);

protected:
  static std::map<std::string, jmethodID> _methods;
  static std::map<std::string, jfieldID> _fields;
};

} // namespace jni
} // namespace spotify

#endif // __JniClassWrapper_h__
