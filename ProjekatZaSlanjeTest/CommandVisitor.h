#ifndef COMMAND_VISITOR_H
#define COMMAND_VISITOR_H

#include"Base.h"
#include "Visitor.h"

class CommandVisitor {
public:
	CommandVisitor();

	virtual bool visit(Command& command) = 0;
	virtual std::string& sendWholeMessage() = 0;

	virtual ~CommandVisitor();
};

#endif