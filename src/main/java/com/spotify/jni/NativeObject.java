package com.spotify.jni;

import com.spotify.jni.annotations.UsedByNativeCode;

public class NativeObject {
    @SuppressWarnings("UnusedDeclaration")
    @UsedByNativeCode
    protected long nPtr;

    @SuppressWarnings("UnusedDeclaration")
    @UsedByNativeCode
    public native void destroy();
}
