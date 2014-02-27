/**
 * This file contains various macros to help testing native code with
 * JUnit test runner. Even though the pure C++ code could be tested with
 * a C++ unit test runner, it's easier to just run all the tests from
 * Java in an IDE rather than having two test suites.
 */

#ifndef __JUnitUtils_h__
#define __JUnitUtils_h__

#include "JniHelpers.h"
#include "TestConstants.h"
#include <math.h>
#include <string.h>

using namespace spotify::jni;

#define kTypeJavaAssertion kTypeJavaClass(AssertionError)
#define DEFAULT_FLOAT_TOLERANCE 0.01f

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
      "expected <object> but was <null> (at %s:%d)", _FILE, _LINE); \
  } \
}
#define JUNIT_ASSERT_NOT_NULL(_R) _JUNIT_ASSERT_NOT_NULL(_R, __FILE__, __LINE__)

#define _JUNIT_ASSERT_NULL(_RESULT, _FILE, _LINE) { \
  if (_RESULT != NULL) { \
    JavaExceptionUtils::throwExceptionOfType(env, kTypeJavaAssertion, \
      "expected <null> but was <object> (at %s:%d)", _FILE, _LINE); \
  } \
}
#define JUNIT_ASSERT_NULL(_R) _JUNIT_ASSERT_NULL(_R, __FILE__, __LINE__)

#define _JUNIT_ASSERT_EQUALS_INT(_EXPECTED, _RESULT, _FILE, _LINE) { \
  if (_EXPECTED != _RESULT) { \
    JavaExceptionUtils::throwExceptionOfType(env, kTypeJavaAssertion, \
      "expected %d but was: %d (at %s:%d)", _EXPECTED, _RESULT, _FILE, _LINE); \
  } \
}
#define JUNIT_ASSERT_EQUALS_INT(_E, _R) _JUNIT_ASSERT_EQUALS_INT(_E, _R, __FILE__, __LINE__)

#define _JUNIT_ASSERT_EQUALS_FLOAT(_EXPECTED, _RESULT, _TOLERANCE, _FILE, _LINE) { \
  if (fabs(_EXPECTED - _RESULT) > _TOLERANCE) { \
    JavaExceptionUtils::throwExceptionOfType(env, kTypeJavaAssertion, \
      "expected %f but was: %f (at %s:%d)", _EXPECTED, _RESULT, _FILE, _LINE); \
  } \
}
#define JUNIT_ASSERT_EQUALS_FLOAT(_E, _R, _T) _JUNIT_ASSERT_EQUALS_FLOAT(_E, _R, _T, __FILE__, __LINE__)

#define _JUNIT_ASSERT_EQUALS_CSTRING(_EXPECTED, _RESULT, _FILE, _LINE) { \
  if (strcmp(_EXPECTED, _RESULT) != 0) { \
    JavaExceptionUtils::throwExceptionOfType(env, kTypeJavaAssertion, \
      "expected <%s> but was: <%s> (at %s:%d)", _EXPECTED, _RESULT, _FILE, _LINE); \
  } \
}
#define JUNIT_ASSERT_EQUALS_CSTRING(_E, _R) _JUNIT_ASSERT_EQUALS_CSTRING(_E, _R, __FILE__, __LINE__)

#define _JUNIT_ASSERT_EQUALS_STRING(_EXPECTED, _RESULT, _FILE, _LINE) { \
  if (strcmp(_EXPECTED, _RESULT.c_str()) != 0) { \
    JavaExceptionUtils::throwExceptionOfType(env, kTypeJavaAssertion, \
      "expected <%s> but was: <%s> (at %s:%d)", _EXPECTED, _RESULT.c_str(), _FILE, _LINE); \
  } \
}
#define JUNIT_ASSERT_EQUALS_STRING(_E, _R) _JUNIT_ASSERT_EQUALS_STRING(_E, _R, __FILE__, __LINE__)

#endif // __JUnitUtils_h__
