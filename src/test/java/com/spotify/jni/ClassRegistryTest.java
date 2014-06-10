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

    @Test
    native public void getWithBracketOperator() throws Exception;

    @Test(expected = IllegalArgumentException.class)
    native public void getNullClass() throws Exception;

    @Test
    native public void getInvalidClass() throws Exception;

    native public void nativeNewInstance(TestObject instance);

    @Test
    public void newInstance() throws Exception {
        TestObject testObject = TestObject.createTestObject();
        nativeNewInstance(testObject);
    }

    native public void nativeNewInstanceWithNull(TestObject instance);

    @Test
    public void newInstanceWithNull() throws Exception {
        nativeNewInstanceWithNull(null);
    }
}
