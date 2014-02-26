#ifndef __PersistedObject_h__
#define __PersistedObject_h__

#include "JniHelpers.h"
#include "JniHelpersTest.h"

using namespace spotify::jni;

class PersistedObject : public ClassWrapper {
public:
  PersistedObject();
  PersistedObject(JNIEnv *env);

  const char* getCanonicalName() const {
    return MAKE_CANONICAL_NAME(PACKAGE, PersistedObject);
  }

  void initialize(JNIEnv *env);
  void mapFields();

public:
  int i;
};

#endif // __PersistedObject_h__
