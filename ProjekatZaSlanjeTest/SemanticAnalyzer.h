#ifndef SEMANTIC_ANALYZER_H
#define SEMANTIC_ANALYZER_H

#include "Token.h"
#include "Command.h"
#include "VisitableElement.h"
#include "VisitorException.h"
#include <string>
#include <vector>
#include <memory>

class SemanticAnalyzer :public VisitableElement{
public:
	SemanticAnalyzer();
	SemanticAnalyzer(std::vector<CommandBlock>& cbs);
	SemanticAnalyzer(SemanticAnalyzer& other);

	bool accept(Visitor& v) override;

	void setCommandBlocks(std::vector<CommandBlock>& cbs) { commandBlocks = cbs; }
	void setCommands(std::vector<std::unique_ptr<Command>>&& cs) {commands= std::move(cs); }
	std::vector <std::unique_ptr< Command >>& getCommands() { return commands; }
	std::vector<CommandBlock>& getCommandBlocks() { return commandBlocks; }
	std::vector<std::unique_ptr<Command>> takeCommands() { return std::move(commands); }
	void clearCommands() { commands.clear(); }

	std::vector<std::unique_ptr<Command>>& analyze();
private:
	std::vector<CommandBlock> commandBlocks;
	std::vector<std::unique_ptr<Command>> commands;
};

#endif