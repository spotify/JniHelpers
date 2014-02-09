#include "JniHelpers.h"
#include "JniHelpersTestInit.h"

using namespace spotify::jni;

class JniHelpersTest : public JniClassWrapper {
public:
  JniHelpersTest(JNIEnv *env);
  JniHelpersTest(JNIEnv *env, jobject fromObject);
  ~JniHelpersTest() {}

  void initialize(JNIEnv *env);
  const char* getPackageName() const { return "com/spotify/jnihelpers"; }
  const char* getSimpleName() const { return "JniHelpersTest"; }

  void setJavaObject(JNIEnv *env, jobject javaObject);
  jobject toJavaObject(JniClassWrapper *nativeObject);

private:
  JniHelpersTest() : JniClassWrapper() {}
  static JniHelpersTest* newInstance() {
    JniHelpersTest *instance = new JniHelpersTest();
    instance->merge(gClasses.get(instance->getSimpleName()));
    return instance;
  }
  static void createClassWrapper(JNIEnv *env, jobject object);
};