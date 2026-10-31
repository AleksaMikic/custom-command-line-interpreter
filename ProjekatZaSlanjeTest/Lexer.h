#ifndef LEXER_H
#define LEXER_H

#include "VisitableElement.h"
#include "LexerException.h"
#include "Token.h"
#include <vector>
#include <string>
#include <iostream>
class Lexer : public VisitableElement {
public:
	Lexer();
	explicit Lexer(std::string& i);
	std::vector<Token> tokenize();

	void setInput(std::string& i) { input = i; }
	void setPosition(unsigned int p) { position = p; }

	std::string& getInput() { return input; };
	unsigned int& getPosition() { return position; }
private:
	std::string input;
	unsigned int position;

	bool accept(Visitor& v)override;

	Token nextToken();//Used as main function
	Token tokenizeSpecialCharacters(unsigned int startingPosition);//Used for |, <, > and >>
	Token tokenizeWord(unsigned int startingPosition);//Used for COMMAND, OPTION or ARGUMENT
	Token tokenizeQuotedString(unsigned int startingPosition);//Used for parsing string as input

};

#endif

