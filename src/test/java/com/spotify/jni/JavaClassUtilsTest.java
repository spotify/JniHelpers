/*
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

import org.junit.Test;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertNotNull;

public class JavaClassUtilsTest {
    static {
        System.loadLibrary("JniHelpersTest");
    }

    public native Class findObjectClass();

    @Test
    public void findClass() throws Exception {
        Class result = findObjectClass();
        assertNotNull(result);
        assertEquals(Object.class.getCanonicalName(), result.getCanonicalName());
    }

    @Test(expected = NoClassDefFoundError.class)
    public native void findInvalidClass() throws Exception;

    public native Class nativeFindClassWithLoader();

    @Test
    public void findClassWithLoader() throws Exception {
        Class result = nativeFindClassWithLoader();
        assertNotNull(result);
        assertEquals(this.getClass().getCanonicalName(), result.getCanonicalName());
    }

    @Test(expected = NoClassDefFoundError.class)
    public native void findInvalidClassWithLoader() throws Exception;

    @Test(expected = IllegalArgumentException.class)
    public native void makeNameForSignatureWithNull() throws Exception;

    @Test
    public native void makeNameForSignatureWithPrimitive() throws Exception;

    @Test
    public native void makeNameForSignatureWithArray() throws Exception;

    @Test
    public native void makeNameForSignatureWithObject() throws Exception;

    @Test
    public native void makeNameForSignatureWithJniSignature() throws Exception;

    @Test
    public native void makeNameForSignatureWithArrayOfObjects() throws Exception;
}
