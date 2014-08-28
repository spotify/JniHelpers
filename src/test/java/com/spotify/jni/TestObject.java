/*
 * Copyright (c) 2014 Spotify AB
 *
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 */

package com.spotify.jni;

public class TestObject {
    String string;
    int i;
    short s;
    float f;
    double d;
    boolean z;
    byte b;
    char c;

    public TestObject() {
        // TODO: Need to deal with classes that don't have a no-arg ctor
    }

    private TestObject(String string, int i, short s, float f, double d, boolean z, byte b, char c) {
        this.string = string;
        this.i = i;
        this.s = s;
        this.f = f;
        this.d = d;
        this.z = z;
        this.b = b;
        this.c = c;
    }

    public static TestObject createTestObject() {
        return new TestObject(TestConstants.TEST_STRING,
                TestConstants.TEST_INTEGER,
                TestConstants.TEST_SHORT,
                TestConstants.TEST_FLOAT,
                TestConstants.TEST_DOUBLE,
                TestConstants.TEST_BOOLEAN,
                TestConstants.TEST_BYTE,
                TestConstants.TEST_CHAR
        );
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

    public boolean getZ() {
        return z;
    }

    public void setZ(boolean z) {
        this.z = z;
    }

    public byte getB() {
        return b;
    }

    public void setB(byte b) {
        this.b = b;
    }

    public char getC() {
        return c;
    }

    public void setC(char c) {
        this.c = c;
    }
}
