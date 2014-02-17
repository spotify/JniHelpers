#ifndef __ClassRegistry_h__
#define __ClassRegistry_h__

#include "JniHelpersCommon.h"
#include "ClassWrapper.h"
#include "JavaExceptionUtils.h"
#include <map>
#include <string>
#include <string.h>

namespace spotify {
namespace jni {

#if WIN32
// TODO: Should only be exported for DLL builds
template class EXPORT std::map<std::string, const ClassWrapper*>;
#endif

/**
 * @brief Keeps a map of cached ClassWrapper instances
 *
 * The ClassRegistry class holds a map of bare (but initialized) ClassWrapper objects.
 * Since looking up class, field, and method definitions is somewhat expensive, it
 * makes sense to cache this data (and in fact, the JNI documentation also advises
 * doing this).
 *
 * Your program should keep an instance of the ClassRegistry either globally (with a
 * static object), or in some other long-lived object. During the JNI initialization
 * called via System.loadLibrary() in Java, you should add each ClassWrapper object
 * that you want to use in the map. Assuming that you have correctly subclassed the
 * ClassWrapper interface in your objects, they should be initialized and cache their
 * field and method data for quick access later.
 *
 * After the map has been configured, you can call newInstance<>() to create a new
 * ClassWrapper object of the desired type. The method/field IDs from the global map
 * will be merged into this object, thus making it relatively cheap to copy to/from
 * Java objects.
 */
class EXPORT ClassRegistry {
public:
  ClassRegistry() {}
  virtual ~ClassRegistry();

  /**
   * @brief Add a new class to the map
   * @param env JNIEnv
   * @param item ClassWrapper instance to add. This instance should be properly
   *             initialized (see the ClassWrapper documentation for more details).
   */
  virtual void add(JNIEnv *env, const ClassWrapper *item);

  /**
   * @brief Get an instance of a ClassWrapper definition in the map.
   * @param name Canonincal class name. If NULL, this method will throw an
   *             IllegalArgumentException to Java.
   * @return Pointer to instance, or NULL if no such instance exists.
   */
  virtual const ClassWrapper* get(const char* name);

  /**
   * @brief Create a new instance of a given ClassWrapper subclass
   *
   * This method will create a new instance of the given type, which must subclass
   * the ClassWrapper abstract class. The instance will be populated with the cached
   * field and method IDs that were (presumably) set during initialization.
   *
   * If local field mappings have been configured with ClassWrapper::mapField, then
   * the new instance will be populated with data from the Java object.
   *
   * @param env JNIEnv
   * @param fromObject Java object to copy data from
   * @return New instance, or NULL if none could not be created.
   */
  // Sorry about the mess in this header file, however some compilers
  // *cough*MSVC*cough* require templates to be in the header file or else they won't
  // work.
  template<typename TypeName>
  TypeName* newInstance(JNIEnv *env, jobject fromObject) {
    TypeName *result = new TypeName();
    const char *name = result->getCanonicalName();
    if (name == NULL || strlen(name) == 0) {
      JavaExceptionUtils::throwExceptionOfType(env, kTypeIllegalArgumentException,
        "Could not find canonical name for class");
      return NULL;
    }
    const TypeName *classInfo = dynamic_cast<const TypeName*>(get(name));
    if (classInfo == NULL) {
      JavaExceptionUtils::throwExceptionOfType(env, kTypeIllegalStateException,
        "No class information registered for '%s'", name);
      return NULL;
    }
    result->merge(classInfo);
    result->setJavaObject(env, fromObject);
    return result;
  }

  /**
   * @brief Return number of items in the map
   * @return Number of items
   */
  virtual const size_t size() const {
    return _classes.size();
  }

protected:
  std::map<std::string, const ClassWrapper*> _classes;
};

} // namespace jni
} // namespace spotify

#endif // __ClassRegistry_h__
