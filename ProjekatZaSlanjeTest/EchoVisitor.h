#ifndef ECHO_VISITOR_H
#define ECHO_VISITOR_H

#include "CommandVisitor.h"
#include "Token.h"
#include "Echo.h"

class EchoVisitor:public CommandVisitor {
public:
	EchoVisitor();

	virtual bool visit(Command& command) = 0;
	virtual std::string& sendWholeMessage() = 0;

	~EchoVisitor();
};

#endif