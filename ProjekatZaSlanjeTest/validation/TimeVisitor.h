#ifndef TIME_VISITOR_H
#define TIME_VISITOR_H

#include "CommandVisitor.h"
#include "Token.h"
#include "Time.h"

// Input: a time command to check
// Output: true if valid, and the collected error messages
// Responsibility: interface for the time command's validator
class TimeVisitor :public CommandVisitor {
public:
	TimeVisitor();

	virtual bool visit(Command& command) = 0;
	virtual std::string& sendWholeMessage() = 0;

	~TimeVisitor();
};

#endif
