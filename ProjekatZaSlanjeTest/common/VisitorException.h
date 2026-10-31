#ifndef VISITOR_EXCEPTION_H
#define VISITOR_EXCEPTION_H
#include "CPIException.h"
// Input: an error message
// Output: the error text through what()
// Responsibility: reports that an element was given a validator of the wrong type
class VisitorException :public CPIException {
public:
	VisitorException(const std::string& m);
	VisitorException(const std::string& m, std::vector<unsigned int>& p);
	
	const char* what() const override;
	
	~VisitorException();
};

#endif
