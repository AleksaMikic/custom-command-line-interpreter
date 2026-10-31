#include "Wc.h"
#include "WcProcessor.h"
#include "VisitorException.h"

Wc::Wc(std::string& str, CommandBlock& cb,int cbi,int cbs) :Command(str),option(), argument(), commandBlock(cb),commandBlockIndex(cbi),commandBlockSize(cbs){

}

Wc::Wc(Wc& other) :Command(other.getCommandName()), option(other.option) ,argument(other.argument), commandBlock(other.commandBlock), commandBlockIndex(other.commandBlockIndex),commandBlockSize(other.commandBlockSize){

}

void Wc::create() {
	std::vector<Token>& tokens = getCommandBlock().getTokens();
	for (unsigned int i = 1; i < static_cast<unsigned int>(tokens.size()); i++) {
		if (tokens[i].getTokenType() == TokenType::OPTION) {
			option.setOption(tokens[i].getValue());
		}
		else if (tokens[i].getTokenType() == TokenType::FILE_NAME) {
			Argument a(ArgumentType::FILE_NAME_ARG, tokens[i].getValue());
			argument.setValue(a);
		}
		else if (tokens[i].getTokenType() == TokenType::QUOTED_STRING) {
			Argument a(ArgumentType::QUOTED_STRING_ARG, tokens[i].getValue());
			argument.setValue(a);
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

std::unique_ptr<Command> Wc::clone() {
	return std::make_unique<Wc>(*this);
}

bool Wc::accept(Visitor& v) {
	try {
		if (auto* wp = dynamic_cast<WcProcessor*>(&v)) {
			return wp->visit(*this);
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

std::string Wc::executeOptionW(std::string& str) {
	bool turn = true;
	int counter = 0;
	for (unsigned int i = 0; i < static_cast<unsigned int>(str.size()); i++) {
		if (!std::isspace(str[i]) && turn) {
			turn = false;
			counter++;
		}
		if (std::isspace(str[i]) && !turn) {
			turn = true;
		}
	}
	return std::to_string(counter);
}

std::string Wc::executeOptionC(std::string & str) {
	return std::to_string(static_cast<int>(str.size()));
}

std::string Wc::executeOptions(std::string& str) {
	if (commandBlock.getTokens()[1].getValue() == "-w") {
		return executeOptionW(str);
	}
	else if (commandBlock.getTokens()[1].getValue() == "-c") {
		return executeOptionC(str);
	}
	return "";
}

void Wc::execute(StreamController& sc){
	changeStreams(sc);
	if (argument.checkHasValue()) {
		argument.getValue()->changeStream(sc);
	}
	std::string workingString = sc.getFullInputString();
	std::string resultString = executeOptions(workingString);
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