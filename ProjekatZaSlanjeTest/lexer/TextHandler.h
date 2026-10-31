#ifndef TEXT_HANDLER_H
#define TEXT_HANDLER_H

// Input: a single character
// Output: true or false for the requested character class
// Responsibility: character tests shared by the lexer (whitespace, special, alphanumeric, allowed)
class TextHandler {
public:
	static bool isWhiteSpace(char c);
	static bool isSpecialCharacter(char c);
	static bool isAlphaNumeric(char c);
	static bool isNumeric(char c);
	static bool isAllowedCharacter(char c);
};

#endif
