#include "SemanticAnalyzerProcessor.h"
#include "CommandFactory.h"
#include "CommandProcessorFactory.h"
#include "VisitorException.h"

std::unordered_set<std::string>& SemanticAnalyzerProcessor::getAllowedCommands() {
	return allowedCommands;
}

std::vector<unsigned int>& SemanticAnalyzerProcessor::getPositions() {
	return Visitor::getPositions();
}

std::vector<std::string>& SemanticAnalyzerProcessor::getMessages() {
	return Visitor::getMessages();
}

void SemanticAnalyzerProcessor::setAllowedCommands() {
	allowedCommands.insert("echo");
	allowedCommands.insert("prompt");
	allowedCommands.insert("time");
	allowedCommands.insert("date");
	allowedCommands.insert("touch");
	allowedCommands.insert("truncate");
	allowedCommands.insert("rm");
	allowedCommands.insert("wc");
	allowedCommands.insert("tr");
	allowedCommands.insert("head");
	allowedCommands.insert("batch");
}

SemanticAnalyzerProcessor::SemanticAnalyzerProcessor(std::vector<unsigned int> ps, std::vector<std::string> ms):Visitor(ps,ms) {
	setAllowedCommands();
}

SemanticAnalyzerProcessor::SemanticAnalyzerProcessor(SemanticAnalyzerProcessor& sap) :Visitor(sap.getPositions(), sap.getMessages()) {
	setAllowedCommands();
}

SemanticAnalyzerProcessor::SemanticAnalyzerProcessor(){
	setAllowedCommands();
}

SemanticAnalyzerProcessor::~SemanticAnalyzerProcessor() {

}

void SemanticAnalyzerProcessor::pushException(std::string m, unsigned int p) {
	getMessages().push_back(m);
	getPositions().push_back(p);
}

bool SemanticAnalyzerProcessor::checkAndCall(Command& c, CommandVisitor& cv) {
	try {
		if (auto* v = dynamic_cast<Visitor*>(&cv)) {
			return c.accept(*v);
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

bool SemanticAnalyzerProcessor::setCommand(SemanticAnalyzer& sa, unsigned int& commandsIndex,unsigned int commandBlocksIndex,std::string& commandName) {
	bool error_part = true;
	CommandBlock& commandBlock = sa.getCommandBlocks()[commandBlocksIndex];
	auto& commands = sa.getCommands();
	commands.push_back(CommandFactory::createCommand(commandName,commandBlock,commandBlocksIndex,static_cast<int>(sa.getCommandBlocks().size())));
	std::unique_ptr<CommandVisitor> visitor = CommandProcessorFactory::createCommandProcessor(commandName, commandBlocksIndex);
	Command& command = *(commands[commandsIndex]);
	error_part &= checkAndCall(command, *visitor);
	if (!error_part) {
		pushException("Exception/s detected in command block " + std::to_string(commandBlocksIndex+1) + ":\n" 
			+ sa.getCommandBlocks()[commandBlocksIndex].convertCommandBlockToString() + visitor->sendWholeMessage(), commandBlocksIndex);
	}
	else {
		command.create();
	}
	commandsIndex++;
	return error_part;
}

bool SemanticAnalyzerProcessor::checkCommandBlock(SemanticAnalyzer& sa, unsigned int& commandsIndex, unsigned int commandBlocksIndex) {
	std::string commandName = sa.getCommandBlocks()[commandBlocksIndex].getTokens()[0].getValue();
	bool err = true;
	if (getAllowedCommands().count(commandName) == 1) {
		err &= setCommand(sa, commandsIndex, commandBlocksIndex, commandName);
	}
	else {
		err &= false;
		pushException("Exception detected in " + std::to_string(commandBlocksIndex+1) + ". command block, command of type " + commandName + " doesn't exist", commandBlocksIndex);
	}
	return err;
}

bool SemanticAnalyzerProcessor::visit(SemanticAnalyzer& sa) {
	bool  err = true;
	unsigned int j = 0;
	try {
		for (unsigned int i = 0; i < static_cast<unsigned int>(sa.getCommandBlocks().size()); i++) {
			err &= checkCommandBlock(sa, j, i);
		}
		if (!err) {
			throw SemanticAnalyzerException("Semantic errors detected in command line:\n", sa.getCommandBlocks(), getPositions(), getMessages());
		}
		else {
			return err;
		}
	}
	catch (SemanticAnalyzerException& e) {
		std::cerr << e.what() << "\n";
	}
	return err;
}