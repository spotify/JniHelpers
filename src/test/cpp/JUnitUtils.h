/**
 * This file contains various macros to help testing native code with
 * JUnit test runner. Even though the pure C++ code could be tested with
 * a C++ unit test runner, it's easier to just run all the tests from
 * Java in an IDE rather than having two test suites.
 */

#ifndef __JUnitUtils_h__
#define __JUnitUtils_h__

#include "JniHelpers.h"
#include <string.h>

using namespace spotify::jni;

#define kTypeJavaAssertion kTypeJavaClass(AssertionError)

#define _JUNIT_ASSERT_TRUE(_RESULT, _FILE, _LINE) { \
  if (!_RESULT) { \
    JavaExceptionUtils::throwExceptionOfType(env, kTypeJavaAssertion, \
      "expected true but was false (at %s:%d)", _FILE, _LINE); \
  } \
}
#define JUNIT_ASSERT_TRUE(_R) _JUNIT_ASSERT_TRUE(_R, __FILE__, __LINE__)

#define _JUNIT_ASSERT_NOT_NULL(_RESULT, _FILE, _LINE) { \
  if (_RESULT == NULL) { \
    JavaExceptionUtils::throwExceptionOfType(env, kTypeJavaAssertion, \
      "object was null (at %s:%d)", _FILE, _LINE); \
  } \
}
#define JUNIT_ASSERT_NOT_NULL(_R) _JUNIT_ASSERT_NOT_NULL(_R, __FILE__, __LINE__)

#define _JUNIT_ASSERT_EQUALS_INT(_EXPECTED, _RESULT, _FILE, _LINE) { \
  if (_EXPECTED != _RESULT) { \
    JavaExceptionUtils::throwExceptionOfType(env, kTypeJavaAssertion, \
      "expected %d but was: %d (at %s:%d)", _EXPECTED, _RESULT, _FILE, _LINE); \
  } \
}
#define JUNIT_ASSERT_EQUALS_INT(_E, _R) _JUNIT_ASSERT_EQUALS_INT(_E, _R, __FILE__, __LINE__)

#define _JUNIT_ASSERT_EQUALS_STRING(_EXPECTED, _RESULT, _FILE, _LINE) { \
  if (_EXPECTED != _RESULT) { \
    JavaExceptionUtils::throwExceptionOfType(env, kTypeJavaAssertion, \
      "expected <%s> but was: <%s> (at %s:%d)", _EXPECTED, _RESULT, _FILE, _LINE); \
  } \
}
#define JUNIT_ASSERT_EQUALS_STRING(_E, _R) _JUNIT_ASSERT_EQUALS_STRING(_E, _R, __FILE__, __LINE__)

#endif // __JUnitUtils_h__
