#ifndef BATCH_H
#define BATCH_H

#include <stack>
#include <memory>
#include "StreamController.h"
#include "Visitor.h"
#include "Base.h"
#include "Token.h"

class Shell;

// Input: text with one command line per row, from a .txt file, string, redirect or keyboard
// Output: the results of every command line
// Responsibility: implements the batch command by running each row in a new nested shell
class Batch : public Command {
public:
	Batch(std::string& str, CommandBlock& cb,int cbi,int cbs);
	Batch(Batch& other);
	virtual void create() override;
	virtual std::unique_ptr <Command> clone() override;

	CommandBlock& getCommandBlock() { return commandBlock; }
	int getCommandBlockIndex() { return commandBlockIndex; }
	int getCommandBlockSize() { return commandBlockSize; }

	void execute(StreamController& sc) override;
private:
	std::vector <std::string> breakUp(std::string& str);
	void executeSubShell(std::vector<std::string> strings,std::stack<std::unique_ptr<Shell>>& instances);
	bool accept(Visitor& v) override;

	CommandBlock commandBlock;
	int commandBlockIndex;
	int commandBlockSize;
	Optional<Argument> argument;
};


#endif