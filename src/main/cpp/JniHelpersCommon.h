#ifndef __JniHelpersCommon_h__
#define __JniHelpersCommon_h__

// Disable some annoying compiler warnings
#if WIN32
#pragma warning(disable: 4996) // "Security" warnings for vsnprintf
#endif

#if WIN32
#define EXPORT __declspec(dllexport)
#else
#define EXPORT
#endif

#include <jni.h>

#define kTypeInt "I"
#define kTypeLong "J"
#define kTypeFloat "F"
#define kTypeDouble "D"
#define kTypeBool "Z"
#define kTypeByte "B"
#define kTypeVoid "V"
#define kTypeString "Ljava/lang/String;"
#define kTypeClass(x) "L" #x ";"
#define kTypeJavaClass(x) "Ljava/lang/" #x ";"

// Exceptions
#define kTypeException kTypeJavaClass(Exception)
#define kTypeRuntimeException kTypeJavaClass(RuntimeException)
#define kTypeIllegalArgumentException kTypeJavaClass(IllegalArgumentException)
#define kTypeIllegalStateException kTypeJavaClass(IllegalStateException)

// Array builder macro
#define kTypeArray(x) "[" x

#define TYPE_EQUALS(_TYPE1, _TYPE2) (strcmp(_TYPE1, _TYPE2) == 0)

#endif // __JniHelpersCommon_h__
