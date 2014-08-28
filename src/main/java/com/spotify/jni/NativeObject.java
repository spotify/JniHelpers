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

import com.spotify.jni.annotations.UsedByNativeCode;

public abstract class NativeObject {
    /**
     * Field used to store a pointer to the native instance allocated on the heap.
     * Don't modify this value directly, or else you risk causing segfaults or
     * leaking memory.
     */
    @SuppressWarnings("UnusedDeclaration")
    @UsedByNativeCode
    protected long nPtr;

    /**
     * This method is used to invoke C++ destructors and free native resources,
     * and normally you want to declare it as <code>native</code> in your subclass.
     * The native implementation should, in turn, simply invoke the base class'
     * <code>destroy()</code> method. For more information, see the documentation
     * in the NativeObject.h header file.
     */
    public abstract void destroy();
}
