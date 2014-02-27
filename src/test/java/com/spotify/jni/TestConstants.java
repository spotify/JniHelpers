package com.spotify.jni;

/**
 * Common values for tests which pass objects through JNI. Note that this file has
 * a matching counterpart in C++, the values defined here must match the ones there.
 */
public class TestConstants {
    public static final String TEST_STRING = "hello";
    public static final int TEST_INTEGER = 42;
    public static final float TEST_FLOAT = 3.14159f;
    public static final double TEST_FLOAT_TOLERANCE = 0.01;
}
