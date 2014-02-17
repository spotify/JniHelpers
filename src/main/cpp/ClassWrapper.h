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

namespace spotify {
namespace jni {

#if WIN32
// TODO: Should only be defined for dynamic lib builds
template class EXPORT std::vector<JNINativeMethod>;
template class EXPORT std::map<std::string, jmethodID>;
template class EXPORT std::map<std::string, jfieldID>;
template class EXPORT std::map<std::string, FieldMapping*>; 
#endif

/**
 * @brief Interface for a Java/C++ bridge class
 *
 * This interface serves as a bridge between Java/C++ classes, allowing one to easily
 * transfer data over JNI. You can use this class to sync data between Java/C++, or
 * simply to register native method callbacks. For example usage, see the test cases
 * distributed with JniHelpers.
 */
class EXPORT ClassWrapper {
public:
  /**
   * @brief Create a new empty instance.
   *
   * This method is invoked by ClassRegistry::newInstance, so it should have a minimal
   * implementation.
   */
  ClassWrapper() : _clazz(NULL) {}

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
  ClassWrapper(JNIEnv *env) {
    // TODO: Can't call initialize here because of symbol visibility :(
    // initialize(env);
  }

  virtual ~ClassWrapper();

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

public:
  /**
   * @brief Return the simple name for the class (ie, com.example.Foo -> "Foo")
   * @return Class name (should not be NULL)
   */
  const char* getSimpleName() const;

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
   * If you want to create field mappings (see mapField()), then you should override
   * this method (making sure to call the super!), and make those calls here. That
   * will set up direct mappings between your class' local fields and those of the
   * Java world, which allow setJavaObject() and toJavaObject() to automatically set
   * data to the given object.
   *
   * If you don't care about syncing data between Java/C++ classes (for instance, in
   * the case of a class which is only designed to receive native callbacks), you
   * can ignore this method.
   *
   * @param globalInstance Global instance from ClassRegistry (should not be NULL)
   */
  void merge(const ClassWrapper *globalInstance);

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
  void setJavaObject(JNIEnv *env, jobject javaThis);

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
  jobject toJavaObject(JNIEnv *env);

  /**
   * @brief Get JNI class information for this object type
   * @return JNI class object
   */
  JniGlobalRef<jclass> getClass() const;

  /**
   * @brief Retreive a method from the cache
   * @param method_name Method name (without signature)
   * @return JNI method ID, or NULL if no such method was cached
   */
  jmethodID getMethod(const char *method_name);

  /**
   * @brief Retreive a field from the cache
   * @param field_name Field name
   * @return JNI field ID, or NULL if no such field was cached
   */
  jfieldID getField(const char* field_name);

protected:
  typedef struct {
    const char* type;
    void* address;
  } FieldMapping;

  /**
   * @brief Find and save JNI class information for this object
   *
   * This method uses getCanonicalName() to find the class and saves it to the
   * cached class info. This method should be called during initialize() or things
   * may break later.
   *
   * @param env JNIEnv
   */
  void setClass(JNIEnv *env);

  /**
   * @brief Find a method and save it in the cache
   * @param env JNIEnv
   * @param method_name Method name, as it appears in the Java class
   * @param return_type Return type (see type definitions in JniTypes.h)
   * @param ... Method argument list, which *must* end with NULL. If the method
   *            takes no arguments, pass only NULL here instead of kTypeVoid.
   */
  void cacheMethod(JNIEnv *env, const char *method_name, const char *return_type, ...);

  /**
   * @brief Find a field and save it in the cache
   * @param env JNIEnv
   * @param field_name Field name, as it appears in the Java class
   * @param field_type Field type (see type definitions in JniTypes.h)
   */
  void cacheField(JNIEnv *env, const char *field_name, const char *field_type);

  /**
   * @brief Add a mapping between a Java field name and an instance of this class
   * @param field_name Field name
   * @param field_type Field type (see type definitions in JniTypes.h)
   * @param field_ptr Pointer to field. Must be public. :(
   */
  // TODO: Would be nice to be able to pass in a setter function, not sure how to manage that
  void mapField(const char *field_name, const char *field_type, void *field_ptr);

  void addNativeMethod(const char *method_name, void *function, const char *return_type, ...);
  bool registerNativeMethods(JNIEnv *env);

protected:
  JniGlobalRef<jclass> _clazz;
  jmethodID _constructor;
  std::map<std::string, jmethodID> _methods;
  std::map<std::string, jfieldID> _fields;
  std::map<std::string, FieldMapping*> _field_mappings;

private:
  std::vector<JNINativeMethod> _jni_methods;
};

} // namespace jni
} // namespace spotify

#endif // __ClassWrapper_h__
