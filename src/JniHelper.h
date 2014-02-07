#ifndef __JniHelper_h__
#define __JniHelper_h__

#include "JniClass.h"
#include "JniLocalRef.h"
#include <jni.h>
#include <map>
#include <string>

namespace spotify {

class JniHelper {
public:
  static void initialize(JavaVM *jvm);
  static JniHelper* get();

public:
  virtual ~JniHelper();

  JNIEnv* getEnvForCurrentThread();
  JNIEnv* attachCurrentThreadToJVM(const std::string &thread_name);
  void detatchCurrentThreadFromJVM();

  jobject lookupClass(JNIEnv *env, const std::string &class_name);
  void setClassLoaderForCurrentThread(JNIEnv *env, jobject class_loader);
  bool initializeClass(JniClass *jniClass);
  JniClass* getClass(const char* name) const;

  void checkException(JNIEnv *env);
  JniLocalRef<jobject> newThrowable(JNIEnv *env, const char *message, ...);
  void throwRuntimeException(JNIEnv *env, const char *message, ...);

protected:
  JniHelper(JavaVM *jvm);

private:
  JavaVM *_jvm;
  std::map<std::string, JniClass*> _classes;
};

} // namespace spotify

#endif // __JniHelper_h__
