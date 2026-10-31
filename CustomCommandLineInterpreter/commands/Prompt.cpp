#include "Prompt.h"
#include "PromptProcessor.h"
#include "VisitorException.h"
#include "Shell.h"

Prompt::Prompt(std::string& str, CommandBlock& cb,int cbi,int cbs): Command(str), commandBlock(cb),commandBlockIndex(cbi),commandBlockSize(cbs),
    argument(),option(){
}

Prompt::Prompt(Prompt& other) :Command(other.getCommandName()),commandBlock(other.getCommandBlock()),commandBlockIndex(other.commandBlockIndex),commandBlockSize(other.commandBlockSize), argument(other.argument), option(other.option) {

}

void Prompt::create() {
	std::vector<Token>& tokens = getCommandBlock().getTokens();
	for (unsigned int i = 1; i < static_cast<unsigned int>(tokens.size()); i++) {
		if (tokens[i].getTokenType() == TokenType::OPTION) {
			Option o(tokens[i].getValue());
			option.setValue(o);
		}
		else if (tokens[i].getTokenType() == TokenType::FILE_NAME) {
			argument.setArgument(ArgumentType::FILE_NAME_ARG, tokens[i].getValue());
		}
		else if (tokens[i].getTokenType() == TokenType::QUOTED_STRING) {
			argument.setArgument(ArgumentType::QUOTED_STRING_ARG, tokens[i].getValue());
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

std::unique_ptr<Command> Prompt::clone() {
    return std::make_unique<Prompt>(*this);
}



bool Prompt::accept(Visitor& v) {
	try {
		if (auto* pp= dynamic_cast<PromptProcessor*>(&v)) {
			return pp->visit(*this);
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


void Prompt::changeReadinessSign(Shell& s, std::string& rs) {
	s.setReadinessSign(rs);
}
void Prompt::changeTempReadinessSign(Shell& s, std::string& trs) {
	s.setTempReadinessSign(trs);
}
void Prompt::execute(StreamController& sc) {
	changeStreams(sc);
	argument.changeStream(sc);
	std::string workingString = sc.getFullInputString();
	if (this->option.checkHasValue()) {
		changeTempReadinessSign(Shell::getInstance(), workingString);
	}
	else {
		changeReadinessSign(Shell::getInstance(), workingString);
	}
	sc.setInputString("");
	sc.setOutputCout();
}
