package com.spotify.jni;

import com.spotify.jni.annotations.UsedByNativeCode;

public class PersistedObject {
    @UsedByNativeCode
    @SuppressWarnings("UnusedDeclaration")
    long nPtr;
    int i;

    public PersistedObject() {
    }

    public int getI() {
        return i;
    }

    public void setI(int i) {
        this.i = i;
    }
}
