package com.spotify.jni;

import org.junit.Test;

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
        TestObject testObject = new TestObject(TestConstants.TEST_STRING,
                TestConstants.TEST_INTEGER, TestConstants.TEST_FLOAT);
        nativeNewInstance(testObject);
    }

    native public void nativeNewInstanceWithNull(TestObject instance);
    @Test
    public void newInstanceWithNull() throws Exception {
        nativeNewInstanceWithNull(null);
    }
}
