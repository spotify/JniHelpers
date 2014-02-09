#include "JniHelpers.h"

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *jvm, void *reserved);

using namespace spotify::jni;

class JniHelperTest : public JniClassWrapper {
public:
  JniHelperTest(JNIEnv *env);
  ~JniHelperTest() {}

  virtual void initialize(JNIEnv *env);
  virtual const char* getClassName() const;

  virtual JniClassWrapper* fromJavaObject(JNIEnv *env, jobject javaObject) const;
  virtual jobject toJavaObject(JniClassWrapper *nativeObject);
};