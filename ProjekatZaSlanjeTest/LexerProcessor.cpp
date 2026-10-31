#include "LexerProcessor.h"
#include "TextHandler.h"

std::vector<unsigned int>& LexerProcessor::getPositions() {
	return Visitor::getPositions();
}

std::vector<std::string>& LexerProcessor::getMessages() {
	return Visitor::getMessages();
}

LexerProcessor::LexerProcessor(std::vector<unsigned int> ps, std::vector<std::string> ms) :Visitor(ps,ms){

}

LexerProcessor::LexerProcessor(LexerProcessor& lp) :Visitor(lp.getPositions(), lp.getMessages()) {

}

LexerProcessor::~LexerProcessor() {

}

void LexerProcessor::resizeInput(Lexer& l) {
	if (static_cast<unsigned int>(l.getInput().size()) > 512) {
		l.getInput().resize(512);
	}
}

bool LexerProcessor::processLine(Lexer& l) {
	resizeInput(l);
	bool isInDoubleQuotes = false, err = true;
	char c;
	for (unsigned int i = 0; i < static_cast<unsigned int>(l.getInput().size()); i++) {
		c = l.getInput()[i];
		if (isInDoubleQuotes) {
			if (c == '"') {
				isInDoubleQuotes = false;
			}
		}
		else {
			if (c == '"') {
				isInDoubleQuotes = true;
			}
			else if (!TextHandler::isAllowedCharacter(c)) {
				err = false;
				getPositions().push_back(i);
			}
		}
	}
	//Slucaj kada sa " nije zavrsen string
	if (isInDoubleQuotes) {
		err = false;
		getPositions().push_back(static_cast<unsigned int>(l.getInput().size()));
	}
	return err;
}

bool LexerProcessor::visit(Lexer& l) {
	bool err = true;
	try {
		err = processLine(l);
		if (!err) {
			throw LexerException("Error - unexpected characters:", getPositions(), l.getInput());
		}
		else {
			return err;
		}
	}
	catch (const LexerException& e) {
		std::cerr << e.what() << "\n";
	}
	return err;
}