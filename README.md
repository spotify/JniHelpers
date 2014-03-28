JniHelpers
==========

JniHelpers is a library designed to facilitate writing JNI code with C++.
It provides the following features:

* Registry for cached class info (ie, jclass, jmethodID and jfieldID) lookups
* "Automatic" conversion between Java <--> C++ objects. Note the ironic quotes
  around "automatic", which means:
  - Conversion must be enabled for the given class. This is as a feature, not
    a limitation, and means that classes may be represented by a Java/C++
    counterpart, but you may not necessarily care to copy data from them in
    every JNI call. JniHelpers can do this for you, but you can also do it
    manally if performance is a concern.
  - Conversions are not performed automatically in every native callback.
    However, getting a C++ instance of a Java class or creating a Java
    instance of a C++ class is trivial.
  - JniHelpers prefers to copy memory, thus avoiding tricky issues that arise
    when trying to mix the very different memory management models of the Java
    garbage collector and C++ RAII paradigm.
* Conversion between Java `String` objects and `stl::string`.
* Conversion between Java `byte` arrays and C `void*` arrays.
* Persisting native objects through Java without copying data. Persisted
  objects are created on the heap and leaked; their memory address is stored
  in a corresponding Java instance in a `long` field. If you want to access
  the native instance again, a call is provided to fetch the native instance
  from the field. This is very useful for creating long-lived native objects
  without having to make them singletons.
* Easy registration of native methods. Rather than having to memorize the
  complex rules of creating JNI type signatures, a simple varargs interface is
  provided to do the grunt work for you.
* Conveniences for common JNI functions, including:
  - Lookup Java classes with a given classloader
  - Attach/detach native threads to Java
  - Throw Java exceptions from native code, with printf-style string
    formatting
  - RAII template classes for JNI local/global/weak global references.
* Android-friendly. JniHelpers was designed to work well on Android, and
  although it also runs fine on desktop Java, the CMake configuration is meant
  to be easily adapted for an Android build.
* Tested! There is a test suite provided which is executed from Java that will
  run the native tests and prove communication across JNI works as expected.
* Documented! Extensive class documentation is provided.


Examples
--------

The best examples for JniHelpers is the test suite itself. See the classes
within `src/test/cpp`, they are documented and explain correct usage of the
library.


What JniHelpers is NOT
----------------------

JniHelpers is *not* SWIG. It does not attempt to automatically create wrappers
around native code.

JniHelpers is also not a complete wrapper around JNI. It does not attempt to
completely shield the programmer from all JNI calls. Rather, it just makes the
trickiest parts of JNI a bit friendlier to work with.

