package com.spotify.jni;

import org.junit.Ignore;
import org.junit.Test;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.fail;

public class ClassRegistryTest {
    static {
        System.loadLibrary("JniHelpersTest");
    }

    @Test
    native public void createRegistry() throws Exception;


    @Test
    native public void addClass() throws Exception;

    @Test(expected = IllegalArgumentException.class)
    native public void addNullClass() throws Exception;

    @Test(expected = IllegalArgumentException.class)
    native public void addClassWithEmptyName() throws Exception;

    @Test(expected = IllegalArgumentException.class)
    native public void addClassWithNullName() throws Exception;

    @Test(expected = IllegalArgumentException.class)
    native public void addClassWithoutInfo() throws Exception;

    @Test
    native public void addClassMultipleTimes() throws Exception;


    @Test
    native public void get() throws Exception;

    @Test(expected = IllegalArgumentException.class)
    native public void getNullClass() throws Exception;

    @Test
    native public void getInvalidClass() throws Exception;


    native public void nativeNewInstance(TestObject instance);
    @Test
    public void newInstance() throws Exception {
        TestObject testObject = new TestObject("hello", 1, 3.14f);
        nativeNewInstance(testObject);
    }

    native public void nativeNewInstanceWithNull(TestObject instance);
    @Ignore
    @Test(expected = IllegalArgumentException.class)
    public void newInstanceWithNull() throws Exception {
        nativeNewInstanceWithNull(null);
    }
}
