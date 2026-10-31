#ifndef TOUCH_H
#define TOUCH_H

#include "StreamController.h"
#include "Visitor.h"
#include "Base.h"
#include "Token.h"

// Input: name of a .txt file that does not exist yet
// Output: no text; creates the empty file
// Responsibility: implements the touch command
class Touch :public Command {
public:
	Touch(std::string& str, CommandBlock& cb,int cbi,int cbs);
	Touch(Touch& other);
	virtual void create() override;
	virtual std::unique_ptr<Command> clone() override;

	CommandBlock& getCommandBlock() { return commandBlock; }
	int getCommandBlockIndex() { return commandBlockIndex; }
	int getCommandBlockSize() { return commandBlockSize; }

	void execute(StreamController& sc) override;
private:
	bool accept(Visitor& v)override;

	CommandBlock commandBlock;
	int commandBlockIndex;
	int commandBlockSize;
	FileName fileName;
};

#endif
