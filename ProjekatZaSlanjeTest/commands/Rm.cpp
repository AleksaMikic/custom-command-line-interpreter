#include "Rm.h"
#include "RmProcessor.h"
#include "VisitorException.h"

Rm::Rm(std::string& str, CommandBlock& cb,int cbi,int cbs) : Command(str), commandBlock(cb), commandBlockIndex(cbi), commandBlockSize(cbs),fileName() {

}

Rm::Rm(Rm& other) :Command(other.getCommandName()), commandBlock(other.getCommandBlock()),commandBlockIndex(other.commandBlockIndex),commandBlockSize(other.commandBlockSize), fileName(other.fileName) {

}

void Rm::create() {
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

std::unique_ptr<Command> Rm::clone() {
	return std::make_unique<Rm>(*this);
}

bool Rm::accept(Visitor& v) {
	try {
		if (auto* rp = dynamic_cast<RmProcessor*>(&v)) {
			return rp->visit(*this);
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

void Rm::execute(StreamController& sc) {
	changeStreams(sc);
	try {
		std::string filePathStr = fileName.getFilePath();
		const char* filePath = filePathStr.c_str();
		if (std::remove(filePath) != 0) {
			throw CPIException("For some reason deleting the file was impossible!");
		}
	}
	catch (CPIException& e) {
		std::cerr << e.what() << "\n";
	}
	sc.setInputString("");
	sc.setOutputCout();
}