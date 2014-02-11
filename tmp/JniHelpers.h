#pragma once

#include <jni.h>

#include <stdlib.h>
#include <string>
#include <vector>

#define _jstring "Ljava/lang/String;"
#define _jint "I"
#define _jlong "J"
#define _jfloat "F"
#define _jdouble "D"
#define _jbool "Z"
#define _jbyte "B"
#define _jvoid "V"
#define _jarray(x) "[" x

// A JavaVM must be set first thing when JNI_OnLoad is called, things will crash without it.
void JniSetJavaVmForNative(JavaVM *java_vm);

// Returns the JNIEnv associated with the current thread, if any. Threads created from native
// are not attached to the Java VM by default, and won't have a JNIEnv until you attach them.
JNIEnv *JniCurrentEnv();

// Attach current thread to the Java VM and gives it the name thread_name. The name can be
// seen in DDMS and is useful for debugging. Remember that attached threads must be detached
// before stopping!
JNIEnv *JniAttachCurrentThreadToVm(const std::string &thread_name);

// Detach current thread from the Java VM. Any thread that has attached itself to the VM using
// the above call must also call this before stopping!
void JniDetachCurrentThreadFromVm();

// Native threads attached to the Java VM do not have a ClassLoader by default, so they will
// use the system ClassLoader, which is only aware of system classes. So if you try to use
// FindClass on any of the classes in your package, it will fail with ClassNotFoundException.
// These two utility functions gives a way to work around that - before using
// JniFindClassFromNativeThread() to load Java classes, make sure to call
// JniSetClassLoaderForNative() with a ClassLoader that _does_ know about your package. Such
// a ClassLoader can be acquired by calling Context.getClassLoader() from a Context living in
// said package. You should only need to do this once, at startup.
//
// Reference: "Android JNI Tips" http://www.milk.com/kodebase/dalvik-docs-mirror/docs/jni-tips.html#FAQFindClass
//
void JniSetClassLoaderForNative(JNIEnv *env, jobject class_loader);
jclass JniFindClassFromNativeThread(JNIEnv *env, const std::string &class_name);

void JniThrowRuntimeException(JNIEnv *env, const char* message);

// Clears any pending exception and prints it out (potentially reporting it to Breakpad). If
// compiled with _DEBUG, it will force a crash to make unhandled exceptions more visible.
void JniCheckForExceptionAndReport(JNIEnv *env);

// Registers native methods in Java classes.
bool JniRegisterNativeMethods(JNIEnv* env, const std::string &class_name, const std::vector<JNINativeMethod> &methods);

template<typename FuncType>
JNINativeMethod JniNativeMethod(const char *name, const char *signature, FuncType *func) {
  return (JNINativeMethod){(char *)name, (char *)signature, (void *)func};
}

// RAII helper to maintain local references automatically.
template<typename JniType>
class JniLocalRef {
 public:
  JniLocalRef() : _obj(NULL) {}
  JniLocalRef(JniType obj) : _obj(NULL) { set(obj); }
  JniLocalRef(const JniLocalRef<JniType> &ref) : _obj(NULL) {
    set((JniType)JniCurrentEnv()->NewLocalRef(ref.get()));
  }

  ~JniLocalRef() { set(NULL); }

  JniType get() const { return _obj; }
  void set(JniType obj) {
    JNIEnv *env = JniCurrentEnv();
    if (_obj) env->DeleteLocalRef(_obj);
    _obj = obj;
  }
  JniType leak() {
    JniType obj = _obj;
    _obj = NULL;
    return obj;
  }

  operator JniType() const { return _obj; }

  void operator=(JniType obj) { set(obj); }
  void operator=(const JniLocalRef<JniType> &ref) {
    set((JniType)JniCurrentEnv()->NewLocalRef(ref.get()));
  }

 private:
  JniType _obj;
};

// RAII helper to maintain global references automatically.
template<typename JniType>
class JniGlobalRef {
 public:
  JniGlobalRef() : _obj(NULL) {}
  JniGlobalRef(const JniGlobalRef<JniType> &ref) : _obj(NULL) { set(ref.get()); }
  JniGlobalRef(const JniLocalRef<JniType> &ref) : _obj(NULL) { set(ref.get()); }

  ~JniGlobalRef() { set(NULL); }

  JniType get() const { return _obj; }

  void set(JniType obj) {
    JNIEnv *env = JniCurrentEnv();
    if (!env) {
        _obj = NULL;
        return;
    }
    if (_obj) {
      env->DeleteGlobalRef(_obj);
      _obj = NULL;
    }

    if (obj) {
      _obj = (JniType)env->NewGlobalRef(obj);
    }
  }

  operator JniType() const { return _obj; }

  void operator=(const JniLocalRef<JniType> &ref) { set(ref.get()); }

 private:
  JniType _obj;
};

class JniLocalFrame {
 public:
  JniLocalFrame(int capacity) { JniCurrentEnv()->PushLocalFrame(capacity); }
  ~JniLocalFrame() { JniCurrentEnv()->PopLocalFrame(NULL); }
};

class JniByteArray {
 public:
  JniByteArray() : _data(NULL), _data_size(0) {}
  JniByteArray(jbyteArray byteArray) { set(byteArray); }
  ~JniByteArray() { set(NULL); }

  void set(jbyteArray byteArray) {
    JNIEnv *env = JniCurrentEnv();
    if (_data) ::free(_data);
    if (byteArray) {
      _data_size = env->GetArrayLength(byteArray);
      _data = ::malloc(_data_size);

      env->GetByteArrayRegion(byteArray, 0, _data_size, (jbyte *)_data);
    }
  }

  void operator=(jbyteArray byteArray) {
    set(byteArray);
  }

  const void *data() const { return _data; }
  size_t size() const { return _data_size; }

 private:
  void *_data;
  size_t _data_size;
};

// Converts a Java String to a std::string.
std::string JniStringToString(JNIEnv *env, jstring jstr);

// Converts a std::string to a Java String.
JniLocalRef<jstring> JniStringFromString(JNIEnv *env, const std::string &str);

// Creates a new Java byte array with the given data and size.
JniLocalRef<jbyteArray> JniNewByteArray(JNIEnv *env, const void *data, size_t data_size);

// Create a new Throwable with the given message
JniLocalRef<jobject> JniNewThrowable(JNIEnv *env, const std::string &message);

// Get a String field of an object as a char* pointer. The caller is responsible
// for freeing this memory when finished using it.
char *JniGetStringField(JNIEnv *env, jclass cls, jobject object, const char *field_name);

// Get a boolean field of an object as a C++ bool
bool JniGetBooleanField(JNIEnv *env, jclass cls, jobject object, const char *field_name);
