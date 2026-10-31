#ifndef COMMAND_H
#define COMMAND_H

#include <memory>
#include "VisitableElement.h"
#include "StreamController.h"
#include "Redirect.h"
#include "Optional.h"


class Command :public VisitableElement{
public:
	Command(std::string& cn) :commandName(cn),input(),output() {}

	bool checkInput() { return input.checkHasValue(); }
	bool checkOutput() { return output.checkHasValue(); }

	std::string& getCommandName() { return commandName; }
	Optional<Redirect>& getInput() { return input; }
	Optional<Redirect>& getOutput() { return output; }
	
	void setInput(Redirect& r) { input.setValue(r); }
	void setOutput(Redirect& r) { output.setValue(r); }

	virtual void create()=0;

	void changeStreams(StreamController& sc);
	virtual void execute(StreamController& sc)=0;
	virtual std::unique_ptr <Command> clone() = 0;
	virtual bool accept(Visitor& v) = 0;

	virtual ~Command() { input.~Optional(); output.~Optional(); }
private:
	std::string commandName;
	Optional<Redirect> input;
	Optional<Redirect> output;
};

#endif