#include "Tr.h"
#include "TrProcessor.h"
#include "VisitorException.h"

Tr::Tr(std::string& str, CommandBlock& cb,int cbi,int cbs):Command(str), argument(),what(),with(),commandBlock(cb),commandBlockIndex(cbi),commandBlockSize(cbs){

}

Tr::Tr(Tr& other) :Command(other.getCommandName()), argument(other.argument), what(other.what), with(other.with), commandBlock(other.commandBlock), commandBlockIndex(other.commandBlockIndex),commandBlockSize(other.commandBlockSize){

}

int Tr::countQuotedStrings() {
	std::vector<Token>& tokens = getCommandBlock().getTokens();
	int counter = 0;
	for (unsigned int i = 1; i < static_cast<unsigned int>(tokens.size()); ++i) {
		if (tokens[i].getTokenType() == TokenType::QUOTED_STRING) {
			counter++;
		}
	}
	return counter;
}

void Tr::createWithThreeQuotedStrings() {
	std::vector<Token>& tokens = getCommandBlock().getTokens();
	int counter = 0;
	for (unsigned int i = 1; i < static_cast<unsigned int>(tokens.size()); i++) {
		if (tokens[i].getTokenType() == TokenType::QUOTED_STRING && counter == 0) {
			Argument a(ArgumentType::QUOTED_STRING_ARG, tokens[i].getValue());
			argument.setValue(a);
			counter++;
		}
		else if (tokens[i].getTokenType() == TokenType::QUOTED_STRING && counter == 1) {
			what.setValue(QuotedStringType::WHAT, tokens[i].getValue());
			counter++;
		}
		else if (tokens[i].getTokenType() == TokenType::QUOTED_STRING && counter == 2) {
			QuotedString qs(QuotedStringType::WITH, tokens[i].getValue());
			with.setValue(qs);
		}
		else if (tokens[i].getTokenType() == TokenType::REDIRECT_INPUT) {
			Redirect ri(RedirectType::INPUT, tokens[i + 1].getValue());
			getInput().setValue(ri);
			i++;
		}
		else if (tokens[i].getTokenType() == TokenType::REDIRECT_OUTPUT) {
			Redirect ro(RedirectType::OUTPUT, tokens[i + 1].getValue());
			getOutput().setValue(ro);
			i++;
		}
		else if (tokens[i].getTokenType() == TokenType::REDIRECT_APPEND) {
			Redirect ro(RedirectType::APPEND, tokens[i + 1].getValue());
			getOutput().setValue(ro);
			i++;
		}
	}
}

void Tr::createWithoutThreeQuotedStrings() {
	std::vector<Token>& tokens = getCommandBlock().getTokens();
	int counter = 0;
	for (unsigned int i = 1; i < static_cast<unsigned int>(tokens.size()); i++) {
		if (tokens[i].getTokenType() == TokenType::FILE_NAME) {
			Argument a(ArgumentType::FILE_NAME_ARG, tokens[i].getValue());
			argument.setValue(a);
		}
		else if (tokens[i].getTokenType() == TokenType::QUOTED_STRING && counter == 0) {
			what.setValue(QuotedStringType::WHAT, tokens[i].getValue());
			counter++;
		}
		else if (tokens[i].getTokenType() == TokenType::QUOTED_STRING && counter == 1) {
			QuotedString qs(QuotedStringType::WITH, tokens[i].getValue());
			with.setValue(qs);
		}
		else if (tokens[i].getTokenType() == TokenType::REDIRECT_INPUT) {
			Redirect ri(RedirectType::INPUT, tokens[i + 1].getValue());
			getInput().setValue(ri);
			i++;
		}
		else if (tokens[i].getTokenType() == TokenType::REDIRECT_OUTPUT) {
			Redirect ro(RedirectType::OUTPUT, tokens[i + 1].getValue());
			getOutput().setValue(ro);
			i++;
		}
		else if (tokens[i].getTokenType() == TokenType::REDIRECT_APPEND) {
			Redirect ro(RedirectType::APPEND, tokens[i + 1].getValue());
			getOutput().setValue(ro);
			i++;
		}
	}
}

void Tr::create() {
	int numOfQuotedStrings = countQuotedStrings();
	if (numOfQuotedStrings == 3) {
		createWithThreeQuotedStrings();
	}
	else {
		createWithoutThreeQuotedStrings();
	}
}

std::unique_ptr<Command> Tr::clone() {
	return std::make_unique<Tr>(*this);
}

bool Tr::accept(Visitor& v) {
	try {
		if (auto* tp = dynamic_cast<TrProcessor*>(&v)) {
			return tp->visit(*this);
		}
		else {
			throw VisitorException("Wrong visitor\n");
		}
	}
	catch (VisitorException& e) {
		std::cerr << e.what() << "\n";
	}
	return false;
}

std::string Tr::changeOnlyWhat(std::string& str, std::string& what) {
	std::string result;
	unsigned int i = 0;
	unsigned int sizeStr = static_cast<unsigned int>(str.size());
	unsigned int sizeWhat = static_cast<unsigned int>(what.size());
	while (i < sizeStr) {
		bool matched = true;
		for(unsigned int j = 0; j < sizeWhat; ++j) {
			if (i + j >= sizeStr) {
				matched = false;
				break;
			}
			if (str[i + j] != what[j]) {
				matched = false;
				break;
			}
		}
		if (matched) {
			i += sizeWhat;
		}
		else {
			result += str[i];
			++i;
		}
	}
	return result;
}

std::string Tr::changeWhatWith(std::string& str, std::string& what, std::string& with) {
	std::string result;
	unsigned int i = 0;
	unsigned int sizeStr = static_cast<unsigned int>(str.size());
	unsigned int sizeWhat = static_cast<unsigned int>(what.size());
	while (i < sizeStr) {
		bool matched = true;
		for (unsigned int j = 0; j < sizeWhat; ++j) {
			if (i + j >= sizeStr) {
				matched = false;
				break;
			}
			if (str[i + j] != what[j]) {
				matched = false;
				break;
			}
		}
		if (matched) {
			i += sizeWhat;
			result += with;
		}
		else {
			result += str[i];
			++i;
		}
	}
	return result;
}

void Tr::execute(StreamController& sc) {
	changeStreams(sc);
	if (argument.checkHasValue()) {
		argument.getValue()->changeStream(sc);
	}
	std::string workingString = sc.getFullInputString();
	std::string resultString = (with.checkHasValue()) ? changeWhatWith(workingString, what.getValue(), with.getValue()->getValue()) : changeOnlyWhat(workingString, what.getValue());
	sc.setOutputCapture(true);
	if ((sc.getOutput().getStreamType() == StreamType::CONSOLE_STREAM && this->getCommandBlockIndex() == this->getCommandBlockSize() - 1)
		|| sc.getOutput().getStreamType() != StreamType::CONSOLE_STREAM) {
		sc.getOutput() << resultString;
		sc.getOutput().closeIfFile();
		sc.getOutput() << "\n";
	}
	sc.setInputString(resultString);
	sc.setOutputCout();
}