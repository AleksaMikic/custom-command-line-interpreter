#ifndef PARSER_VISITOR_H
#define PARSER_VISITOR_H

#include "Parser.h"

class ParserVisitor{
public:
	ParserVisitor() {};

	virtual bool visit(Parser& p)=0;

	~ParserVisitor();
};

#endif