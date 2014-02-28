package com.spotify.jni;

import org.junit.Ignore;
import org.junit.Test;

import static org.junit.Assert.assertEquals;

public class JavaStringTest {
    static {
        System.loadLibrary("JniHelpersTest");
    }

    @Test
    native public void createJavaString() throws Exception;

    native public void nativeCreateJavaStringFromJavaString(String s);


    @Test
    public void createJavaStringFromJavaString() throws Exception {
        nativeCreateJavaStringFromJavaString(TestConstants.TEST_STRING);
    }


    native public String nativeGetJavaString();

    @Test
    public void getJavaString() throws Exception {
        String result = nativeGetJavaString();
        assertEquals(TestConstants.TEST_STRING, result);
    }

    native public void nativeSetValue(String s);

    @Test
    public void setValue() throws Exception {
        nativeSetValue(TestConstants.TEST_STRING);
    }
}
