#include "TextHandler.h"

// provera specijalnih karaktera
bool TextHandler::isSpecialCharacter(char c) {
	if (c == '<' || c == '>' || c == '|') {
		return true;
	}
	else {
		return false;
	}
}
// provera belih karaktera
bool TextHandler::isWhiteSpace(char c) {
	if (c == ' ' || c == '\t' || c == '\n') {
		return true;
	}
	else {
		return false;
	}
}
// provera da li je slovo
bool TextHandler::isAlphaNumeric(char c) {
	if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9')) {
		return true;
	}
	else {
		return false;
	}
}

bool TextHandler::isNumeric(char c) {
	if (c >= '0' && c <= '9') {
		return true;
	}
	else {
		return false;
	}
}

bool TextHandler::isAllowedCharacter(char c) {
	if (isSpecialCharacter(c) || isWhiteSpace(c) || isAlphaNumeric(c) || c == '.' || c == '-'||c=='/') {
		return true;
	}
	else {
		return false;
	}
}