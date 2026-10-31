#ifndef COMMAND_VISITOR_H
#define COMMAND_VISITOR_H

#include"Base.h"
#include "Visitor.h"

// Input: a command to check
// Output: true if valid, and all error messages joined into one string
// Responsibility: interface for all per-command validators
class CommandVisitor {
public:
	CommandVisitor();

	virtual bool visit(Command& command) = 0;
	virtual std::string& sendWholeMessage() = 0;

	virtual ~CommandVisitor();
};

#endif