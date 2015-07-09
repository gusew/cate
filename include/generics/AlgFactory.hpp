#ifndef ALGFACTORY_INCLUDED
#define ALGFACTORY_INCLUDED

#include <dlfcn.h>
#include <functional>
#include <string>
#include <generics/Base.hpp>
#include <memory>

/**
 * A simple wrapper class around an algorithm to release allocated ressources
 * (by earlier dynamic library binding) after the algorithm was deleted.
 * No smart pointer are used, as algorithm instance has to be deleted, before
 * the dlopen-handle can be used for dlclose.
 */
class AlgWrapper {
	Base* _algorithm;
	void* _handle;

public:
	AlgWrapper() : _algorithm(nullptr), _handle(nullptr) { }
	~AlgWrapper();

	inline void setAlgorithm(Base* alg) { _algorithm = alg; }
	inline void setHandle(void* handle) { _handle = handle; }

	inline Base* getAlgorithm() { return _algorithm; }
	inline bool isValid() { return (_algorithm != nullptr && _handle != nullptr); }
};

/**
 * Helps to create a specified filtering algorithm by using dynamic library
 * binding techniques during runtime.
 */
class AlgFactory {

public:
	/**
	 * Loads the specified algorithm library and instantiates it.
	 *
	 * @param name name of the algorithm library-file in application-path
   * @param result a smart pointer, which will be set with an algorithm wrapper instance
	 * @return true, if creation successful, otw. false
	 */
	static bool createAlgorithm(const std::string& name, std::unique_ptr<AlgWrapper>& result);
};

typedef Base* CreateAlg_t();

/**
 * Helper function to cast a void* (object-pointer) to a C++ function-pointer.
 * Is needed to convert the return value of dlsym(...) without warnings.
 *
 * @param fct a void* object
 * @return a C++ function pointer with the specified type (by template parameter).
 */
template<typename S> std::function<S> castVoidPtr(void* fct) {
	S* fct_ptr = 0; // workaround to convert [void*] to function-pointer
	*reinterpret_cast<void**>(&fct_ptr) = fct;
	return std::function<S>(fct_ptr);
}

#endif
