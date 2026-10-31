#ifndef DATE_VISITOR_H
#define DATE_VISITOR_H

#include "CommandVisitor.h"
#include "Token.h"
#include "Date.h"

// Input: a date command to check
// Output: true if valid, and the collected error messages
// Responsibility: interface for the date command's validator
class DateVisitor : public CommandVisitor {
public:
	DateVisitor();

	virtual bool visit(Command& command) = 0;
	virtual std::string& sendWholeMessage() = 0;

	~DateVisitor();
};

#endif