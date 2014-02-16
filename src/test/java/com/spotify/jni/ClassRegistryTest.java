package com.spotify.jni;

import com.spotify.jni.util.Success;
import org.junit.Rule;
import org.junit.Test;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.fail;

public class ClassRegistryTest {
    static {
        System.loadLibrary("JniHelpersTest");
    }

    @Test(expected = Success.class)
    native public void addClass() throws Exception;

    @Test(expected = RuntimeException.class)
    native public void addNullItem() throws Exception;

    @Test(expected = RuntimeException.class)
    native public void addItemWithEmptyName() throws Exception;

    @Test(expected = RuntimeException.class)
    native public void addItemWithoutJavaClass() throws Exception;

    @Test(expected = Success.class)
    native public void addItemMultipleTimes() throws Exception;
}
