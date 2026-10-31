#include "TrProcessor.h"
#include "VisitorException.h"
#include <fstream>
#include <memory>

unsigned int TrProcessor::getCurrentPosition() {
	return currentPosition;
}

std::vector<unsigned int>& TrProcessor::getPositions() {
	return Visitor::getPositions();
}

std::vector<std::string>& TrProcessor::getMessages() {
	return Visitor::getMessages();
}

TrProcessor::TrProcessor(std::vector<unsigned int>& ps, std::vector<std::string>& ms, unsigned int cp) :Visitor(ps, ms), currentPosition(cp), wholeMessage() {
}

TrProcessor::TrProcessor(TrProcessor& other) :Visitor(other.getPositions(), other.getMessages()), currentPosition(other.currentPosition), wholeMessage(other.wholeMessage) {

}

TrProcessor::TrProcessor(unsigned int cp) :Visitor(), currentPosition(cp), wholeMessage() {
}

TrProcessor::~TrProcessor() {
}

void TrProcessor::pushException(std::string m, unsigned int p) {
	getMessages().push_back(m);
	getPositions().push_back(p);
}

int TrProcessor::convert(Token t) {
	if (t.getTokenType() == COMMAND) {
		return 0;
	}
	else if (t.getTokenType() == OPTION) {
		return 1;
	}
	else if (t.getTokenType() == FILE_NAME) {
		return 2;
	}
	else if (t.getTokenType() == QUOTED_STRING) {
		return 3;
	}
	else if (t.getTokenType() == REDIRECT_INPUT) {
		return 4;
	}
	else {
		return 5;
	}
}


std::string TrProcessor::reconvert(int p) {
	if (p == 0) {
		return "COMMAND";
	}
	else if (p == 1) {
		return "OPTION";
	}
	else if (p == 2|| p==3) {
		return "ARGUMENT";
	}
	else if (p == 4) {
		return "REDIRECT_INPUT";
	}
	else {
		return "REDIRECT_OUTPUT";
	}
}

bool TrProcessor::checkIfIsFilePath(Token& t) {
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

bool TrProcessor::checkIfAllFilePathsAreFilePaths(CommandBlock& cb) {
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

bool TrProcessor::checkIfFileExists(Token& t) {
	std::ifstream filePath(t.getValue().c_str());
	bool b = filePath.good();
	return b;
}

bool TrProcessor::checkAllFilePaths(CommandBlock& cb) {
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

bool TrProcessor::checkForDuplicates(CommandBlock& cb) {
	bool err = true;
	int j = 0;
	std::vector<int> v;
	std::vector<int> map(6, 0);
	for (unsigned int i = 0; i < static_cast<unsigned int>(cb.getTokens().size()); i++) {
		v.push_back(convert(cb.getTokens()[i]));
		++map[v[j]];
		j++;
		if (convert(cb.getTokens()[i]) >= 4) {
			i++;
		}
	}
	for (unsigned int i = 0; i < static_cast<unsigned int>(map.size()); i++) {
		if (i != 3) {
			if (map[i] > 1) {
				err &= false;
				pushException("Semantical error, command tr has too much tokens of type: " + reconvert(i) + "\n", currentPosition);
			}
		}
		else {
			if (map[2] + map[3] > 3) {
				err &= false;
				pushException("Semantical error, command tr has too much tokens of type: " + reconvert(i) + "\n", currentPosition);
			}
		}
	}
	return err;
}

bool TrProcessor::checkForSequence(CommandBlock& cb) {
	std::vector<int> v;
	for (unsigned int i = 0; i < static_cast<unsigned int>(cb.getTokens().size()); i++) {
		if (convert(cb.getTokens()[i]) > 3) {
			v.push_back(convert(cb.getTokens()[i]));
			i++;
		}
		else {
			v.push_back(convert(cb.getTokens()[i]));
		}
	}
	unsigned int size = static_cast<unsigned int>(v.size());
	bool err = true;
	if (size > 1) {
		for (unsigned int i = 1; i < size; i++) {
			if (v[i] < v[i - 1]) {
				err = false;
				pushException("Exception in sequencing of tokens in command block, token of type " + reconvert(v[i - 1]) + " is before token of type " + reconvert(v[i]) + "\n", currentPosition);
			}
		}
	}
	return err;
}

unsigned int TrProcessor::convertSize(CommandBlock& cb) {
	unsigned int size = static_cast<unsigned int>(cb.getTokens().size());
	for (unsigned int i = 0; i < static_cast<unsigned int>(cb.getTokens().size()); i++) {
		if (convert(cb.getTokens()[i]) > 3) {
			i++;
			size -= 2;
		}
	}
	return size;
}

bool TrProcessor::checkMandatory(CommandBlock& cb) {
	bool err = true;
	int convertedSize = convertSize(cb);
	if (convertedSize < 2) {
		err &= false;
		pushException("Semantical error, exception no argument tokens, command tr must have an quoted string of type what!\n", currentPosition);
	}
	else {
		if (cb.getTokens()[1].getTokenType() == TokenType::FILE_NAME) {
			if (convertedSize < 3) {
				err &= false;
				pushException("Semantical error, if second token is file name,than command tr must have third token quoted string of type what!\n ", currentPosition);
			}
			else {
				if (cb.getTokens()[2].getTokenType() != TokenType::QUOTED_STRING) {
					err &= false;
					pushException("Semantical error, if second token is file name, exception third token must be quoted string in command tr!\n ", currentPosition);
				}
			}
		}
		else if (cb.getTokens()[1].getTokenType() != TokenType::QUOTED_STRING) {
			err &= false;
			pushException("Semantical error, second error in case it isn't file name, must be quoted string of type what!\n ", currentPosition);
		}
	}
	return err;
}

bool TrProcessor::visit(Command& command) {
	bool err = true;
	try {
		Tr* tr = dynamic_cast<Tr*>(&command);
		if (tr) {
			err &= checkMandatory(tr->getCommandBlock());
			err &= checkForDuplicates(tr->getCommandBlock());
			err &= checkForSequence(tr->getCommandBlock());
			err &= checkIfAllFilePathsAreFilePaths(tr->getCommandBlock());
			err &= checkAllFilePaths(tr->getCommandBlock());
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

std::string& TrProcessor::sendWholeMessage() {
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