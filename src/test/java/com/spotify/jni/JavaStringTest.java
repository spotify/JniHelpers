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

import org.junit.Ignore;
import org.junit.Test;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertTrue;

public class JavaStringTest {
    static {
        System.loadLibrary("JniHelpersTest");
    }

    native static public boolean supportsRawStringLiterals();

    @Test
    native public void createJavaString() throws Exception;

    native public void nativeCreateJavaStringFromJavaString(String s);
    @Test
    native public void createJavaStringFromStdString() throws Exception;

    @Test
    public void createJavaStringFromJavaString() throws Exception {
        nativeCreateJavaStringFromJavaString(TestConstants.TEST_STRING);
    }

    native public String nativeGetJavaString();
    @Test
    public void getJavaString() throws Exception {
        String result = nativeGetJavaString();
        assertEquals(TestConstants.TEST_STRING, result);
    }

    native public String nativeGetJavaStringWithNullChar();
    @Test
    public void getJavaStringTruncatesAtNullChar() throws Exception {
        assertTrue(TestConstants.TEST_STRING_WITH_NULL_CHAR.contains(TestConstants.TEST_NULL_CHAR));
        String result = nativeGetJavaStringWithNullChar();
        String expected = TestConstants.TEST_STRING_WITH_NULL_CHAR.substring(0,
                TestConstants.TEST_STRING_WITH_NULL_CHAR.indexOf(TestConstants.TEST_NULL_CHAR));
        assertEquals(expected, result);
    }

    native public String nativeGetJavaStringUtf16();
    @Ignore("Not currently supported")
    @Test
    public void getJavaStringUtf16() throws Exception {
        String result = nativeGetJavaStringUtf16();
        assertEquals(TestConstants.TEST_UTF16_STRING, result);
    }

    native public String nativeGetJavaStringUtf8();
    @Test
    public void getJavaStringUtf8() throws Exception {
        if (JavaStringTest.supportsRawStringLiterals()) {
            String result = nativeGetJavaStringUtf8();
            assertEquals(TestConstants.TEST_UTF8_STRING, result);
        }
    }

    native public void nativeSetValue(String s);
    @Test
    public void setValue() throws Exception {
        nativeSetValue(TestConstants.TEST_STRING);
    }

    @Test
    native public void nativeSetValueWithOperator() throws Exception;

    native public String nativeSetAndReturnValue(String s);
    @Test
    public void setValueWithNullChar() throws Exception {
        String result = nativeSetAndReturnValue(TestConstants.TEST_STRING_WITH_NULL_CHAR);
        assertEquals(TestConstants.TEST_STRING_WITH_NULL_CHAR, result);
    }
}
