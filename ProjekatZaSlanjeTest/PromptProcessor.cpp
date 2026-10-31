#include "PromptProcessor.h"
#include "VisitorException.h"
#include <fstream>
#include <memory>

unsigned int PromptProcessor::getCurrentPosition() {
	return currentPosition;
}

std::vector<unsigned int>& PromptProcessor::getPositions() {
	return Visitor::getPositions();
}

std::vector<std::string>& PromptProcessor::getMessages() {
	return Visitor::getMessages();
}

PromptProcessor::PromptProcessor(std::vector<unsigned int>& ps, std::vector<std::string>& ms, unsigned int cp):Visitor(ps,ms),currentPosition(cp),wholeMessage() {

}

PromptProcessor::PromptProcessor(PromptProcessor& other) :Visitor(other.getPositions(), other.getMessages()), currentPosition(other.currentPosition), wholeMessage(other.wholeMessage) {

}

PromptProcessor::PromptProcessor(unsigned int cp) :Visitor(),currentPosition(cp), wholeMessage() {

}

PromptProcessor::~PromptProcessor() {

}

void PromptProcessor::pushException(std::string m,unsigned int p) {
	getMessages().push_back(m);
	getPositions().push_back(p);
}

int PromptProcessor::convert(Token& t) {
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

std::string PromptProcessor::reconvert(int p) {
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

bool PromptProcessor::checkAllowedToken(Token& t) {
	if (t.getTokenType() == TokenType::OPTION) {
		if (t.getValue() != "-t") {
			return false;
		}
		else {
			return true;
		}
	}
	else {
		return true;
	}
}

bool PromptProcessor::checkAllowedTokens(CommandBlock& cb) {
	bool err = true;
	for (unsigned int i = 0; i < static_cast<unsigned int>(cb.getTokens().size()); i++) {
		if (!checkAllowedToken(cb.getTokens()[i])) {
			err &= false;
			pushException("Semantical error, command Prompt doesn't support options\n", i);
		}
		if (convert(cb.getTokens()[i]) >= 3) {
			i++;
		}
	}
	return err;
}

bool PromptProcessor::checkIfIsFilePath(Token& t) {
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

bool PromptProcessor::checkIfAllFilePathsAreFilePaths(CommandBlock& cb) {
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

bool PromptProcessor::checkIfFileExists(Token& t) {
	std::ifstream filePath(t.getValue().c_str());
	bool b = filePath.good();
	return b;
}

bool PromptProcessor::checkAllFilePaths(CommandBlock& cb) {
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

bool PromptProcessor::checkForDuplicates(CommandBlock& cb) {
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
			pushException("Semantical error, command Prompt has too much tokens of type: " + reconvert(i) + "\n", currentPosition);
		}
	}
	return err;
}

bool PromptProcessor::checkMandatory(CommandBlock& cb) {
	bool err = true;
	if (cb.getTokens().size() <2) {
		err &= false;
		pushException("Semantical error, exception no argument tokens, command Prompt must have an argument\n", currentPosition);
	}
	else {
		if (convert(cb.getTokens()[1]) != 2) {
			if (cb.getTokens().size() >= 3) {
				if (convert(cb.getTokens()[2]) != 2) {
					err &= false;
					pushException("Sematnical error, exception second or third token must be argument in command Prompt\n ", currentPosition);
				}
			}
			else {
				err &= false;
				pushException("Sematnical error, exception second or third token must be argument in command Prompt\n ", currentPosition);
			}
		}
	}
	return err;
}

bool PromptProcessor::visit(Command& command) {
	bool err = true;
	try {
		Prompt* prompt = dynamic_cast<Prompt*>(&command);
		if (prompt) {
			err &= checkMandatory(prompt->getCommandBlock());
			err &= checkAllowedTokens(prompt->getCommandBlock());
			err &= checkForDuplicates(prompt->getCommandBlock());
			err &= checkIfAllFilePathsAreFilePaths(prompt->getCommandBlock());
			err &= checkAllFilePaths(prompt->getCommandBlock());
		}
		else{
			throw VisitorException("Wrong visitor!\n");
		}
	}
	catch (VisitorException& e) {
		std::cerr << e.what() << "\n";
		return false;
	}
	return err;
}

std::string& PromptProcessor::sendWholeMessage() {
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