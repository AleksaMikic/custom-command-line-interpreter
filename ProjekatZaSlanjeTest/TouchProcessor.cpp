#include "TouchProcessor.h"
#include "VisitorException.h"
#include <fstream>
#include <memory>

unsigned int TouchProcessor::getCurrentPosition() {
	return currentPosition;
}

std::vector<unsigned int>& TouchProcessor::getPositions() {
	return Visitor::getPositions();
}

std::vector<std::string>& TouchProcessor::getMessages() {
	return Visitor::getMessages();
}

TouchProcessor::TouchProcessor(std::vector<unsigned int>& ps, std::vector<std::string>& ms, unsigned int cp) :Visitor(ps, ms), currentPosition(cp), wholeMessage() {

}

TouchProcessor::TouchProcessor(TouchProcessor& other) :Visitor(other.getPositions(), other.getMessages()), currentPosition(other.currentPosition), wholeMessage(other.wholeMessage) {

}

TouchProcessor::TouchProcessor(unsigned int cp) :Visitor(), currentPosition(cp), wholeMessage() {

}

TouchProcessor::~TouchProcessor() {

}

void TouchProcessor::pushException(std::string m, unsigned int p) {
	getMessages().push_back(m);
	getPositions().push_back(p);
}

int TouchProcessor::convert(Token& t) {
	if (t.getTokenType() == COMMAND) {
		return 0;
	}
	else if (t.getTokenType() == OPTION) {
		return 1;
	}
	else if (t.getTokenType() == FILE_NAME || t.getTokenType() == QUOTED_STRING) {
		return 2;
	}
	else if (t.getTokenType() == REDIRECT_INPUT) {
		return 3;
	}
	else {
		return 4;
	}
}

std::string TouchProcessor::reconvert(int p) {
	if (p == 0) {
		return "COMMAND";
	}
	else if (p == 1) {
		return "OPTION";
	}
	else if (p == 2) {
		return "ARGUMENT";
	}
	else if (p == 3) {
		return "REDIRECT_INPUT";
	}
	else {
		return "REDIRECT_OUTPUT";
	}
}

bool TouchProcessor::checkAllowedToken(Token& t) {
	if (t.getTokenType() == TokenType::OPTION||t.getTokenType() == TokenType::QUOTED_STRING) {
		return false;
	}
	else {
		return true;
	}
}

bool TouchProcessor::checkAllowedTokens(CommandBlock& cb) {
	bool err = true;
	for (unsigned int i = 0; i < static_cast<unsigned int>(cb.getTokens().size()); i++) {
		if (!checkAllowedToken(cb.getTokens()[i])) {
			err &= false;
			pushException("Semantical error, command Touch doesn't support options and quoted strings\n", i);
		}
		if (convert(cb.getTokens()[i]) >= 3) {
			i++;
		}
	}
	return err;
}

bool TouchProcessor::checkIfIsFilePath(Token& t) {
	std::string& str = t.getValue();
	unsigned int size = static_cast<unsigned int>(str.size());
	if (size < 4) {
		return false;
	}
	else {
		if (str[size - 4] == '.' && str[size - 3] == 't' && str[size - 2] == 'x' && str[size - 1] == 't') {
			return true;
		}
		return false;
	}
}

bool TouchProcessor::checkIfAllFilePathsAreFilePaths(CommandBlock& cb) {
	bool err = true;
	for (unsigned int i = 1; i < static_cast<unsigned int>(cb.getTokens().size()); i++) {
		if (cb.getTokens()[i].getTokenType() == TokenType::FILE_NAME) {
			if (!checkIfIsFilePath(cb.getTokens()[i])) {
				err &= false;
				pushException("Semantical error,file with the name: " + cb.getTokens()[i].getValue() + " isn't a text file!\n", currentPosition);
			}
		}
	}
	return err;
}

bool TouchProcessor::checkIfFileExists(Token& t) {
	std::ifstream filePath(t.getValue().c_str());
	bool b = filePath.good();
	return b;
}


bool TouchProcessor::checkAllFilePaths(CommandBlock& cb) {
	bool err = true;
	for (unsigned int i = 1; i < static_cast<unsigned int>(cb.getTokens().size()); i++) {
		if (cb.getTokens()[i].getTokenType() == TokenType::REDIRECT_OUTPUT || cb.getTokens()[i].getTokenType() == TokenType::REDIRECT_APPEND) {
			i++;
		}
		else if(cb.getTokens()[i].getTokenType() == TokenType::REDIRECT_INPUT) {
			i++;
			if (cb.getTokens()[i].getTokenType() == TokenType::FILE_NAME) {
				if (!checkIfFileExists(cb.getTokens()[i])) {
					err &= false;
					pushException("File with the name: " + cb.getTokens()[i].getValue() + " doesn't exist!\n", currentPosition);
				}
			}
		}
	}
	return err;
}

bool TouchProcessor::checkIfCreatingFileExists(Token& t) {
	bool err = true;
	if (checkIfFileExists(t)) {
		err &= false;
		pushException("Semantical error, file with the name: " + t.getValue() + " can't be created, because it already exists!\n",currentPosition);
	}
	return err;
}

bool TouchProcessor::checkForDuplicates(CommandBlock& cb) {
	bool err = true;
	int j = 0;
	std::vector<int> v;
	std::vector<int> map(5, 0);
	for (unsigned int i = 0; i < static_cast<unsigned int>(cb.getTokens().size()); i++) {
		v.push_back(convert(cb.getTokens()[i]));
		++map[v[j]];
		j++;
		if (convert(cb.getTokens()[i]) >= 3) {
			i++;
		}
	}
	for (unsigned int i = 0; i < static_cast<unsigned int>(map.size()); i++) {
		if (map[i] > 1) {
			err &= false;
			pushException("Semantical error, command Touch has too much tokens of type: " + reconvert(i) + "\n", currentPosition);
		}
	}
	return err;
}


bool TouchProcessor::checkMandatory(CommandBlock& cb) {
	bool err = true;
	if (cb.getTokens().size() < 2) {
		err &= false;
		pushException("Semantical error, exception no argument tokens, command Touch must have a file name\n", currentPosition);
	}
	else {
		if (cb.getTokens()[1].getTokenType() != TokenType::FILE_NAME) {
			err &= false;
			pushException("Sematnical error, exception second token must be file name in command Touch\n ", currentPosition);
		}
		else {
			err &= checkIfCreatingFileExists(cb.getTokens()[1]);
		}
	}
	return err;
}


bool TouchProcessor::visit(Command& command) {
	bool err = true;
	try {
		Touch* touch = dynamic_cast<Touch*>(&command);
		if (touch) {
			err &= checkMandatory(touch->getCommandBlock());
			err &= checkAllowedTokens(touch->getCommandBlock());
			err &= checkForDuplicates(touch->getCommandBlock());
			err &= checkIfAllFilePathsAreFilePaths(touch->getCommandBlock());
			err &= checkAllFilePaths(touch->getCommandBlock());
		}
		else {
			throw VisitorException("Wrong visitor!\n");
		}
	}
	catch (VisitorException& e) {
		std::cerr << e.what() << "\n";
		return false;
	}
	return err;
}

std::string& TouchProcessor::sendWholeMessage() {
	size_t totalLength = 0;
	for (unsigned int i = 0; i < static_cast<unsigned int>(getMessages().size()); i++) {
		totalLength += getMessages()[i].size();
	}
	wholeMessage.reserve(totalLength);
	for (unsigned int i = 0; i < static_cast<unsigned int>(getMessages().size()); i++) {
		wholeMessage += getMessages()[i];
	}
	return wholeMessage;
}
