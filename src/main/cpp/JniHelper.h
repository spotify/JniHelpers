#ifndef __JniHelper_h__
#define __JniHelper_h__

#include "JniClassWrapper.h"
#include "JniLocalRef.h"
#include <jni.h>
#include <map>
#include <string>

class JniHelper {
public:
  static __declspec(dllexport) void initialize(JavaVM *jvm);
  static __declspec(dllexport) JniHelper* get();

protected:
  // Disallow direct construction; must call initialize() instead
  explicit JniHelper(JavaVM *jvm);

private:
  // Disable copy and no-arg constructors
  JniHelper() {}
  JniHelper(const JniHelper&) {}

public:
  virtual __declspec(dllexport) ~JniHelper();

  JNIEnv* getEnvForCurrentThread();
  JNIEnv* attachCurrentThreadToJVM(const std::string &thread_name);
  void detatchCurrentThreadFromJVM();

  void setJavaClassLoaderForCurrentThread(JNIEnv *env, jobject class_loader);
  jobject lookupJavaClass(JNIEnv *env, const std::string &class_name);

  void checkException(JNIEnv *env);
  JniLocalRef<jobject> newThrowable(JNIEnv *env, const char *message, ...);
  void throwRuntimeException(JNIEnv *env, const char *message, ...);

  bool __declspec(dllexport) addClassWrapper(JniClassWrapper *jniClass);
  JniClassWrapper* getClassWrapper(const char* name) const;

private:
  JavaVM *_jvm;
  std::map<std::string, JniClassWrapper*> _classes;
};

#endif // __JniHelper_h__
