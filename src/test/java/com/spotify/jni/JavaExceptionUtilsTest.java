package com.spotify.jni;

import org.junit.Test;

import static org.junit.Assert.assertEquals;

public class JavaExceptionUtilsTest {
    static {
        System.loadLibrary("JniHelpersTest");
    }

    // TODO: Need to test checkException, not sure how

    public native void nativeThrowException() throws Exception;

    @Test(expected = Exception.class)
    public void throwException() throws Exception {
        try {
            nativeThrowException();
        } catch (Exception e) {
            assertEquals(TestConstants.TEST_STRING, e.getMessage());
            throw e;
        }
    }

    public native void nativeThrowExceptionWithFormatString() throws Exception;

    @Test
    public void testThrowExceptionWithFormatString() throws Exception {
        try {
            nativeThrowExceptionWithFormatString();
        } catch (Exception e) {
            String expected = TestConstants.TEST_STRING + ", " + TestConstants.TEST_INTEGER;
            assertEquals(expected, e.getMessage());
        }
    }

    public native void nativeThrowRuntimeException() throws Exception;

    @Test(expected = RuntimeException.class)
    public void throwRuntimeException() throws Exception {
        try {
            nativeThrowRuntimeException();
        } catch (RuntimeException e) {
            assertEquals(TestConstants.TEST_STRING, e.getMessage());
            throw e;
        }
    }

    public native void nativeThrowExceptionOfType() throws Exception;

    @Test(expected = UnsupportedOperationException.class)
    public void throwExceptionOfType() throws Exception {
        try {
            nativeThrowExceptionOfType();
        } catch (UnsupportedOperationException e) {
            assertEquals(TestConstants.TEST_STRING, e.getMessage());
            throw e;
        }
    }
}
