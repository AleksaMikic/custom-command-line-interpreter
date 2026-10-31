#include "Batch.h"
#include "BatchProcessor.h"
#include "VisitorException.h"
#include "Shell.h"

Batch::Batch(std::string& str, CommandBlock& cb,int cbi,int cbs) :Command(str), argument(), commandBlock(cb),commandBlockIndex(cbi),commandBlockSize(cbs) {

}

Batch::Batch(Batch& other) :Command(other.getCommandName()), argument(other.argument), commandBlock(other.commandBlock),commandBlockIndex(other.commandBlockIndex),
	commandBlockSize(other.commandBlockSize){

}

void Batch::create() {
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

std::unique_ptr<Command> Batch::clone() {
	return std::make_unique<Batch>(*this);
}

bool Batch::accept(Visitor& v) {
	try {
		if (auto* bp = dynamic_cast<BatchProcessor*>(&v)) {
			return bp->visit(*this);
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

std::vector<std::string> Batch::breakUp(std::string& str) {
	std::vector<std::string> inputs;
	std::string temp = "";
	for (unsigned int i = 0; i < static_cast<unsigned int>(str.size()); i++) {
		if (str[i] == '\n') {
			inputs.push_back(temp);
			temp = "";
		}
		else {
			temp += str[i];
		}
	}
	if (static_cast<unsigned int>(temp.size()) > 0) {
		inputs.push_back(temp);
	}
	return inputs;
}

void Batch::executeSubShell(std::vector<std::string> strings, std::stack<std::unique_ptr<Shell>>& instances) {
	instances.push(std::make_unique<Shell>());
	Shell::getInstance().run(strings);
	instances.pop();
}

void Batch::execute(StreamController& sc) {
	changeStreams(sc);
	if (argument.checkHasValue()) {
		argument.getValue()->changeStream(sc);
	}
	std::string workingString = sc.getFullInputString();
	executeSubShell(breakUp(workingString), Shell::getInstances());
	sc.setInputString("");
	sc.setOutputCout();
}