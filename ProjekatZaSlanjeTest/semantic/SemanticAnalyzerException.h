#ifndef SEMANTIC_ANALYZER_EXCEPTION_H
#define SEMANTIC_ANALYZER_EXCEPTION_H

#include "CPIException.h"
#include "Token.h"

// Input: the command blocks and the messages of each failing block
// Output: an error message showing the whole line and every reason
// Responsibility: reports semantic errors
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
