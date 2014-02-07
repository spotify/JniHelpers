#ifndef __JniObject_h__
#define __JniObject_h__

#include <jni.h>
#include <map>
#include <string>
#include <vector>

namespace spotify {

class JniClass {
public:
  explicit JniClass(JNIEnv *env);
  virtual ~JniClass();

  virtual void initialize(JNIEnv *env) = 0;
  virtual const char* getClassName() const = 0;

  virtual JniClass* fromJavaObject(JNIEnv *env, jobject javaObject) const = 0;
  virtual jobject toJavaObject(JniClass*) = 0;

  template<typename TypeName>
  TypeName getFieldValue(jobject instance, const char* field_name);

  void cacheMethod(const char* method_name, const char* return_type, ...);
  jmethodID lookupMethod(const char *field_name);

  void cacheField(const char* field_name);
  jmethodID lookupField(const char* field_name);

  template<typename FunctionPtr>
  JNINativeMethod makeNativeMethod(const char *method_name, FunctionPtr *function, const char *return_type, ...);
  bool registerNativeMethods(JNIEnv *env, const std::string &class_name, const std::vector<JNINativeMethod> &methods);

private:
  JniClass() {}
  JniClass(const JniClass&) {}

protected:
  static std::map<std::string, jmethodID> _methods;
  static std::map<std::string, jfieldID> _fields;
};

} // namespace spotify

#endif // __JniObject_h__
