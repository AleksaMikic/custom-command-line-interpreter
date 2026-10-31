#ifndef RM_H
#define RM_H

#include "StreamController.h"
#include "Visitor.h"
#include "Base.h"
#include "Token.h"

class Rm :public Command {
public:
	Rm(std::string& str, CommandBlock& cb,int cbi,int cbs);
	Rm(Rm& other);
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
