package com.spotify.jni;

import org.junit.Test;

import static org.junit.Assert.assertEquals;

public class JavaClassTest {
    static {
        System.loadLibrary("JniHelpersTest");
    }

    @Test
    native public void createJavaClass() throws Exception;

    @Test
    native public void isInitialized() throws Exception;

    @Test
    native public void getCanonicalName() throws Exception;

    @Test
    native public void getSimpleName() throws Exception;

    @Test
    native public void merge() throws Exception;

    native public void nativeSetJavaObject(TestObject object);

    @Test
    public void setJavaObject() throws Exception {
        TestObject object = TestObject.createTestObject();
        nativeSetJavaObject(object);
    }

    native public TestObject nativeToJavaObject();

    @Test
    public void toJavaObject() throws Exception {
        TestObject object = nativeToJavaObject();
        assertEquals(TestConstants.TEST_STRING, object.string);
        assertEquals(TestConstants.TEST_INTEGER, object.i);
        assertEquals(TestConstants.TEST_SHORT, object.s);
        assertEquals(TestConstants.TEST_FLOAT, object.f, TestConstants.TEST_FLOAT_TOLERANCE);
        assertEquals(TestConstants.TEST_DOUBLE, object.d, TestConstants.TEST_FLOAT_TOLERANCE);
        assertEquals(TestConstants.TEST_BOOLEAN, object.z);
        assertEquals(TestConstants.TEST_BYTE, object.b);

        if (JavaStringTest.supportsRawStringLiterals()) {
            assertEquals(TestConstants.TEST_CHAR, object.c);
        }
    }

    @Test
    native public void getCachedMethod() throws Exception;

    @Test(expected = IllegalArgumentException.class)
    native public void getInvalidCachedMethod() throws Exception;

    @Test(expected = IllegalStateException.class)
    native public void getCachedMethodOnUninitialized() throws Exception;

    @Test
    native public void getCachedField() throws Exception;

    @Test(expected = IllegalArgumentException.class)
    native public void getInvalidCachedField() throws Exception;

    @Test(expected = IllegalStateException.class)
    native public void getCachedFieldOnUninitialized() throws Exception;

    @Test(expected = NoSuchFieldError.class)
    native public void cacheInvalidField() throws Exception;

    @Test(expected = NoSuchMethodError.class)
    native public void cacheInvalidMethod() throws Exception;
}
