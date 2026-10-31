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
