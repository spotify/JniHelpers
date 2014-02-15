package com.spotify.jni;

/**
 * If you choose to build JniHelpers as a shared library, then this class can
 * serve as a static loader for your project.
 */
final class JniHelpersLoader {
    static {
        System.loadLibrary("JniHelpers");
    }

    private JniHelpersLoader() {
        throw new UnsupportedOperationException("Instantiation of JniHelpersLoader is not allowed");
    }
}