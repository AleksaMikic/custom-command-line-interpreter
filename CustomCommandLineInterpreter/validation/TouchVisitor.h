#ifndef TOUCH_VISITOR_H
#define TOUCH_VISITOR_H

#include "CommandVisitor.h"
#include "Token.h"
#include "Touch.h"

// Input: a touch command to check
// Output: true if valid, and the collected error messages
// Responsibility: interface for the touch command's validator
class TouchVisitor:public CommandVisitor {
public:
	TouchVisitor();

	virtual bool visit(Command& command) = 0;
	virtual std::string& sendWholeMessage() = 0;

	~TouchVisitor();
};

#endif