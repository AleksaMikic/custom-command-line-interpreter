#include "HeadProcessor.h"
#include "VisitorException.h"
#include "TextHandler.h"
#include <fstream>
#include <memory>

unsigned int HeadProcessor::getCurrentPosition() {
	return currentPosition;
}

std::vector<unsigned int>& HeadProcessor::getPositions() {
	return Visitor::getPositions();
}

std::vector<std::string>& HeadProcessor::getMessages() {
	return Visitor::getMessages();
}

HeadProcessor::HeadProcessor(std::vector<unsigned int>& ps, std::vector<std::string>& ms, unsigned int cp) :Visitor(ps, ms), currentPosition(cp), wholeMessage() {
}

HeadProcessor::HeadProcessor(HeadProcessor& other) :Visitor(other.getPositions(), other.getMessages()), currentPosition(other.currentPosition), wholeMessage(other.wholeMessage) {
}

HeadProcessor::HeadProcessor(unsigned int cp) :Visitor(), currentPosition(cp), wholeMessage() {
}

HeadProcessor::~HeadProcessor() {
}

void HeadProcessor::pushException(std::string m, unsigned int p) {
	getMessages().push_back(m);
	getPositions().push_back(p);
}


int HeadProcessor::convert(Token t) {
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

std::string HeadProcessor::reconvert(int p) {
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

bool HeadProcessor::checkIfIsFilePath(Token& t) {
	std::string str = t.getValue();
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

bool HeadProcessor::checkIfAllFilePathsAreFilePaths(CommandBlock& cb) {
	bool err = true;
	for (unsigned int i = 2; i < static_cast<unsigned int>(cb.getTokens().size()); i++) {
		if (cb.getTokens()[i].getTokenType() == TokenType::FILE_NAME) {
			if (!checkIfIsFilePath(cb.getTokens()[i])) {
				err &= false;
				pushException("Semantical error,file with the name: " + cb.getTokens()[i].getValue() + " isn't a text file!\n", currentPosition);
			}
		}
	}
	return err;
}

bool HeadProcessor::checkIfFileExists(Token& t) {
	std::ifstream filePath(t.getValue().c_str());
	bool b = filePath.good();
	return b;
}

bool HeadProcessor::checkAllFilePaths(CommandBlock& cb) {
	bool err = true;
	for (unsigned int i = 1; i < static_cast<unsigned int>(cb.getTokens().size()); i++) {
		if (cb.getTokens()[i].getTokenType() == TokenType::REDIRECT_OUTPUT || cb.getTokens()[i].getTokenType() == TokenType::REDIRECT_APPEND) {
			i++;
		}
		else {
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

bool HeadProcessor::checkOptionSize(Token& t) {
	bool err = true;
	std::string workingString = t.getValue(),result ="";
	for (unsigned int i = 2; i < static_cast<unsigned int>(t.getValue().size()); ++i) {
		result += workingString[i];
	}
	if (std::stoi(result) < 100000) {
		err &= true;
	}
	else {
		err &= false;
		pushException("Semantical error, command head can't have as the count of rows number with more than 5 digits!\n", currentPosition);
	}
	return err;
}

bool HeadProcessor::checkIfOptionIsAllowed(Token& t) {
	bool err = true;
	if (t.getValue().size() < 3) {
		err &= false;
		pushException("Semantical error, command head's option must have -n and then some number, it is impossible for it to not have a number!\n", currentPosition);
	}
	else {
		if (t.getValue()[0] == '-' && t.getValue()[1] == 'n') {
			for (unsigned int i = 2; i < static_cast<unsigned int>(t.getValue().size()); ++i) {
				if (!TextHandler::isNumeric(t.getValue()[i])) {
					err &= false;
					pushException("Semantical error, command head's option must have only digits after -n!\n", currentPosition);
					break;
				}
			}
			err &= checkOptionSize(t);
		}
		else {
			err &= false;
			pushException("Semantical error, command head's option must have as it's first two letters of the command - and n!\n", currentPosition);
		}
	}
	return err;
}

bool HeadProcessor::checkForDuplicates(CommandBlock& cb) {
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
			pushException("Semantical error, command head has too much tokens of type: " + reconvert(i) + "\n", currentPosition);
		}
	}
	return err;
}

bool HeadProcessor::checkMandatory(CommandBlock& cb) {
	bool err = true;
	if (cb.getTokens().size() < 2) {
		err &= false;
		pushException("Semantical error, exception no argument tokens, command head must have an option!\n", currentPosition);
	}
	else {
		if (cb.getTokens()[1].getTokenType() != TokenType::OPTION) {
			err &= false;
			pushException("Sematnical error, exception second token must be option in command head!\n ", currentPosition);
		}
		else {
			err &= checkIfOptionIsAllowed(cb.getTokens()[1]);
		}
	}
	return err;
}

bool HeadProcessor::visit(Command& command) {
	bool err = true;
	try {
		Head* head = dynamic_cast<Head*>(&command);
		if (head) {
			err &= checkMandatory(head->getCommandBlock());
			err &= checkForDuplicates(head->getCommandBlock());
			err &= checkIfAllFilePathsAreFilePaths(head->getCommandBlock());
			err &= checkAllFilePaths(head->getCommandBlock());
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

std::string& HeadProcessor::sendWholeMessage() {
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