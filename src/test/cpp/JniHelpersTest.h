#include "JniHelpers.h"
#include "JniHelpersTestInit.h"

using namespace spotify::jni;

class JniHelpersTest : public ClassWrapper {
public:
  JniHelpersTest() : ClassWrapper() {}
  JniHelpersTest(JNIEnv *env) : ClassWrapper(env) { initialize(env); }
  ~JniHelpersTest() {}

  void initialize(JNIEnv *env);
  const char* getPackageName() const;
  const char* getSimpleName() const;

  void setJavaObject(JNIEnv *env, jobject javaObject);
  jobject toJavaObject(ClassWrapper *nativeObject);

private:
  static void createNewInstance(JNIEnv *env, jobject object);
};
