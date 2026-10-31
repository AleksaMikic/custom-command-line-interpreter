#ifndef TIME_VISITOR_H
#define TIME_VISITOR_H

#include "CommandVisitor.h"
#include "Token.h"
#include "Time.h"

class TimeVisitor :public CommandVisitor {
public:
	TimeVisitor();

	virtual bool visit(Command& command) = 0;
	virtual std::string& sendWholeMessage() = 0;

	~TimeVisitor();
};

#endif
