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

    @Test(expected = RuntimeException.class)
    native public void addNullItem() throws Exception;

    @Ignore
    @Test(expected = RuntimeException.class)
    native public void addItemWithEmptyName() throws Exception;

    @Ignore
    @Test(expected = RuntimeException.class)
    native public void addItemWithoutJavaClass() throws Exception;

    @Test
    native public void addItemMultipleTimes() throws Exception;
}
