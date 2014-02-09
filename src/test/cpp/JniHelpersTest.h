#include "JniHelpers.h"

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *jvm, void *reserved);

using namespace spotify::jni;

class JniHelperTest : public JniClassWrapper {
public:
  JniHelperTest(JNIEnv *env);
  ~JniHelperTest() {}

  void initialize(JNIEnv *env);
  const char* getPackageName() const { return "com/spotify/jnihelpers"; }
  const char* getSimpleName() const { return "JniHelpersTest"; }

  JniClassWrapper* fromJavaObject(JNIEnv *env, jobject javaObject) const;
  jobject toJavaObject(JniClassWrapper *nativeObject);
};