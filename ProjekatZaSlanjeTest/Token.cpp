#include "Token.h"

std::string CommandBlock::convertCommandBlockToString() {
	std::string returningString = "";
	for (unsigned int i = 0; i < static_cast<unsigned int>(getTokens().size()); i++) {
		if (getTokens()[i].getTokenType() == TokenType::QUOTED_STRING) {
			returningString += "\"" + getTokens()[i].getValue() + "\" ";
		}
		else {
			returningString += getTokens()[i].getValue() + " ";
		}
	}
	return returningString;
}