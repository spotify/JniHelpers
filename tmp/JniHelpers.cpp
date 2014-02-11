#include "JniHelpers.h"
#include <string.h>

static JavaVM *sJavaVm = NULL;
static jobject sJavaClassLoader = NULL;

JNIEnv *JniCurrentEnv() {
  JNIEnv *env;

  if (sJavaVm == NULL) {
    return NULL;
  }

  if (sJavaVm->GetEnv((void**) &env, JNI_VERSION_1_4) != JNI_OK) {
    // The current thread isn't attached to a JNIEnv, return NULL.
    return NULL;
  }

  return env;
}

JNIEnv *JniAttachCurrentThreadToVm(const std::string &thread_name) {
  JNIEnv *env;
  JavaVMAttachArgs args;

  args.version = JNI_VERSION_1_4;
  args.name = const_cast<char*>(thread_name.c_str());
  args.group = NULL;

#ifdef ANDROID
  sJavaVm->AttachCurrentThread((JNIEnv**)(&env), &args);
#else
  sJavaVm->AttachCurrentThread((void**)(&env), &args);
#endif

  return env;
}

void JniDetachCurrentThreadFromVm() {
  sJavaVm->DetachCurrentThread();
}

void JniSetJavaVmForNative(JavaVM *java_vm) {
  sJavaVm = java_vm;
}

void JniSetClassLoaderForNative(JNIEnv *env, jobject class_loader) {
  if (sJavaClassLoader != NULL) {
    env->DeleteGlobalRef(class_loader);
  }

  sJavaClassLoader = env->NewGlobalRef(class_loader);
}

jclass JniFindClassFromNativeThread(JNIEnv *env, const std::string &class_name) {
  if (sJavaClassLoader == NULL) {
    return NULL;
  }

  jclass cls = env->FindClass("java/lang/ClassLoader");
  jmethodID methodLoadClass = env->GetMethodID(cls, "loadClass", "(Ljava/lang/String;)Ljava/lang/Class;");

  jstring className = env->NewStringUTF(class_name.c_str());
  jclass result = (jclass) env->CallObjectMethod(sJavaClassLoader, methodLoadClass, className);
  JniCheckForExceptionAndReport(env);

  return result;
}

void JniThrowRuntimeException(JNIEnv *env, const char* message) {
  jclass cls = env->FindClass("java/lang/RuntimeException");
  env->ThrowNew(cls, message);
}

void JniCheckForExceptionAndReport(JNIEnv *env) {
  if (!env->ExceptionCheck()) {
    return;
  }

  env->ExceptionDescribe();
  std::terminate();
}

bool JniRegisterNativeMethods(JNIEnv* env, const std::string &class_name, const std::vector<JNINativeMethod> &methods) {
  if (methods.empty()) {
    return false;
  }

  jclass cls = env->FindClass(class_name.c_str());
  if (!cls) {
    return false;
  }

  if (env->RegisterNatives(cls, &methods[0], methods.size()) < 0) {
    return false;
  }

  return true;
}

std::string JniStringToString(JNIEnv *env, jstring jstr) {
  std::string result;
  if (jstr == NULL)
    return result;

  const char *str = env->GetStringUTFChars(jstr, 0);

  if (str == NULL)
    return result;

  result = str;
  env->ReleaseStringUTFChars(jstr, str);

  return result;
}

JniLocalRef<jstring> JniStringFromString(JNIEnv *env, const std::string &str) {
  return env->NewStringUTF(str.c_str());
}

JniLocalRef<jbyteArray> JniNewByteArray(JNIEnv *env, const void *data, size_t data_size) {
  JniLocalRef<jbyteArray> byteArray = env->NewByteArray(data_size);
  JniCheckForExceptionAndReport(env);
  env->SetByteArrayRegion(byteArray, 0, data_size, (jbyte *)data);
  return byteArray;
}

JniLocalRef<jobject> JniNewThrowable(JNIEnv *env, const std::string &message) {
  jclass cls = env->FindClass("java/lang/Throwable");
  if (!cls) {
  JniThrowRuntimeException(env, "Could not find class Throwable");
  return nullptr;
}

jmethodID throwableCtor = env->GetMethodID(cls, "<init>", "(" _jstring ")" _jvoid);
  if (!throwableCtor) {
  JniThrowRuntimeException(env, "Could not find Throwable constructor");
  return nullptr;
}

JniLocalRef<jstring> messageString = JniStringFromString(env, message);
  JniCheckForExceptionAndReport(env);
  JniLocalRef<jobject> result = env->NewObject(cls, throwableCtor, messageString.get());
  JniCheckForExceptionAndReport(env);
  return result;
}

char *JniGetStringField(JNIEnv *env, jclass cls, jobject object, const char *field_name) {
  jfieldID field = env->GetFieldID(cls, field_name, _jstring);
  if (field == NULL) {
    JniThrowRuntimeException(env, "Could not find field in class");
    return NULL;
  }

  jstring fieldValue = (jstring)env->GetObjectField(object, field);
  JniCheckForExceptionAndReport(env);
  if (fieldValue == NULL) {
    return NULL;
  }

  std::string string = JniStringToString(env, fieldValue);
  JniCheckForExceptionAndReport(env);
  return strdup(string.c_str());
}

bool JniGetBooleanField(JNIEnv *env, jclass cls, jobject object, const char *field_name) {
  jfieldID field = env->GetFieldID(cls, field_name, _jbool);
  if (field == NULL) {
    JniThrowRuntimeException(env, "Could not find field in class");
    return false;
  }

  jboolean fieldValue = env->GetBooleanField(object, field);
  JniCheckForExceptionAndReport(env);
  return fieldValue;
}
