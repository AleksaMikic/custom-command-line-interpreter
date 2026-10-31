#ifndef DATE_VISITOR_H
#define DATE_VISITOR_H

#include "CommandVisitor.h"
#include "Token.h"
#include "Date.h"

class DateVisitor : public CommandVisitor {
public:
	DateVisitor();

	virtual bool visit(Command& command) = 0;
	virtual std::string& sendWholeMessage() = 0;

	~DateVisitor();
};

#endif