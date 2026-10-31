#ifndef PARSER_VISITOR_H
#define PARSER_VISITOR_H

#include "Parser.h"

// Input: a Parser to check
// Output: true if every command block is syntactically valid
// Responsibility: interface for syntax validators
class ParserVisitor{
public:
	ParserVisitor() {};

	virtual bool visit(Parser& p)=0;

	~ParserVisitor();
};

#endif