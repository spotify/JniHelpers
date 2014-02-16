package com.spotify.jni.annotations;

/**
 * Annotation which indicates that a piece of seemingly-unused Java code
 * is actually being referenced by native code via JNI. Since annotations
 * cannot subclass each other, this should be used in combination with
 * {@link java.lang.SuppressWarnings()}.
 */
public @interface UsedByNativeCode {}
