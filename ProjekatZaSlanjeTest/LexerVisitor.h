#ifndef LEXER_VISITOR_H
#define LEXER_VISITOR_H

#include "Lexer.h"
class LexerVisitor {
public:
	LexerVisitor() {};

	virtual bool visit(Lexer& p) = 0;

	~LexerVisitor();
};

#endif