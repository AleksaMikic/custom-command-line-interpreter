#include "VisitorException.h"


VisitorException::VisitorException(const std::string& m):CPIException(m) {

}

VisitorException::VisitorException(const std::string& m, std::vector<unsigned int>& p) :CPIException(m, p) {

}
VisitorException::~VisitorException() {

}
const char* VisitorException::what() const{
	return message.c_str();
}