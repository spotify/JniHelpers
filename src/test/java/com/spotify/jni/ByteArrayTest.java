package com.spotify.jni;

import org.junit.Test;

import static org.junit.Assert.assertEquals;

public class ByteArrayTest {
    static {
        System.loadLibrary("JniHelpersTest");
    }

    private byte[] getTestByteArray() {
        byte[] data = new byte[10];
        for (int i = 0; i < data.length; i++) {
            data[i] = (byte) i;
        }
        return data;
    }

    @Test
    public native void createNewByteArray() throws Exception;

    @Test
    public native void createNewByteArrayWithData() throws Exception;

    public native void nativeCreateNewByteArrayWithJavaData(byte[] data);

    @Test
    public void createNewByteArrayWithJavaData() throws Exception {
        nativeCreateNewByteArrayWithJavaData(getTestByteArray());
    }

    @Test
    public native void createNewByteArrayWithNull() throws Exception;

    @Test
    public native void createNewByteArrayWithNullAndNonZeroLength() throws Exception;

    public native byte[] nativeGetTestJavaByteArray();

    @Test
    public void getJavaByteArray() throws Exception {
        byte[] result = nativeGetTestJavaByteArray();
        byte[] expected = getTestByteArray();
        assertEquals(expected.length, result.length);
        for (int i = 0; i < result.length; i++) {
            assertEquals(expected[i], result[i]);
        }
    }

    @Test
    public native void setData() throws Exception;

    @Test
    public native void setDataWithCopy() throws Exception;

    public native void nativeSetJavaByteArray(byte[] data, int expectedSize);

    @Test
    public void setJavaData() throws Exception {
        byte[] expected = getTestByteArray();
        nativeSetJavaByteArray(expected, expected.length);
    }

    @Test
    public void setJavaDataWithNull() throws Exception {
        nativeSetJavaByteArray(null, 0);
    }
}