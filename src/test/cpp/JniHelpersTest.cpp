#include "JniHelpersTest.h"

static const char *kPackageName = "com/spotify/jnihelpers";
static const char *kClassName = "JniHelpersTest";

void JniHelpersTest::initialize(JNIEnv *env) {
  setClass(env);
  addNativeMethod("createNewInstance", (void*)&JniHelpersTest::createNewInstance,
    kTypeVoid, NULL);
  registerNativeMethods(env);
}

const char* JniHelpersTest::getPackageName() const {
  return kPackageName;
}

const char* JniHelpersTest::getSimpleName() const {
  return kClassName;
}

void JniHelpersTest::setJavaObject(JNIEnv *env, jobject javaObject) {
}

jobject JniHelpersTest::toJavaObject(ClassWrapper *nativeObject) {
  return NULL;
}

void JniHelpersTest::createNewInstance(JNIEnv *env, jobject object) {
  if (false) {
    JavaExceptionUtils::throwRuntimeException(env, "Foo");
    return;
  }

  JniHelpersTest *instance = gClasses.newInstance<JniHelpersTest>(env, object);
  if (instance == NULL) {
    JavaExceptionUtils::throwRuntimeException(env, "Could not create new instance of class");
    return;
  }
  if (instance->_clazz == NULL) {
    JavaExceptionUtils::throwRuntimeException(env, "Cached class was null");
    return;
  }

  delete instance;
}
