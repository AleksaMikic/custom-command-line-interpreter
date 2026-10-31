#ifndef RM_VISITOR_H
#define RM_VISITOR_H

#include "CommandVisitor.h"
#include "Token.h"
#include "Rm.h"

// Input: a rm command to check
// Output: true if valid, and the collected error messages
// Responsibility: interface for the rm command's validator
class RmVisitor : public CommandVisitor {
public:
	RmVisitor();

	virtual bool visit(Command& command) = 0;
	virtual std::string& sendWholeMessage() = 0;

	~RmVisitor();
};

#endif 