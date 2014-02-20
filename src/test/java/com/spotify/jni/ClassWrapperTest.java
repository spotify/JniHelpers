package com.spotify.jni;

import org.junit.Ignore;
import org.junit.Test;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertFalse;
import static org.junit.Assert.assertNotEquals;

public class ClassWrapperTest {
    static {
        System.loadLibrary("JniHelpersTest");
    }

    static final double TEST_FLOAT_TOLERANCE = 0.01;

    @Test
    native public void createClassWrapper() throws Exception;

    @Test
    native public void getCanonicalName() throws Exception;

    @Test
    native public void getSimpleName() throws Exception;

    @Test
    native public void merge() throws Exception;

    native public PersistedObject createPersistedObject();
    native public PersistedObject getPersistedInstance(PersistedObject object);
    native public void resetPersistedObject(PersistedObject object);
    @Ignore
    @Test
    public void persist() throws Exception {
        PersistedObject object = createPersistedObject();
        assertNotEquals(0, object.nPtr);
        // These properties should be set by the first native method in this case
        assertEquals(42, object.i);

        // Now create a new empty object, but copy the nPtr field to it. Note that
        // the i field is *not* copied; that value is stored natively and should
        // be retrieved in the call to getPersistedInstance.
        PersistedObject emptyInstance = new PersistedObject();
        emptyInstance.nPtr = object.nPtr;

        // The native test should be able to fetch the previous instance via nPtr,
        // and return to us the same instance data in a new object.
        PersistedObject result = getPersistedInstance(emptyInstance);
        assertEquals(object.nPtr, result.nPtr);
        assertEquals(object.i, result.i);

        // Always clean up after yourself, kids!
        resetPersistedObject(object);
    }

    native public boolean nativePersistInvalidClass(TestObject testObject);
    @Ignore
    @Test
    public void persistInvalidClass() throws Exception {
        TestObject testObject = new TestObject();
        assertFalse(nativePersistInvalidClass(testObject));
    }

    @Ignore
    @Test(expected = IllegalArgumentException.class)
    native public void persistNullObject() throws Exception;

    native public void nativeResetPersistedObject(PersistedObject object);
    @Ignore
    @Test
    public void resetPersistedObject() throws Exception {
        PersistedObject object = createPersistedObject();
        assertNotEquals(0, object.nPtr);
        assertEquals(42, object.i);

        nativeResetPersistedObject(object);

        PersistedObject result = getPersistedInstance(object);
        assertEquals(0, result.nPtr);
        assertEquals(0, result.i);
    }

    native public boolean nativeResetInvalidClass(TestObject testObject);
    @Ignore
    @Test
    public void resetInvalidClass() throws Exception {
        TestObject testObject = new TestObject();
        // Nothing should happen, since reset() returns void. However the native
        // code also shouldn't crash.
        nativeResetInvalidClass(testObject);
    }

    @Ignore
    @Test(expected = IllegalArgumentException.class)
    native public void resetNullObject() throws Exception;

    native public void nativeSetJavaObject(TestObject object);
    @Test
    public void setJavaObject() throws Exception {
        TestObject object = new TestObject("hello", 1, 3.14f);
        nativeSetJavaObject(object);
    }

    native public TestObject nativeToJavaObject();
    @Test
    public void toJavaObject() throws Exception {
        TestObject object = nativeToJavaObject();
        assertEquals("hello", object.s);
        assertEquals(1, object.i);
        assertEquals(3.14, object.f, TEST_FLOAT_TOLERANCE);
    }

    @Test
    native public void getCachedMethod() throws Exception;

    @Test
    native public void getCachedField() throws Exception;
}
