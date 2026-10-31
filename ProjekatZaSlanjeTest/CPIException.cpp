#include "CPIException.h"
CPIException::CPIException(const std::string& m):message(m) {

}

CPIException::CPIException(const std::string& m, std::vector<unsigned int>& p) : message(m),positions(p) {
	
}

const char* CPIException::what() const {
	return message.c_str();
}


CPIException::~CPIException() {

}