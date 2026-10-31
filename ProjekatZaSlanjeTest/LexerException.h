#ifndef LEXER_EXCEPTION_H
#define LEXER_EXCEPTION_H

#include "CPIException.h"

class LexerException : public CPIException {
public:
	LexerException(const std::string& m);
	LexerException(const std::string& m, std::vector<unsigned int>& p, std::string& i);
	
	const char* what() const override;

	const std::string& getInput() const { return input; };
	const std::string& getWholeMessage() const { return wholeMessage; }
	
	~LexerException();
private:
	std::string input;
	std::string wholeMessage;
};

#endif