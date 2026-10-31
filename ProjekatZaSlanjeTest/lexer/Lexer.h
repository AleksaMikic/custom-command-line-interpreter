#ifndef LEXER_H
#define LEXER_H

#include "VisitableElement.h"
#include "LexerException.h"
#include "Token.h"
#include <vector>
#include <string>
#include <iostream>
// Input: one raw command line
// Output: list of tokens, or an empty list if the line has lexical errors
// Responsibility: splits the line into commands, options, file names, quoted strings, pipes and redirects
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

