package com.spotify.jni;

import org.junit.Test;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertFalse;
import static org.junit.Assert.assertNotEquals;

public class ClassWrapperTest {
    static {
        System.loadLibrary("JniHelpersTest");
    }

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

    native public void destroyPersistedObject(PersistedObject object);

    @Test
    public void persist() throws Exception {
        PersistedObject object = createPersistedObject();
        assertNotEquals(0, object.nPtr);
        // These properties should be set by the first native method in this case
        assertEquals(TestConstants.TEST_INTEGER, object.i);

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
        destroyPersistedObject(object);
    }

    @Test
    native public void persistInvalidClass() throws Exception;

    @Test(expected = IllegalArgumentException.class)
    native public void persistNullObject() throws Exception;

    @Test
    public void destroyPersistedObject() throws Exception {
        PersistedObject object = createPersistedObject();
        assertNotEquals(0, object.nPtr);
        assertEquals(TestConstants.TEST_INTEGER, object.i);

        destroyPersistedObject(object);

        assertEquals(0, object.nPtr);
        // Destroy should only alter the nPtr field, this should remain untouched
        assertEquals(TestConstants.TEST_INTEGER, object.i);
    }

    @Test
    native public void destroyInvalidClass() throws Exception;

    @Test(expected = IllegalArgumentException.class)
    native public void destroyNullObject() throws Exception;

    native public void nativeSetJavaObject(TestObject object);

    @Test
    public void setJavaObject() throws Exception {
        TestObject object = TestObject.createTestObject();
        nativeSetJavaObject(object);
    }

    native public TestObject nativeToJavaObject();

    @Test
    public void toJavaObject() throws Exception {
        TestObject object = nativeToJavaObject();
        assertEquals(TestConstants.TEST_STRING, object.string);
        assertEquals(TestConstants.TEST_INTEGER, object.i);
        assertEquals(TestConstants.TEST_SHORT, object.s);
        assertEquals(TestConstants.TEST_FLOAT, object.f, TestConstants.TEST_FLOAT_TOLERANCE);
        assertEquals(TestConstants.TEST_DOUBLE, object.d, TestConstants.TEST_FLOAT_TOLERANCE);
        assertEquals(TestConstants.TEST_BOOLEAN, object.z);
        assertEquals(TestConstants.TEST_BYTE, object.b);
    }

    @Test
    native public void getCachedMethod() throws Exception;

    @Test(expected = IllegalArgumentException.class)
    native public void getInvalidCachedMethod() throws Exception;

    @Test(expected = IllegalStateException.class)
    native public void getCachedMethodOnUninitialized() throws Exception;

    @Test
    native public void getCachedField() throws Exception;

    @Test(expected = IllegalArgumentException.class)
    native public void getInvalidCachedField() throws Exception;

    @Test(expected = IllegalStateException.class)
    native public void getCachedFieldOnUninitialized() throws Exception;
}
