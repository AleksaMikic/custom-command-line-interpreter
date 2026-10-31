#ifndef OPTIONAL_H
#define OPTIONAL_H

#include<stdexcept>

template<typename T>
class Optional {
public:
	Optional():value(nullptr),hasValue(false){}
	Optional(T& value, bool hasValue = true) {
		try {
			value = nullptr;
			value = new T(value);
			hasValue = true;
		}
		catch (const std::bad_alloc& e) {
			throw std::runtime_error("Failed to allocate pointer data for Optional\n"+ std::string(e.what()));
		}
	}
	Optional(const Optional& other) : value(nullptr) {  // Init null
		if (other.value == nullptr) {
			return;  
		}
		if (this != &other) {
			reset();
			try {
				value = new T(*other.value);
			}
			catch (const std::bad_alloc& e) {
				throw std::runtime_error("Failed to allocate pointer data for Optional\n" + std::string(e.what()));
			}
		}
	}
	void reset() {
		if (value != nullptr) {
			delete value;
			value = nullptr;
		}
	}
	Optional& operator=(Optional& other) {
		if (this != &other) {
			reset();
			if (other.value != nullptr) {
				try {
					value = nullptr;
					value = new T(*other.value);
				}
				catch (const std::bad_alloc& e) {
					throw std::runtime_error("Failed to allocate pointer data for Optional\n" + std::string(e.what()));
				}
			}
		}
		return *this;
	}

	void setValue(T& val) {
		reset();
		value = new T(val);
		hasValue = true;
	}
	bool checkHasValue() {
		return hasValue != false;
	}
	T* getValue() {
		if (value == nullptr) {
			throw std::runtime_error("Accessing empty Optional\n");
		}
		return value;
	}


	~Optional() {
		reset();
	}
private:
	T* value;
	bool hasValue;
};

#endif

