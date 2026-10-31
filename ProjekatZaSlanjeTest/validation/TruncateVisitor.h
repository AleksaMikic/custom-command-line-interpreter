#ifndef TRUNCATE_VISITOR_H
#define TRUNCATE_VISITOR_H
#include "CommandVisitor.h"
#include "Token.h"
#include "Truncate.h"

// Input: a truncate command to check
// Output: true if valid, and the collected error messages
// Responsibility: interface for the truncate command's validator
class TruncateVisitor :public CommandVisitor {
public:
	TruncateVisitor();

	virtual bool visit(Command& command) = 0;
	virtual std::string& sendWholeMessage() = 0;

	~TruncateVisitor();
};

#endif
