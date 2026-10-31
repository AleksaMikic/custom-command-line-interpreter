#ifndef ECHO_VISITOR_H
#define ECHO_VISITOR_H

#include "CommandVisitor.h"
#include "Token.h"
#include "Echo.h"

// Input: an echo command to check
// Output: true if valid, and the collected error messages
// Responsibility: interface for the echo command's validator
class EchoVisitor:public CommandVisitor {
public:
	EchoVisitor();

	virtual bool visit(Command& command) = 0;
	virtual std::string& sendWholeMessage() = 0;

	~EchoVisitor();
};

#endif