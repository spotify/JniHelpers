package com.spotify.jni;

public class TestObject {
    String string;
    int i;
    short s;
    float f;
    double d;
    byte[] b;

    public TestObject() {
        // TODO: Need to deal with classes that don't have a no-arg ctor
    }

    public TestObject(String string, int i, short s, float f, double d) {
        this.string = string;
        this.i = i;
        this.s = s;
        this.f = f;
        this.d = d;
    }

    public String getString() {
        return string;
    }

    public void setString(String string) {
        this.string = string;
    }

    public int getI() {
        return i;
    }

    public void setI(int i) {
        this.i = i;
    }

    public short getS() {
        return s;
    }

    public void setS(short s) {
        this.s = s;
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
