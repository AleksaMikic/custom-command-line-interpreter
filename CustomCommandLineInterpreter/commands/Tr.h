#ifndef TR_H
#define TR_H

#include "StreamController.h"
#include "Visitor.h"
#include "Base.h"
#include "Token.h"

// Input: text from an argument, redirect, previous command or keyboard, the "what" string and an optional "with" string
// Output: the text with every "what" replaced by "with", or removed if "with" is missing
// Responsibility: implements the tr command
class Tr : public Command {
public:
	Tr(std::string& str, CommandBlock& cb,int cbi,int cbs);
	Tr(Tr& other);
	virtual void create() override;
	virtual std::unique_ptr <Command> clone() override;

	CommandBlock& getCommandBlock() { return commandBlock; }
	int getCommandBlockIndex() { return commandBlockIndex; }
	int getCommandBlockSize() { return commandBlockSize; }

	void execute(StreamController& sc) override;
private:
	bool accept(Visitor& v) override;

	void createWithThreeQuotedStrings();
	void createWithoutThreeQuotedStrings();

	int countQuotedStrings();

	std::string changeOnlyWhat(std::string& str, std::string& what);
	std::string changeWhatWith(std::string& str, std::string& what, std::string& with);

	CommandBlock commandBlock;
	int commandBlockIndex;
	int commandBlockSize;
	Optional<Argument> argument;
	QuotedString what;
	Optional<QuotedString> with;
};

#endif