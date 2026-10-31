#ifndef VISITOR_EXCEPTION_H
#define VISITOR_EXCEPTION_H
#include "CPIException.h"
class VisitorException :public CPIException {
public:
	VisitorException(const std::string& m);
	VisitorException(const std::string& m, std::vector<unsigned int>& p);
	
	const char* what() const override;
	
	~VisitorException();
};

#endif
