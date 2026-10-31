#ifndef TIME_H
#define TIME_H

#include "StreamController.h"
#include "Visitor.h"
#include "Base.h"
#include "Token.h"


// Input: none
// Output: current local time as HH:MM:SS
// Responsibility: implements the time command
class Time :public Command {
public:
	Time(std::string& str, CommandBlock& cb,int cbi,int cbs);
	Time(Time& other);
	virtual void create() override;
	virtual std::unique_ptr <Command> clone() override;

	CommandBlock& getCommandBlock() { return commandBlock; }
	int getCommandBlockIndex() { return commandBlockIndex; }
	int getCommandBlockSize() { return commandBlockSize; }

	void execute(StreamController& sc) override;
private:
	bool accept(Visitor& v) override;

	std::string getCurrentSystemTime(bool useLocalTime);

	CommandBlock commandBlock;
	int commandBlockIndex;
	int commandBlockSize;
};
#endif