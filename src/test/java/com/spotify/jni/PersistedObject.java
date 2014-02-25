package com.spotify.jni;

public class PersistedObject extends NativeObject {
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
