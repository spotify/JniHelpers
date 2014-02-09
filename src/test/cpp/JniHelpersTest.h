#include "JniHelpers.h"
#include "JniHelpersTestInit.h"

using namespace spotify::jni;

class JniHelpersTest : public ClassWrapper {
public:
  JniHelpersTest() : ClassWrapper() {}
  JniHelpersTest(JNIEnv *env) : ClassWrapper(env) { initialize(env); }
  ~JniHelpersTest() {}

  void initialize(JNIEnv *env);
  const char* getPackageName() const { return "com/spotify/jnihelpers"; }
  const char* getSimpleName() const { return "JniHelpersTest"; }

  void setJavaObject(JNIEnv *env, jobject javaObject);
  jobject toJavaObject(ClassWrapper *nativeObject);

private:
  static void createClassWrapper(JNIEnv *env, jobject object);
};