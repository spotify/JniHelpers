#ifndef __JniTypes_h__
#define __JniTypes_h__

#include <string.h>

#define kTypeInt "I"
#define kTypeShort "S"
#define kTypeLong "J"
#define kTypeFloat "F"
#define kTypeDouble "D"
#define kTypeBool "Z"
#define kTypeByte "B"
#define kTypeChar "C"
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

#endif // __JniTypes_h__
