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
    native public void createJavaStringFromStdString() throws Exception;


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


    native public String nativeGetJavaStringUtf16();
    native public String nativeGetJavaStringUtf8();

    @Ignore
    @Test
    public void getJavaStringUtf16() throws Exception {
        String result = nativeGetJavaStringUtf16();
        assertEquals(TestConstants.TEST_UTF16_STRING, result);
    }

    @Test
    public void getJavaStringUtf8() throws Exception {
        String result = nativeGetJavaStringUtf8();
        assertEquals(TestConstants.TEST_UTF8_STRING, result);
    }


    native public void nativeSetValue(String s);

    @Test
    public void setValue() throws Exception {
        nativeSetValue(TestConstants.TEST_STRING);
    }
}
