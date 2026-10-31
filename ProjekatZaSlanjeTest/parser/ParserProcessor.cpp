#include "ParserProcessor.h"


std::vector<unsigned int>& ParserProcessor::getPositions() {
	return Visitor::getPositions();
}

std::vector<std::string>& ParserProcessor::getMessages() {
	return Visitor::getMessages();
}
ParserProcessor::ParserProcessor(std::vector<unsigned int> ps, std::vector<std::string> ms) :Visitor(ps, ms) {

}

ParserProcessor::ParserProcessor(ParserProcessor& pp) :Visitor(pp.getPositions(), pp.getMessages()) {

}



ParserProcessor::~ParserProcessor() {
}

void ParserProcessor::pushException(std::string m, unsigned int p) {
	getMessages().push_back(m);
	getPositions().push_back(p);
}

bool ParserProcessor::checkRedirection(CommandBlock cb, unsigned int p, unsigned int i) {
	if (p + 1 >= cb.getTokens().size()) {
		pushException("Exception in redirection pair in this command block, no FILE_NAME token\n", i);
		return false;
	}
	else {
		if (cb.getTokens()[p + 1].getTokenType() != TokenType::FILE_NAME) {
			pushException("Exception in redirection pair in this command block, second token isn't FILE_NAME\n", i);
			return false;
		}
		return true;
	}
}

int ParserProcessor::convert(Token t) {
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

std::string ParserProcessor::reconvert(int p) {
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

bool ParserProcessor::checkForDuplicates(std::vector<int> v, unsigned int p) {
	std::vector<int> map(5, 0);
	bool e = true;
	for (unsigned int i = 0; i < static_cast<unsigned int>(v.size()); i++) {
		map[v[i]]++;
	}
	for (unsigned int i = 0; i < 5; i++) {
		if (i != 2) {
			if (map[i] > 1) {
				e = false;
				pushException("Exception in command block, more than 1 tokens of the same type " + reconvert(i) + "\n", p);
			}
		}
		else {
			if (map[i] > 3) {
				e = false;
				pushException("Exception in command block, more than 3 tokens of the same type " + reconvert(i) + "\n", p);
			}
		}
	}
	return e;
}

bool ParserProcessor::checkForSequence(std::vector<int> v, unsigned int p) {
	unsigned int size = static_cast<unsigned int>(v.size());
	bool e = true;
	if (size > 1) {
		for (unsigned int i = 1; i < size; i++) {
			if (v[i] < v[i - 1]) {
				e = false;
				pushException("Exception in sequencing of tokens in command block, token of type " + reconvert(v[i - 1]) + " is before token of type " + reconvert(v[i]) + "\n", p);
			}
		}
	}
	return e;
}

bool ParserProcessor::checkCommandBlock(CommandBlock cb, unsigned int p) {
	bool err = true;
	if (cb.getTokens().size() == 0) {
		err &= false;
		pushException("Exception no tokens in command block \n", p);
	}
	else {
		if (cb.getTokens()[0].getTokenType() != TokenType::COMMAND) {
			err &= false;
			pushException("Exception first token must always be command \n", p);
		}
		std::vector<int> vec = {};
		for (unsigned int i = 0; i < static_cast<unsigned int>(cb.getTokens().size()); i++) {
			int val = convert(cb.getTokens()[i]);
			if (val == 3 || val == 4) {
				if (checkRedirection(cb, i, p)) {
					i++;
				}
				else {
					err &= false;
				}
			}
			vec.push_back(val);
		}
		err &= checkForDuplicates(vec, p);
		err &= checkForSequence(vec, p);
	}
	return err;
}

bool ParserProcessor::visit(Parser &p) {
	bool err = true;
	try {
		for (unsigned int i = 0; i < static_cast<unsigned int>(p.getCommandBlocks().size());i++) {
			err &= checkCommandBlock(p.getCommandBlocks()[i], i);
		}
		if (!err) {
			throw ParserException("Error - parsing problems:", getPositions(), p.getCommandBlocks(), getMessages());
		}
		else {
			return err;
		}
	}
	catch (ParserException& e) {
		std::cerr << e.what() << "\n";
	}
	return err;
}