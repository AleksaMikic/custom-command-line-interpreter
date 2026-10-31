#include "Truncate.h"
#include "TruncateProcessor.h"
#include "VisitorException.h"
#include <string>

Truncate::Truncate(std::string& str, CommandBlock& cb,int cbi,int cbs) : Command(str), commandBlock(cb), commandBlockIndex(cbi),commandBlockSize(cbs), fileName() {

}

Truncate::Truncate(Truncate& other) :Command(other.getCommandName()), commandBlock(other.getCommandBlock()), commandBlockIndex(other.getCommandBlockIndex()), commandBlockSize(other.getCommandBlockSize()), fileName(other.fileName) {

}

void Truncate::create() {
	std::vector<Token>& tokens = getCommandBlock().getTokens();
	for (unsigned int i = 1; i < static_cast<unsigned int>(tokens.size()); i++) {
		if (tokens[i].getTokenType() == TokenType::FILE_NAME) {
			fileName.setFilePath(tokens[i].getValue());
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

std::unique_ptr<Command> Truncate::clone() {
	return std::make_unique<Truncate>(*this);
}

bool Truncate::accept(Visitor& v) {
	try {
		if (auto* tp = dynamic_cast<TruncateProcessor*>(&v)) {
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

void Truncate::execute(StreamController& sc) {
	changeStreams(sc);
	std::string filePath = fileName.getFilePath();
	std::ofstream file;
	file.open(filePath);
	file.close();
	sc.setInputString("");
	sc.setOutputCout();

}