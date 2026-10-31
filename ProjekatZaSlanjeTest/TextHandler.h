#ifndef TEXT_HANDLER_H
#define TEXT_HANDLER_H

class TextHandler {
public:
	static bool isWhiteSpace(char c);
	static bool isSpecialCharacter(char c);
	static bool isAlphaNumeric(char c);
	static bool isNumeric(char c);
	static bool isAllowedCharacter(char c);
};

#endif
