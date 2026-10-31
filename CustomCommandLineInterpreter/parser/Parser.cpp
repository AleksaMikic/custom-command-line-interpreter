#include "Parser.h"
#include "ParserProcessor.h"
#include "VisitorException.h"

Parser::Parser() :tokens(), commandBlocks() {

}

Parser::Parser(std::vector<Token> t,std::vector<CommandBlock> cbs):tokens(t),commandBlocks(cbs){
}

void Parser::breakUp() {
	CommandBlock cb;
	for (unsigned int i = 0; i < static_cast<unsigned int>(tokens.size()); i++) {
		if (tokens[i].getTokenType() == TokenType::PIPE) {
			commandBlocks.push_back(cb);
			cb.clearTokens();
		}
		else {
			cb.addToken(tokens[i]);
		}
	}
	if (cb.getTokens().size() != 0) {
		commandBlocks.push_back(cb);
	}
}

bool Parser::accept(Visitor& v) {
	try {
		if (auto* pv = dynamic_cast<ParserProcessor*>(&v)) {
			return pv->visit(*this);
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

std::vector<CommandBlock>& Parser::parse() {
	breakUp();
	ParserProcessor pp;
	for (unsigned int i = 0; i < static_cast<unsigned int>(commandBlocks.size()); i++) {
		if (commandBlocks[i].getTokens().empty()) {
			continue;
		}
		std::string commandName = commandBlocks[i].getTokens()[0].getValue();
		bool test = commandName.size() < 10;
		for (unsigned j = 0; j < static_cast<unsigned int>(commandName.size()); j++) {
			if (!(commandName[j] >= '0' && commandName[j] <= '9') && !std::isspace(commandName[j])) {
				test = false;
			}
		}
		if (test && stoi(commandName) >= 1 && stoi(commandName) <= static_cast<int>(commandBlocks.size())) {
			commandBlocks[i] = commandBlocks[stoi(commandName)-1];
		}
	}
	if (accept(pp)) {
		return commandBlocks;
	}
	else {
		commandBlocks = {};
		return commandBlocks;
	}

}