/*
 * Copyright (c) 2014 Spotify AB
 *
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 */

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

#define _JUNIT_ASSERT_FALSE(_RESULT, _FILE, _LINE) { \
  if (_RESULT) { \
    JavaExceptionUtils::throwExceptionOfType(env, kTypeJavaAssertion, \
      "expected false but was true (at %s:%d)", _FILE, _LINE); \
  } \
}
#define JUNIT_ASSERT_FALSE(_R) _JUNIT_ASSERT_FALSE(_R, __FILE__, __LINE__)

#define JUNIT_ASSERT_EQUALS_BOOL(_E, _R) _JUNIT_ASSERT_TRUE(_E &&_R, __FILE__, __LINE__)

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

#define _JUNIT_ASSERT_EQUALS_ARRAY(_EXPECTED, _RESULT, _SIZE, _FILE, _LINE) { \
  if (memcmp(_EXPECTED, _RESULT, _SIZE) != 0) { \
    JavaExceptionUtils::throwExceptionOfType(env, kTypeJavaAssertion, \
      "(at %s:%d)", _FILE, _LINE); \
  } \
}
#define JUNIT_ASSERT_EQUALS_ARRAY(_E, _R, _S) _JUNIT_ASSERT_EQUALS_ARRAY(_E, _R, _S, __FILE__, __LINE__)

#define _JUNIT_ASSERT_EQUALS_JAVA_STRING_ARRAY(_EXPECTED, _RESULT, _SIZE, _FILE, _LINE) { \
  for (size_t i = 0; i < _SIZE; i++) { \
    if (_EXPECTED[i]->get() != _RESULT[i]->get()) { \
      JavaExceptionUtils::throwExceptionOfType(env, kTypeJavaAssertion, \
        "expected <%s> but was: <%s> (at %s:%d)", _EXPECTED[i]->get().c_str(), _RESULT[i]->get().c_str(), _FILE, _LINE); \
    } \
  } \
}
#define JUNIT_ASSERT_EQUALS_JAVA_STRING_ARRAY(_E, _R, _S) _JUNIT_ASSERT_EQUALS_JAVA_STRING_ARRAY(_E, _R, _S, __FILE__, __LINE__)

#endif // __JUnitUtils_h__
