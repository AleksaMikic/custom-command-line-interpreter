#ifndef SEMANTIC_ANALYZER_EXCEPTION_H
#define SEMANTIC_ANALYZER_EXCEPTION_H

#include "CPIException.h"
#include "Token.h"

class SemanticAnalyzerException:public CPIException {
public:
	SemanticAnalyzerException(const std::string& m);
	SemanticAnalyzerException(const std::string& m, std::vector<CommandBlock> i,std::vector<unsigned int>& p, std::vector<std::string> ms);

	const char* what() const override;

	const std::string& getWholeMessage() const { return wholeMessage; }
	const std::vector<std::string>& getMessages() const { return messages; }

	~SemanticAnalyzerException();
private:
	std::string wholeMessage;
	std::vector<std::string> messages;
};

#endif
