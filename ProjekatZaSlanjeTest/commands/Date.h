#ifndef DATE_H
#define DATE_H

#include "StreamController.h"
#include "Visitor.h"
#include "Base.h"
#include "Token.h"

// Input: none
// Output: current local date as YYYY-MM-DD
// Responsibility: implements the date command
class Date : public Command {
public:
	Date(std::string& str, CommandBlock& cb,int cbi,int cbs);
	Date(Date& other);
	virtual void create() override;
	virtual std::unique_ptr <Command> clone() override;

	CommandBlock& getCommandBlock() { return commandBlock; }
	int getCommandBlockIndex() { return commandBlockIndex; }
	int getCommandBlockSize() { return commandBlockSize; }

	void execute(StreamController& sc) override;
private:
	bool accept(Visitor& v) override;

	std::string getCurrentSystemDate(bool useLocalTime);

	CommandBlock commandBlock;
	int commandBlockIndex;
	int commandBlockSize;
};

#endif 