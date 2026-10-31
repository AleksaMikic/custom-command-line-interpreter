#ifndef CPI_EXCEPTION_H
#define CPI_EXCEPTION_H

#include<stdexcept>
#include<vector>
#include<string>

// Input: an error message and optional positions
// Output: the error text through what()
// Responsibility: base class of all interpreter exceptions
class CPIException : public std::exception {
public:
	CPIException(const std::string& m);
	CPIException(const std::string& m, std::vector<unsigned int>& p);
	virtual const char* what() const noexcept override ;
	virtual ~CPIException();

protected:
	std::string message;
	std::vector<unsigned int> positions;
};

#endif
