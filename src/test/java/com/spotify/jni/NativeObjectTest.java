package com.spotify.jni;

import org.junit.Test;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertNotEquals;

public class NativeObjectTest {
    static {
        System.loadLibrary("JniHelpersTest");
    }

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
    native public void nativeIsPersistenceEnabled() throws Exception;

    @Test
    native public void isPersistenceEnabledWithoutInit() throws Exception;

    @Test
    native public void destroyInvalidClass() throws Exception;

    @Test(expected = IllegalArgumentException.class)
    native public void destroyNullObject() throws Exception;
}
