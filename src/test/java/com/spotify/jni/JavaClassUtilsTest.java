package com.spotify.jni;

import org.junit.Test;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertNotNull;

public class JavaClassUtilsTest {
    static {
        System.loadLibrary("JniHelpersTest");
    }

    public native Class findObjectClass();

    @Test
    public void findClass() throws Exception {
        Class result = findObjectClass();
        assertNotNull(result);
        assertEquals(Object.class.getCanonicalName(), result.getCanonicalName());
    }

    @Test(expected = NoClassDefFoundError.class)
    public native void findInvalidClass() throws Exception;

    public native Class nativeFindClassWithLoader();

    @Test
    public void findClassWithLoader() throws Exception {
        Class result = nativeFindClassWithLoader();
        assertNotNull(result);
        assertEquals(this.getClass().getCanonicalName(), result.getCanonicalName());
    }

    @Test(expected = NoClassDefFoundError.class)
    public native void findInvalidClassWithLoader() throws Exception;

    @Test(expected = IllegalArgumentException.class)
    public native void makeNameForSignatureWithNull() throws Exception;

    @Test
    public native void makeNameForSignatureWithPrimitive() throws Exception;

    @Test
    public native void makeNameForSignatureWithArray() throws Exception;

    @Test
    public native void makeNameForSignatureWithObject() throws Exception;

    @Test
    public native void makeNameForSignatureWithJniSignature() throws Exception;

    @Test
    public native void makeNameForSignatureWithArrayOfObjects() throws Exception;
}
