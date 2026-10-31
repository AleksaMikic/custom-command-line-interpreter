#ifndef RM_VISITOR_H
#define RM_VISITOR_H

#include "CommandVisitor.h"
#include "Token.h"
#include "Rm.h"

class RmVisitor : public CommandVisitor {
public:
	RmVisitor();

	virtual bool visit(Command& command) = 0;
	virtual std::string& sendWholeMessage() = 0;

	~RmVisitor();
};

#endif 