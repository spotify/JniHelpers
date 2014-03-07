package com.spotify.jni;

import org.junit.Ignore;
import org.junit.Test;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertTrue;

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

    native public String nativeGetJavaStringWithNullChar();
    @Test
    public void getJavaStringTruncatesAtNullChar() throws Exception {
        assertTrue(TestConstants.TEST_STRING_WITH_NULL_CHAR.contains(TestConstants.TEST_NULL_CHAR));
        String result = nativeGetJavaStringWithNullChar();
        String expected = TestConstants.TEST_STRING_WITH_NULL_CHAR.substring(0,
                TestConstants.TEST_STRING_WITH_NULL_CHAR.indexOf(TestConstants.TEST_NULL_CHAR));
        assertEquals(expected, result);
    }

    native public String nativeGetJavaStringUtf16();
    @Ignore("Not currently supported")
    @Test
    public void getJavaStringUtf16() throws Exception {
        String result = nativeGetJavaStringUtf16();
        assertEquals(TestConstants.TEST_UTF16_STRING, result);
    }

    native public String nativeGetJavaStringUtf8();
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

    native public String nativeSetAndReturnValue(String s);
    @Test
    public void setValueWithNullChar() throws Exception {
        String result = nativeSetAndReturnValue(TestConstants.TEST_STRING_WITH_NULL_CHAR);
        assertEquals(TestConstants.TEST_STRING_WITH_NULL_CHAR, result);
    }
}
