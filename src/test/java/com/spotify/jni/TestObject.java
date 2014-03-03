package com.spotify.jni;

public class TestObject {
    String s;
    int i;
    float f;
    double d;
    byte[] b;

    public TestObject() {
        // TODO: Need to deal with classes that don't have a no-arg ctor
    }

    public TestObject(String s, int i, float f, double d) {
        this.s = s;
        this.i = i;
        this.f = f;
        this.d = d;
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

    public double getD() {
        return d;
    }

    public void setD(double d) {
        this.d = d;
    }

    public byte[] getB() {
        return b;
    }

    public void setB(byte[] b) {
        this.b = b;
    }
}
