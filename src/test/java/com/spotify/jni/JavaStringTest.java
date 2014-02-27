package com.spotify.jni;

import org.junit.Test;

import static org.junit.Assert.assertEquals;

public class JavaStringTest {
    static {
        System.loadLibrary("JniHelpersTest");
    }

    @Test
    native public void createJavaString() throws Exception;

    @Test
    native public void createJavaStringFromJavaString() throws Exception;


    native public String nativeGetJavaString();

    @Test
    public void getJavaString() throws Exception {
        String result = nativeGetJavaString();
        assertEquals("hello", result);
    }

    native public void nativeSetValue(String s);

    @Test
    public void setValue() throws Exception {
        nativeSetValue("hello");
    }
}
