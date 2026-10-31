#ifndef PARSER_H
#define PARSER_H

#include "Token.h"
#include "VisitableElement.h"
#include "Visitor.h"
#include <vector>
#include <string>
#include <iostream>

// Input: tokens from the lexer
// Output: list of command blocks, or an empty list if the syntax is invalid
// Responsibility: splits tokens into command blocks at each pipe, replaces numeric command names with the referenced block, and runs the syntax checks
class Parser : VisitableElement{
public:
	Parser();
	Parser(std::vector<Token> ts,std::vector<CommandBlock> cbs);

	void breakUp();
	bool accept(Visitor& v) override;

	void setTokens(std::vector<Token>& ts) { tokens = ts; }
	void setCommandBlocks(std::vector<CommandBlock>& cbs) { commandBlocks = cbs; }
	std::vector<CommandBlock>& getCommandBlocks() { return commandBlocks; }
	std::vector<Token>& getTokens() { return tokens; }
	void resetCommandBlocks() { commandBlocks.clear(); }

	std::vector<CommandBlock>& parse();
private:

	std::vector<Token> tokens;
	std::vector<CommandBlock> commandBlocks;
};

#endif