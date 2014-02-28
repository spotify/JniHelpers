#!/bin/sh

if [ $# -ne 3 ]
then
  echo "Usage: $(basename $0) <INPUT_FILE> <OUTPUT_CPP_FILE> <OUTPUT_JAVA_FILE>" >&2
  exit 1
fi

TEST_CONSTANTS_FILE=$1
CPP_TEST_CONSTANTS_FILE=$2
JAVA_TEST_CONSTANTS_FILE=$3

TEST_CONSTANTS_DOC='/**
 * Common values for tests which pass objects through JNI.
 * NB: This file is GENERATED. DO NOT EDIT.
 */'

cat << EOF > ${CPP_TEST_CONSTANTS_FILE}
#ifndef __TestConstants_h__
#define __TestConstants_h__

${TEST_CONSTANTS_DOC}

EOF

cat << EOF > ${JAVA_TEST_CONSTANTS_FILE}
package com.spotify.jni;

${TEST_CONSTANTS_DOC}
public class TestConstants {
EOF

awk -F' @ ' '
/^[[:space:]]*[^#]/ && $0 !~/^[[:space:]]*$/ {
  java_type = $1;
  name = $2;
  value = $3;
  print "#define "name" "value >> "'${CPP_TEST_CONSTANTS_FILE}'";
  print "    public static final "java_type" "name" = "value";" >> "'${JAVA_TEST_CONSTANTS_FILE}'";
}
' ${TEST_CONSTANTS_FILE}

cat << EOF >> ${CPP_TEST_CONSTANTS_FILE}

#endif // __TestConstants_h__
EOF

cat << EOF >> ${JAVA_TEST_CONSTANTS_FILE}
}
EOF
