package com.spotify.jni.util;

import com.spotify.jni.annotations.UsedByNativeCode;

/**
 * Should be thrown at the end of successful native tests in order to
 * show that the test actually passed as expected.
 */
@SuppressWarnings("UnusedDeclaration")
@UsedByNativeCode
public class Success extends Exception {
    public Success(String message) {
        super(message);
    }
}
