#ifndef SEMANTIC_ANALYZER_PROCESSOR_H
#define SEMANTIC_ANALYZER_PROCESSOR_H

#include <unordered_set>
#include "SemanticAnalyzer.h"
#include "Command.h"
#include "CommandVisitor.h"
#include "SemanticAnalyzerException.h"
#include "SemanticAnalyzerVisitor.h"
#include "Visitor.h"

// Input: the SemanticAnalyzer holding the command blocks
// Output: true if valid; otherwise collected messages for each faulty block
// Responsibility: checks command names against the known commands and runs each command's own validator
class SemanticAnalyzerProcessor :public Visitor, public SemanticAnalyzerVisitor {
public:
	SemanticAnalyzerProcessor(std::vector<unsigned int> ps, std::vector<std::string> ms);
	SemanticAnalyzerProcessor(SemanticAnalyzerProcessor& sap);
	SemanticAnalyzerProcessor();

	virtual bool visit(SemanticAnalyzer& sa) override;

	std::unordered_set<std::string>& getAllowedCommands();
	std::vector<unsigned int>& getPositions() override;
	std::vector<std::string>& getMessages() override;
	
	~SemanticAnalyzerProcessor();
private:
	void setAllowedCommands();

	bool checkAndCall(Command& c, CommandVisitor& cv);
	bool checkCommandBlock(SemanticAnalyzer& sa, unsigned int& commandsIndex, unsigned int commandBlocksIndex);
	bool setCommand(SemanticAnalyzer& sa, unsigned int& commandsIndex,unsigned  int commandBlocksIndex, std::string& commandName);

	void pushException(std::string m, unsigned int p);

	std::unordered_set<std::string> allowedCommands;
};

#endif