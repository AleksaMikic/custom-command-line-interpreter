#ifndef HEAD_H
#define HEAD_H

#include "StreamController.h"
#include "Visitor.h"
#include "Base.h"
#include "Token.h"

// Input: -nCOUNT and text from an argument, redirect, previous command or keyboard
// Output: the first COUNT lines of the text
// Responsibility: implements the head command
class Head :public Command {
public:
	Head(std::string& str, CommandBlock& cb,int cbi,int cbs);
	Head(Head& other);
	virtual void create() override;
	virtual std::unique_ptr <Command> clone() override;

	CommandBlock& getCommandBlock() { return commandBlock; }
	int getCommandBlockIndex() { return commandBlockIndex; }
	int getCommandBlockSize() { return commandBlockSize; }

	void execute(StreamController& sc) override;
private:
	int extract();
	std::string transform(int size,std::string& workingStrrin);
	bool accept(Visitor& v) override;

	CommandBlock commandBlock;
	int commandBlockIndex;
	int commandBlockSize;
	Option option;
	Optional<Argument> argument;
};
	

#endif 