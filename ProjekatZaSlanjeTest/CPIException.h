#ifndef CPI_EXCEPTION_H
#define CPI_EXCEPTION_H

#include<stdexcept>
#include<vector>
#include<string>

class CPIException : public std::exception {
public:
	CPIException(const std::string& m);
	CPIException(const std::string& m, std::vector<unsigned int>& p);
	virtual const char* what() const override ;
	virtual ~CPIException();

protected:
	std::string message;
	std::vector<unsigned int> positions;
};

#endif
