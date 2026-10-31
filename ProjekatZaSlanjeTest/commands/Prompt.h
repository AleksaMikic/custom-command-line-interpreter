#ifndef PROMPT_H
#define PROMPT_H
#include "StreamController.h"
#include "Visitor.h"
#include "Base.h"
#include "Token.h"
class Shell;

// Input: the new prompt sign as an argument, and optionally -t
// Output: no text; changes the shell's prompt sign, for one line only with -t
// Responsibility: implements the prompt command
class Prompt :public Command {
public:
	Prompt(std::string& str,CommandBlock& cb,int cbi,int cbs);
	Prompt(Prompt& other);
	virtual void create() override;
	virtual std::unique_ptr<Command> clone() override;

	CommandBlock& getCommandBlock() { return commandBlock; }
	int getCommandBlockIndex() { return commandBlockIndex; }
	int getCommandBlockSize() { return commandBlockSize; }

	void execute(StreamController& sc) override;
private:
	bool accept(Visitor& v)override;

	void changeReadinessSign(Shell& s, std::string& rs);
	void changeTempReadinessSign(Shell& s, std::string& trs);

	CommandBlock commandBlock;
	int commandBlockIndex;
	int commandBlockSize;
	Optional<Option> option;
	Argument temp;
	bool isTempOn = false;
	Argument argument;
};

#endif