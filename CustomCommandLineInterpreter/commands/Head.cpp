#include "Head.h"
#include "HeadProcessor.h"
#include "VisitorException.h"

Head::Head(std::string& str, CommandBlock& cb,int cbi,int cbs) :Command(str), option(), argument(), commandBlock(cb),commandBlockIndex(cbi),commandBlockSize(cbs){

}

Head::Head(Head& other) : Command(other.getCommandName()), option(other.option), argument(other.argument), commandBlock(other.commandBlock) ,commandBlockIndex(other.commandBlockIndex), commandBlockSize(other.commandBlockSize){

}

void Head::create() {
	std::vector<Token>& tokens = getCommandBlock().getTokens();
	for (unsigned int i = 1; i < static_cast<unsigned int> (tokens.size()); i++) {
		if (tokens[i].getTokenType() == TokenType::OPTION) {
			option.setOption(tokens[i].getValue());
		}
		else if (tokens[i].getTokenType() == TokenType::FILE_NAME){
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
			++i;
		}
		else if (tokens[i].getTokenType() == TokenType::REDIRECT_OUTPUT) {
			Redirect ro(RedirectType::OUTPUT, tokens[i + 1].getValue());
			getOutput().setValue(ro);
			++i;
		}
		else if (tokens[i].getTokenType() == TokenType::REDIRECT_APPEND) {
			Redirect ra(RedirectType::APPEND, tokens[i + 1].getValue());
			getOutput().setValue(ra);
			++i;
		}
	}
}

std::unique_ptr<Command> Head::clone() {
	return std::make_unique<Head>(*this);
}

bool Head::accept(Visitor& v) {
	try {
		if (auto* hv = dynamic_cast<HeadProcessor*>(&v)) {
			return hv->visit(*this);
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

int Head::extract() {
	std::string str = option.getValue(),result="";
	for (unsigned int i = 2; i < static_cast<unsigned int>(str.size()); i++) {
		result += str[i];
	}
	return std::stoi(result);
}

std::string Head::transform(int size, std::string& workingString) {
	std::string result;
	result.reserve(workingString.size());
	int curSize = size;
	for (unsigned int i = 0; i < static_cast<unsigned int>(workingString.size()); i++) {
		if (workingString[i] == '\n') {
			curSize--;
			if (curSize == 0) {
				break;
			}
			else {
				result += workingString[i];
			}
		}
		else {
			result += workingString[i];
		}
	}
	return result;
}

void Head::execute(StreamController& sc) {
	changeStreams(sc);
	if (argument.checkHasValue()) {
		argument.getValue()->changeStream(sc);
	}
	std::string workingString = sc.getFullInputString();
	std::string resultString = transform(extract(),workingString);
	sc.setOutputCapture(true);
	if ((sc.getOutput().getStreamType() == StreamType::CONSOLE_STREAM && this->getCommandBlockIndex() == this->getCommandBlockSize() - 1)
		|| sc.getOutput().getStreamType() != StreamType::CONSOLE_STREAM) {
		sc.getOutput() << resultString;
		sc.getOutput() << "\n";
		sc.getOutput().closeIfFile();
	}
	sc.setInputString(resultString);
	sc.setOutputCout();
}
