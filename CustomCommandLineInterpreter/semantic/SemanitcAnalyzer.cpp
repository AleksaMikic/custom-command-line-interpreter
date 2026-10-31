#include "SemanticAnalyzer.h"
#include "SemanticAnalyzerProcessor.h"

SemanticAnalyzer::SemanticAnalyzer() :commandBlocks(), commands() {

}

SemanticAnalyzer::SemanticAnalyzer(std::vector<CommandBlock>& cbs):commandBlocks(cbs),commands() {

}
SemanticAnalyzer::SemanticAnalyzer(SemanticAnalyzer& other) :commandBlocks(other.commandBlocks), commands(std::move(other.commands)) {

}

bool SemanticAnalyzer::accept(Visitor& v) {
	try {
		if (auto* sap = dynamic_cast<SemanticAnalyzerProcessor*>(&v)) {
			return sap->visit(*this);
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

std::vector<std::unique_ptr<Command>>& SemanticAnalyzer::analyze() {
	SemanticAnalyzerProcessor semanticAnalyzerProcessor;
	if (accept(semanticAnalyzerProcessor)) {
		return this->commands;
	}
	else {
		this->commands.clear();
		return this->commands;
	}
}