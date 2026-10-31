#ifndef TOKEN_H
#define TOKEN_H

#include <string>
#include <vector>

enum TokenType {
	COMMAND,
	FILE_NAME,
	OPTION,
	PIPE,
	REDIRECT_INPUT,
	REDIRECT_OUTPUT,
	REDIRECT_APPEND,
	QUOTED_STRING
};


// Input: token type, text and starting position in the line
// Output: the stored type, text and position
// Responsibility: one meaningful piece of the input line produced by the lexer
class Token {
public:
	Token(TokenType t, std::string v, unsigned int p) : tokenType(t), value(v), position(p) {}

	TokenType getTokenType() const { return tokenType; }
	std::string& getValue() { return value; }
	unsigned int getPosition() const { return position; }
private:
	TokenType tokenType;
	std::string value;
	unsigned int position;
};

// Input: tokens between two pipe symbols
// Output: the token list, or the block rebuilt as text for error messages
// Responsibility: groups the tokens of one command in a pipeline
class CommandBlock {
private:
	std::vector<Token> tokens;
public:
	std::vector<Token>& getTokens() { return tokens; }
	void addToken(const Token& t) { tokens.push_back(t); }
	void clearTokens() { tokens.clear(); }
	std::string convertCommandBlockToString();
};
#endif
