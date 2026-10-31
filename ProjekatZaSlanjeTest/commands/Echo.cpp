#include "Echo.h"
#include "EchoProcessor.h"
#include "VisitorException.h"

Echo::Echo(std::string& str,CommandBlock& cb,int cbi,int cbs) :Command(str),argument(), commandBlock(cb), commandBlockIndex(cbi),commandBlockSize(cbs){

}

Echo::Echo(Echo& other) :Command(other.getCommandName()), argument(other.argument), commandBlock(other.commandBlock) ,commandBlockIndex(other.commandBlockIndex),commandBlockSize(other.commandBlockSize){

}

void Echo::create() {
	std::vector<Token>& tokens = getCommandBlock().getTokens();
	for (unsigned int i = 1; i < static_cast<unsigned int>(tokens.size()); i++) {
		if (tokens[i].getTokenType() == TokenType::FILE_NAME) {
			Argument a(ArgumentType::FILE_NAME_ARG, tokens[i].getValue());
			argument.setValue(a);
		}
		else if (tokens[i].getTokenType() == TokenType::QUOTED_STRING) {
			Argument a(ArgumentType::QUOTED_STRING_ARG, tokens[i].getValue());
			argument.setValue(a);
		}
		else if (tokens[i].getTokenType() == TokenType::REDIRECT_INPUT) {
			Redirect ri(RedirectType::INPUT,tokens[i+1].getValue());
			getInput().setValue(ri);
			i++;
		}
		else if (tokens[i].getTokenType() == TokenType::REDIRECT_OUTPUT) {
			Redirect ro(RedirectType::OUTPUT, tokens[i+1].getValue());
			getOutput().setValue(ro);
			i++;
		}
		else if (tokens[i].getTokenType() == TokenType::REDIRECT_APPEND) {
			Redirect ro(RedirectType::APPEND, tokens[i+1].getValue());
			getOutput().setValue(ro);
			i++;
		}
	}
}

std::unique_ptr<Command> Echo::clone() {
	return std::make_unique<Echo>(*this);
}

bool Echo::accept(Visitor& v) {
	try {
		if (auto* ep = dynamic_cast<EchoProcessor*>(&v)) {
			return ep->visit(*this);
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

void Echo::execute(StreamController& sc) {
	changeStreams(sc);
	if (argument.checkHasValue()) {
		argument.getValue()->changeStream(sc);
	}
	std::string workingString = sc.getFullInputString();
	sc.setOutputCapture(true);
	if ((sc.getOutput().getStreamType() == StreamType::CONSOLE_STREAM && this->getCommandBlockIndex() == this->getCommandBlockSize() - 1)
		|| sc.getOutput().getStreamType() != StreamType::CONSOLE_STREAM) {
		sc.getOutput() << workingString;
		sc.getOutput() << "\n";
		sc.getOutput().closeIfFile();
	}
	sc.setInputString(workingString);
	sc.setOutputCout();
}