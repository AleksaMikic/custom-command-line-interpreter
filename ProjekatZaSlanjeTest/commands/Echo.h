#ifndef ECHO_H
#define ECHO_H

#include "StreamController.h"
#include "Visitor.h"
#include "Base.h"
#include "Token.h"

// Input: a quoted string, a .txt file, a redirect, the previous command or the keyboard
// Output: the same text unchanged
// Responsibility: implements the echo command
class Echo : public Command {
public:
	Echo(std::string& str,CommandBlock& cb,int cbi,int cbs);
	Echo(Echo& other);
	virtual void create() override;
	virtual std::unique_ptr <Command> clone() override;

	CommandBlock& getCommandBlock() { return commandBlock; }
	int getCommandBlockIndex() { return commandBlockIndex; }
	int getCommandBlockSize() { return commandBlockSize; }

	void execute(StreamController & sc) override;
private:
	bool accept(Visitor& v) override;

	CommandBlock commandBlock;
	int commandBlockIndex;
	int commandBlockSize;
	Optional<Argument> argument;
};

#endif