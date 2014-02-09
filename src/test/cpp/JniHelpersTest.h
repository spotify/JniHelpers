#include "JniHelpers.h"
#include "JniHelpersTestInit.h"

using namespace spotify::jni;

class JniHelpersTest : public JniClassWrapper {
public:
  JniHelpersTest() : JniClassWrapper() {}
  JniHelpersTest(JNIEnv *env) : JniClassWrapper(env) { initialize(env); }
  ~JniHelpersTest() {}

  void initialize(JNIEnv *env);
  const char* getPackageName() const { return "com/spotify/jnihelpers"; }
  const char* getSimpleName() const { return "JniHelpersTest"; }

  void setJavaObject(JNIEnv *env, jobject javaObject);
  jobject toJavaObject(JniClassWrapper *nativeObject);

private:
  static void createClassWrapper(JNIEnv *env, jobject object);
};