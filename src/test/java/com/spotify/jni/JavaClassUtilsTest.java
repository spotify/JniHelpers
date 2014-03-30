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

    public native Class nativeFindClassWithLoader();

    @Test
    public void findClassWithLoader() throws Exception {
        Class result = nativeFindClassWithLoader();
        assertNotNull(result);
        assertEquals(this.getClass().getCanonicalName(), result.getCanonicalName());
    }
}
