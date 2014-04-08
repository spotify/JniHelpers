#ifndef __PersistedObject_h__
#define __PersistedObject_h__

#include "JniHelpers.h"
#include "JniHelpersTest.h"

using namespace spotify::jni;

/**
 * @brief Example of a natively persisted class
 *
 * Instances of this class can be created on the heap and then leaked, the
 * address of the leaked instance is stored in a corresponding Java object
 * in a long field. When you want to access the native instance again, it
 * is provided automatically when invoking ClassRegistry::newInstance.
 *
 * In order for JniHelpers to correctly recognize a class as being persisted,
 * you must call enablePersistence() in initialize().
 */
class PersistedObject : public NativeObject {
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
