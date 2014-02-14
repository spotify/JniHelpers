package com.spotify.jnihelpers;

import org.junit.Test;

public class JniHelpersTest {
    static {
        System.loadLibrary("JniHelpers");
        System.loadLibrary("JniHelpersTest");
    }

    @Test
    public native void createNewInstance() throws Exception;
}
