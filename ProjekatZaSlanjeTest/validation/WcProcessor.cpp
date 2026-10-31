#include "WcProcessor.h"
#include "VisitorException.h"
#include <fstream>
#include <memory>

std::unordered_set<std::string>& WcProcessor::getAllowedOptions() {
	return allowedOptions;
}

unsigned int WcProcessor::getCurrentPosition() {
	return currentPosition;
}

std::vector<unsigned int>& WcProcessor::getPositions() {
	return Visitor::getPositions();
}

std::vector<std::string>& WcProcessor::getMessages() {
	return Visitor::getMessages();
}

void WcProcessor::setAllowedOptions() {
	allowedOptions.insert("-w");
	allowedOptions.insert("-c");
}

WcProcessor::WcProcessor(std::vector<unsigned int>& ps, std::vector<std::string>& ms, unsigned int cp) :Visitor(ps, ms), currentPosition(cp), wholeMessage() {
	setAllowedOptions();
}

WcProcessor::WcProcessor(WcProcessor& other) :Visitor(other.getPositions(), other.getMessages()), currentPosition(other.currentPosition), wholeMessage(other.wholeMessage) {
	setAllowedOptions();
}

WcProcessor::WcProcessor(unsigned int cp) :Visitor(), currentPosition(cp), wholeMessage() {
	setAllowedOptions();
}

WcProcessor::~WcProcessor() {
}

void WcProcessor::pushException(std::string m, unsigned int p) {
	getMessages().push_back(m);
	getPositions().push_back(p);
}


int WcProcessor::convert(Token t) {
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

std::string WcProcessor::reconvert(int p) {
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


bool WcProcessor::checkIfIsFilePath(Token& t) {
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

bool WcProcessor::checkIfAllFilePathsAreFilePaths(CommandBlock& cb) {
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

bool WcProcessor::checkIfFileExists(Token& t) {
	std::ifstream filePath(t.getValue().c_str());
	bool b = filePath.good();
	return b;
}

bool WcProcessor::checkAllFilePaths(CommandBlock& cb) {
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

bool WcProcessor::checkIfOptionIsAllowed(Token& t) {
	bool err = true;
	if (getAllowedOptions().count(t.getValue()) != 1) {
		err &= false;
		pushException("Semantical error, command of the type: " + t.getValue() + " doesn't exist for command Wc!\n", currentPosition);
	}
	return err;
}

bool WcProcessor::checkForDuplicates(CommandBlock& cb) {
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
			pushException("Semantical error, command Wc has too much tokens of type: " + reconvert(i) + "\n", currentPosition);
		}
	}
	return err;
}

bool WcProcessor::checkMandatory(CommandBlock& cb) {
	bool err = true;
	if (cb.getTokens().size() < 2) {
		err &= false;
		pushException("Semantical error, exception no argument tokens, command wc must have an option!\n", currentPosition);
	}
	else {
		if (cb.getTokens()[1].getTokenType() != TokenType::OPTION) {
			err &= false;
			pushException("Semantical error, exception second token must be option in command wc!\n ", currentPosition);
		}
		else {
			err &= checkIfOptionIsAllowed(cb.getTokens()[1]);
		}
	}
	return err;
}

bool WcProcessor::visit(Command& command) {
	bool err = true;
	try {
		Wc* wc = dynamic_cast<Wc*>(&command);
		if (wc) {
			err &= checkMandatory(wc->getCommandBlock());
			err &= checkForDuplicates(wc->getCommandBlock());
			err &= checkIfAllFilePathsAreFilePaths(wc->getCommandBlock());
			err &= checkAllFilePaths(wc->getCommandBlock());
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

std::string& WcProcessor::sendWholeMessage() {
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