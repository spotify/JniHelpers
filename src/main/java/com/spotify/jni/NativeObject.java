package com.spotify.jni;

import com.spotify.jni.annotations.UsedByNativeCode;

public class NativeObject {
    @SuppressWarnings("UnusedDeclaration")
    @UsedByNativeCode
    protected long nPtr;

    public native void destroy();
}
