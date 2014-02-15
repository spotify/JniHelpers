package com.spotify.jni;

import org.junit.Test;

public class JniHelpersTest {
    static {
        System.loadLibrary("JniHelpersTest");
    }

    @Test
    public native void createNewInstance() throws Exception;
}
