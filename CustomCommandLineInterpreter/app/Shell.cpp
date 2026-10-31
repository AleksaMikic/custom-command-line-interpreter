#include "Shell.h"

std::stack<std::unique_ptr<Shell>> Shell::instances;

Shell::Shell() :lexer(), parser(), semanticAnalyzer(), executor(), readinessSign("$") {

}
Shell::Shell(std::string& rs) :lexer(), parser(), semanticAnalyzer(), executor(), readinessSign(rs) {

}

Shell& Shell::getInstance() {
	if (instances.empty()) {
		instances.push(std::make_unique<Shell>());
	}
	return *instances.top();
}

std::stack<std::unique_ptr<Shell>>& Shell::getInstances() {
	return instances;
}

void Shell::run() {
	while (1) {
		std::string input;
		if (isTempOn) {
			std::cout << tempReadinessSign << " ";
			isTempOn = false;
		}
		else {
			std::cout << readinessSign << " ";
		}
		if (!std::getline(std::cin, input) || input == "exit") {
			break;
		}
		lexer.setInput(input);
		std::vector<Token> tokens = lexer.tokenize();
		parser.setTokens(tokens);
		parser.resetCommandBlocks();
		std::vector<CommandBlock> commandBlocks = parser.parse();
		semanticAnalyzer.setCommandBlocks(commandBlocks);
		semanticAnalyzer.clearCommands();
		std::vector<std::unique_ptr<Command>> commands=std::move(semanticAnalyzer.analyze());
		executor.setCommands(std::move(commands));
		executor.execute();
	}
}

void Shell::run(std::vector<std::string>& inputs) {
	for (unsigned int i = 0; i < static_cast<unsigned int>(inputs.size()); i++) {
		std::string input = inputs[i];
		lexer.setInput(input);
		std::vector<Token> tokens = lexer.tokenize();
		parser.setTokens(tokens);
		parser.resetCommandBlocks();
		std::vector<CommandBlock> commandBlocks = parser.parse();
		semanticAnalyzer.setCommandBlocks(commandBlocks);
		semanticAnalyzer.clearCommands();
		std::vector<std::unique_ptr<Command>> commands = std::move(semanticAnalyzer.analyze());
		executor.setCommands(std::move(commands));
		executor.execute();
	}
}
