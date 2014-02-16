package com.spotify.jni;

public class TestObject {
    String s;
    int i;
    float f;
    byte[] b;

    public TestObject(String s, int i, float f) {
        this.s = s;
        this.i = i;
        this.f = f;
    }

    public String getS() {
        return s;
    }

    public void setS(String s) {
        this.s = s;
    }

    public int getI() {
        return i;
    }

    public void setI(int i) {
        this.i = i;
    }

    public float getF() {
        return f;
    }

    public void setF(float f) {
        this.f = f;
    }

    public byte[] getB() {
        return b;
    }

    public void setB(byte[] b) {
        this.b = b;
    }
}
