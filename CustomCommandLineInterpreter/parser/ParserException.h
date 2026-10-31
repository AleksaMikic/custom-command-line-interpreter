#ifndef PARSER_EXCEPTION_H
#define PARSER_EXCEPTION_H

#include <vector>
#include "CPIException.h"
#include "Token.h"
// Input: the command blocks, positions of faulty blocks and their messages
// Output: an error message grouped by command block
// Responsibility: reports syntax errors
class ParserException :public CPIException {
public:
	ParserException(const std::string& m);
	ParserException(const std::string& m, std::vector<unsigned int>& p, std::vector<CommandBlock>& i,std::vector<std::string> ms);
	
	const char* what() const noexcept override;

	const std::string& getWholeMessage() const{ return wholeMessage; }
	const std::vector<CommandBlock>& getInput() const { return input; }
	const std::vector<std::string>& getMessages() const{ return messages; }

	~ParserException();
private:
	std::string wholeMessage;
	std::vector<CommandBlock> input;
	std::vector<std::string> messages;
};

#endif
