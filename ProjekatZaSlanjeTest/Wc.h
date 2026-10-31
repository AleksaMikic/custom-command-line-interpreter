#ifndef WC_H
#define WC_H

#include <cctype>
#include "StreamController.h"
#include "Visitor.h"
#include "Base.h"
#include "Token.h"

class Wc : public Command {
public:
	Wc(std::string& str, CommandBlock& cb,int cbi,int cbs);
	Wc(Wc& other);
	virtual void create() override;
	virtual std::unique_ptr <Command> clone() override;

	CommandBlock& getCommandBlock() { return commandBlock; }
	int getCommandBlockIndex() { return commandBlockIndex; }
	int getCommandBlockSize() { return commandBlockSize; }

	void execute(StreamController& sc) override;
private:
	std::string executeOptionW(std::string& str);
	std::string executeOptionC(std::string& str);
	std::string executeOptions(std::string& str);
	bool accept(Visitor& v) override;

	CommandBlock commandBlock;
	int commandBlockIndex;	
	int commandBlockSize;
	Option option;
	Optional<Argument> argument;
};

#endif
