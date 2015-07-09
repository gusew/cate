#include <generics/AlgFactory.hpp>
#include <iostream>

AlgWrapper::~AlgWrapper() {
	if (_algorithm != nullptr) delete _algorithm; 
	if (_handle != nullptr) dlclose(_handle); // reason why smart pointer would not help
}

bool AlgFactory::createAlgorithm(const std::string& name, std::unique_ptr<AlgWrapper>& result) {
  result.reset(new AlgWrapper);
	void* handle = dlopen(name.c_str(), RTLD_NOW); // RTLD_LAZY);

	if (!handle) {
		std::cerr << "Error dlopen: " << std::string(dlerror()) << std::endl;
		std::cerr << "Failed to load filtering-algorithm!" << std::endl;
		return false;
	} else {
		result->setHandle(handle); // to close dl-handle after use
	}
	
	std::function<CreateAlg_t> create = castVoidPtr<CreateAlg_t>(dlsym(handle, "create_algorithm"));
	result->setAlgorithm(create());
	
	// if "dlclose" called here, a segmentation fault will happen
	
	return true;
}
