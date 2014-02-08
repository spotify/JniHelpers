#ifndef __JniObject_h__
#define __JniObject_h__

#include <jni.h>
#include <map>
#include <string>
#include <vector>

class JniClass {
private:
  // Disallow no-arg constructor
  JniClass() {}
  // Disallow copy constructor
  JniClass(const JniClass&) {}

public:
  /**
   * Create a new JNI class wrapper.
   * @param env JVM environment
   */
  explicit JniClass(JNIEnv *env);
  virtual ~JniClass();

  virtual void initialize(JNIEnv *env) = 0;
  virtual const char* getClassName() const = 0;

  virtual JniClass* fromJavaObject(JNIEnv *env, jobject javaObject) const = 0;
  virtual jobject toJavaObject(JniClass*) = 0;

  jmethodID getMethod(const char *field_name);
  jmethodID getField(const char* field_name);
  template<typename TypeName>
  TypeName getFieldValue(jobject instance, const char* field_name);

protected:
  void cacheMethod(const char* method_name, const char* return_type, ...);
  void cacheField(const char* field_name);

  template<typename FunctionPtr>
  JNINativeMethod makeNativeMethod(const char *method_name, FunctionPtr *function, const char *return_type, ...);
  bool registerNativeMethods(JNIEnv *env, const std::string &class_name, const std::vector<JNINativeMethod> &methods);

protected:
  static std::map<std::string, jmethodID> _methods;
  static std::map<std::string, jfieldID> _fields;
};

#endif // __JniObject_h__
