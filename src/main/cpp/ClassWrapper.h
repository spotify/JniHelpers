#ifndef __ClassWrapper_h__
#define __ClassWrapper_h__

#include "JniHelpersCommon.h"
#include "JavaString.h"
#include "JniGlobalRef.h"
#include <map>
#include <string>
#include <sstream>
#include <vector>
#include <stdarg.h>

#ifndef PERSIST_FIELD_NAME
/**
 * @brief Field name to use for persisting objects (see persist() for details)
 */
#define PERSIST_FIELD_NAME "nPtr"
#endif

namespace spotify {
namespace jni {

typedef struct {
  const char* type;
  void* address;
} FieldMapping;

#if WIN32
// TODO: This is a MSVC thing, should refactor to use PIMPL instead (ugh)
template class EXPORT std::vector<JNINativeMethod>;
template class EXPORT std::map<std::string, jmethodID>;
template class EXPORT std::map<std::string, jfieldID>;
template class EXPORT std::map<std::string, FieldMapping*>; 
#endif

typedef std::map<std::string, jfieldID> FieldMap;
typedef std::map<std::string, jmethodID> MethodMap;

/**
 * @brief Interface for a Java/C++ bridge class
 *
 * This interface serves as a bridge between Java/C++ classes, allowing one to easily
 * transfer data over JNI. You can use this class to sync data between Java/C++, or
 * simply to register native method callbacks. For example usage, see the test cases
 * distributed with JniHelpers.
 */
class EXPORT ClassWrapper {
// Constructors /////////////////////////////////////////////////////////////////////
public:
  /**
   * @brief Create a new empty instance.
   *
   * This method is invoked by ClassRegistry::newInstance, so it should have a minimal
   * implementation.
   */
  ClassWrapper() : _clazz_global(NULL), _clazz(NULL), _constructor(NULL) {}

  /**
   * @brief Create a new ClassWrapper with class information
   *
   * This method will be called by the top-level JNI initialization when the library
   * is loaded into memory. This constructor is expected to contain a call to the
   * initialize() method, which in turn will cache all necessary class, method, and
   * field data for quick access later on.
   *
   * Generally speaking, you should not create ClassWrapper objects with this ctor,
   * as it will be rather expensive to lookup all of the class information. Instead
   * use the empty ctor, or the ClassRegistry::newInstance method.
   *
   * @param env JNIEnv
   */
  ClassWrapper(JNIEnv *env) : _clazz_global(NULL), _clazz(NULL), _constructor(NULL) {
    // Ideally, we would like to call initialize() from the ClassWrapper() ctor.
    // However this won't work because initialize() is pure virtual, and such methods
    // cannot be called here because the object is in an incomplete state. So instead,
    // one needs to make sure that initialize(env) is called in the subclass' ctor.
  }

  virtual ~ClassWrapper();

// Pure virtual methods /////////////////////////////////////////////////////////////
public:
  /**
   * @brief Cache Java class information
   *
   * This method should take care of any necessary class information caching, namely
   * it must call setClass(), and can optionally call cacheMethod() and cacheField()
   * on any methods or fields that you might want to use later.
   *
   * Additionally, you may set up callbacks from here by calling addNativeMethod()
   * for each method, and then registerNativeMethods() at the end.
   *
   * @param env JNIEnv
   */
  virtual void initialize(JNIEnv *env) = 0;

  /**
   * @brief Check to see if global class info is registered for this object
   *
   * Instances of ClassWrapper are still usable without having been initialized,
   * however most JNI-related operations such as toJavaObject() and setJavaObject()
   * will not work properly.
   *
   * @return True if this instance has valid class info, false otherwise
   */
  virtual bool isInitialized() const;

  /**
   * @brief Configure field mappings per instance
   *
   * If you want to create field mappings (see mapField()), then you should do it
   * here. That will configure direct mappings between your class' local fields
   * and those of the Java world, which allow setJavaObject() and toJavaObject()
   * to automatically copy data between C++/Java class instances.
   *
   * If you don't care about syncing data between Java/C++ classes (for instance, in
   * the case of a class which is only designed to receive native callbacks), you
   * can ignore this method and simply implement it as an empty body.
   */
  virtual void mapFields() = 0;

  /**
   * @brief Get the canonical name for the associated Java class
   *
   * Note that the canonical name is in JNI notation, not Java notation. So if your
   * Java class is com.example.Foo, then this should return "com/example/Foo".
   * Preferably this method should return a pointer to some heap method; returning
   * a stack pointer will probably result in leaked memory.
   *
   * @return Canonincal name (may NOT be NULL, or other problems will result!)
   */
  virtual const char* getCanonicalName() const = 0;

// Public methods ///////////////////////////////////////////////////////////////////
public:
  /**
   * @brief Return the simple name for the class (ie, com.example.Foo -> "Foo")
   * @return Class name (should not be NULL)
   */
  virtual const char* getSimpleName() const;

  /**
   * @brief Copy cached class info data from the global instance
   *
   * If correctly configured, the ClassRegistry map should contains bare instances
   * of your ClassWrapper subclasses, each of which has been called with the
   * ClassWrapper(JNIEnv *env) ctor and therefore done some actual work in the
   * initialize(JNIEnv *env) method.
   *
   * When ClassRegistry::newInstance is called, it will create an empty instance of
   * this class and then populate it with data from a corresponding Java instance.
   * For this to work correctly, the cached class/field/method data which was fetched
   * during initialize() must be populated into this instance. That is done with this
   * method.
   *
   * @param globalInstance Global instance from ClassRegistry (should not be NULL)
   */
  virtual void merge(const ClassWrapper *globalInstance);

  /**
   * @brief Persist this object as a reference in a Java object
   *
   * Sometimes you might need to create a C++ object with a long lifespan which may
   * be too expensive to keep copying between native and Java code. Or perhaps the
   * C++ object contains members which are not easily mapped to Java types and can't
   * be copied up to Java.
   *
   * In such cases, it makes more sense to create the C++ object on the heap and
   * pass a reference to it up to Java. In this manner, subsequent calls from Java
   * may return to native code and retrieve the native object instance directly.
   *
   * For this method to work, your Java class must declare a `long` field named
   * `PERSIST_FIELD_NAME` (which is defined to `nPtr`). If your class defines such
   * a field, then it will be used by ClassRegistry::newInstance to give you a
   * pointer to that instance instead of creating a new object.
   *
   * Note that this implies that you may only have one persisted native object in
   * memory at any given time. Also, you are responsible for disposing of it; the
   * preferred convention for this is to define a native method to call destroy().
   * This is perhaps a bit tedious, however that's one of the risks of mixing Java
   * and C++ lifecycles. Therefore, you should only use this if you absolutely
   * cannot copy object data to Java via the toJavaObject() method.
   *
   * Rather than make assumptions on how your application manages memory, calling
   * this method multiple times with different objects will leak them rather than
   * free them. To free the objects, you should call destroy() instead. You've been
   * warned!
   *
   * The restrictions of the hard-coded field name and limitation of one persisted
   * object may be removed in a future version of the library.
   *
   * @param env JNIEnv
   * @param javaThis Java object to set the field on (may not be NULL)
   * @return True if the object was persisted, false otherwise. If your class is
   *         incorrectly configured (ie, has no field named `nPtr`) then this
   *         method will return false rather than throwing.
   */
  virtual bool persist(JNIEnv *env, jobject javaThis);

  /**
   * @brief Check to see if this class is persisted natively
   *
   * See the persist() method for more information on ClassWrapper persistence.
   *
   * @return True if the class is persisted, false otherwise
   */
  virtual bool isPersistenceEnabled() const;

  /**
   * @brief Return the object saved by a previous call to persist()
   * @param env JNIEnv
   * @param javaThis Java object to set the field from
   * @return Object instance, or NULL if no persisted instance exists.
   */
  virtual ClassWrapper* getPersistedInstance(JNIEnv *env, jobject javaThis) const;

  /**
   * @brief Free an object set with persist()
   *
   * If your class has been configured to persist objects (see the documentation in
   * persist() for more details), then this method will free the persisted object,
   * and set the `nPtr` field on the corresponding Java object to 0.
   *
   * This method is safe to call even if no object has been persisted in the Java
   * instance. In such cases, it does nothing.
   *
   * @param env JNIEnv
   * @param javaThis Java object to free the field on (may not be NULL)
   */
  virtual void destroy(JNIEnv *env, jobject javaThis);

  /**
   * @brief Set data from a Java instance to this class
   *
   * This call is invoked by ClassRegistry::newInstance to set data from a Java
   * object into this instance. For this method to work, you need to set up field
   * mappings by overriding merge() and calling mapField(). See the documentation
   * for mapField() for more details.
   *
   * @param env JNIEnv
   * @param javaThis Java object to copy data from
   */
  virtual void setJavaObject(JNIEnv *env, jobject javaThis);

  /**
   * @brief Set data from this instance to a Java object
   *
   * This method creates a new Java representation of this ClassWrapper instance,
   * which is then populated with data from this object. The resulting object can
   * then be passed up to Java and used there. For this method to work properly,
   * you need to set up field mappings by overriding merge() and calling mapField().
   * See the documentation for mapField() for more details.
   *
   * @param env JNIEnv
   * @return Initialized Java object, or NULL if the object could not be created
   */
  virtual jobject toJavaObject(JNIEnv *env);

  /**
   * @brief Retreive a method from the cache
   * @param method_name Method name (without signature)
   * @return JNI method ID, or NULL if no such method was cached
   */
  virtual jmethodID getMethod(const char *method_name) const;

  /**
   * @brief Retreive a field from the cache
   * @param field_name Field name
   * @return JNI field ID, or NULL if no such field was cached
   */
  virtual jfieldID getField(const char* field_name) const;

// Internal helper calls ////////////////////////////////////////////////////////////
protected:
  /**
   * @brief Find and save JNI class information for this object
   *
   * This method uses getCanonicalName() to find the class and saves it to the
   * cached class info. This method should be called during initialize() or
   * you may get some unexpected behavior when trying to use instances of this
   * class.
   *
   * @param env JNIEnv
   */
  virtual void setClass(JNIEnv *env);

  /**
   * @brief Cache a constructor
   *
   * This method should be called from within initialize() if you plan on using
   * toJavaObject() with this class instance. If you do not need toJavaObject(),
   * then it is not necessary to call this.
   *
   * Note that only no-argument constructors are supported at the moment. This is
   * a known limitation and will be addressed in a future version of the library.
   *
   * @param env JNIEnv
   */
  virtual void cacheConstructor(JNIEnv *env);

  /**
   * @brief Find a method and save it in the cache
   * @param env JNIEnv
   * @param method_name Method name, as it appears in the Java class
   * @param return_type Return type (see type definitions in JniTypes.h)
   * @param ... Method argument list, which *must* end with NULL. If the method
   *            takes no arguments, pass only NULL here instead of kTypeVoid.
   */
  virtual void cacheMethod(JNIEnv *env, const char *method_name, const char *return_type, ...);

  /**
   * @brief Find a field and save it in the cache
   * @param env JNIEnv
   * @param field_name Field name, as it appears in the Java class
   * @param field_type Field type (see type definitions in JniTypes.h)
   */
  virtual void cacheField(JNIEnv *env, const char *field_name, const char *field_type);

  /**
   * @brief Add a mapping between a Java field name and an instance of this class
   * @param field_name Field name
   * @param field_type Field type (see type definitions in JniTypes.h)
   * @param field_ptr Pointer to field. Must be public. :(
   */
  // TODO: Would be nice to be able to pass in a setter function, not sure how to manage that
  // At least, how to manage it without C++11 features or boost::bind...
  virtual void mapField(const char *field_name, const char *field_type, void *field_ptr);

  /**
   * @brief Get a field mapping for this class instance
   * @param key Field name
   * @return Field mapping, or NULL if no such field mapping exists
   */
  virtual FieldMapping* getFieldMapping(const char *key) const;

  /**
   * @brief Add a native function callback to this class
   *
   * This method should be called in initialize() to register native methods to
   * be called from Java. After adding the methods, you must call the
   * registerNativeMethods() function at the end of initialize(), but only once!
   *
   * Note since JNI is fundamentally a C API, the function pointer must be static,
   * therefore the intended usage is to call ClassRegistry::newInstance from
   * within the static method in order to obtain a real object.
   *
   * @param method_name Method name
   * @param function Function to be called when the method is invoked from Java.
   * @param return_type Return type (see types declared in JniTypes.h)
   * @param ... Method argument list, which *must* end with NULL. If the method
   *            takes no arguments, pass only NULL here instead of kTypeVoid.
   */
  virtual void addNativeMethod(const char *method_name, void *function, const char *return_type, ...);

  /**
   * @brief Register all native methods on the class
   *
   * This method should be called *once* at the end of initialize(). It will tell
   * the JVM to register mappings between `native` Java methods and those which
   * you have created in calls to addNativeMethod().
   *
   * @param env JNIEnv
   * @return True if successful, false otherwise. If you register methods which
   *         do not exist in Java or have invalid signatures, then an exception
   *         may be thrown to Java.
   */
  virtual bool registerNativeMethods(JNIEnv *env);

// Fields ///////////////////////////////////////////////////////////////////////////
protected:
  jclass _clazz;
  jmethodID _constructor;
  MethodMap _methods;
  FieldMap _fields;
  std::map<std::string, FieldMapping*> _field_mappings;

private:
  // This reference to the class information should *only* be held by the global
  // instance of the class held in the ClassRegistry. For this reason it is private,
  // and a pointer to the internal jclass is copied over to any locally created
  // instances during merging (ie, from newInstance).
  // Otherwise the global instance will be destroyed when any local instances are
  // destroyed, which would be bad.
  JniGlobalRef<jclass> _clazz_global;

private:
  std::vector<JNINativeMethod> _jni_methods;
};

} // namespace jni
} // namespace spotify

#endif // __ClassWrapper_h__
