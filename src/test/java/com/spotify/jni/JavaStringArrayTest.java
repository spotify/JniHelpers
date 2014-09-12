package com.spotify.jni;

import org.junit.Test;

import static org.junit.Assert.assertEquals;

public class JavaStringArrayTest {

    static {
        System.loadLibrary("JniHelpersTest");
    }

    private String[] getTestJavaStringArray() {
        return new String[] {"abc", "def", "ghi", "ążń"};
    }

    @Test
    public native void createNewJavaStringArray() throws Exception;

    @Test
    public native void createNewJavaStringArrayWithData() throws Exception;

    @Test
    public native void createNewJavaStringArrayWithDataCopy() throws Exception;

    @Test
    public void createNewJavaStringArrayWithJavaData() throws Exception {
        nativeCreateNewJavaStringArrayWithJavaData(getTestJavaStringArray());
    }

    private native void nativeCreateNewJavaStringArrayWithJavaData(String[] elements);

    @Test
    public native void createNewJavaStringArrayWithNull() throws Exception;

    @Test
    public native void createNewJavaStringArrayWithNullAndNonZeroLength() throws Exception;

    public native String[] nativeGetTestJavaStringArray();

    @Test
    public void getJavaStringArray() throws Exception {
        String[] result = nativeGetTestJavaStringArray();
        String[] expected = getTestJavaStringArray();
        assertEquals(expected.length, result.length);
        for (int i = 0; i < result.length; i++) {
            assertEquals(expected[i], result[i]);
        }
    }

    @Test
    public native void setData() throws Exception;

    @Test
    public native void setDataWithCopy() throws Exception;

    public native void nativeSetJavaStringArray(String[] data, int expectedSize);

    @Test
    public void setJavaData() throws Exception {
        String[] expected = getTestJavaStringArray();
        nativeSetJavaStringArray(expected, expected.length);
    }

    @Test
    public void setJavaDataWithNull() throws Exception {
        nativeSetJavaStringArray(null, 0);
    }
}
