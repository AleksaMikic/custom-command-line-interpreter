#ifndef TRUNCATE_H
#define TRUNCATE_H

#include "StreamController.h"
#include "Visitor.h"
#include "Base.h"
#include "Token.h"

// Input: name of an existing .txt file
// Output: no text; removes all content from the file
// Responsibility: implements the truncate command
class Truncate :public Command {
public:
	Truncate(std::string& str, CommandBlock& cb,int cbi,int cbs);
	Truncate(Truncate& other);
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
