#ifndef LEXER_VISITOR_H
#define LEXER_VISITOR_H

#include "Lexer.h"
// Input: a Lexer to check
// Output: true if the line is lexically valid
// Responsibility: interface for validators of the raw input line
class LexerVisitor {
public:
	LexerVisitor() {};

	virtual bool visit(Lexer& p) = 0;

	~LexerVisitor();
};

#endif