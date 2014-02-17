package com.spotify.jni;

import org.junit.Test;

import static org.junit.Assert.assertEquals;

public class ClassWrapperTest {
    static {
        System.loadLibrary("JniHelpersTest");
    }

    static final double TEST_FLOAT_TOLERANCE = 0.01;

    @Test
    native public void createClassWrapper() throws Exception;

    @Test
    native public void getCanonicalName() throws Exception;

    @Test
    native public void getSimpleName() throws Exception;

    @Test
    native public void merge() throws Exception;

    native public void nativeSetJavaObject(TestObject object);
    @Test
    public void setJavaObject() throws Exception {
        TestObject object = new TestObject("hello", 1, 3.14f);
        nativeSetJavaObject(object);
    }

    native public TestObject nativeToJavaObject();
    @Test
    public void toJavaObject() throws Exception {
        TestObject object = nativeToJavaObject();
        assertEquals("hello", object.s);
        assertEquals(1, object.i);
        assertEquals(3.14, object.f, TEST_FLOAT_TOLERANCE);
    }

    @Test
    native public void getCachedMethod() throws Exception;

    @Test
    native public void getCachedField() throws Exception;
}
