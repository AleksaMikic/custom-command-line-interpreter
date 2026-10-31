#ifndef WC_VISITOR_H
#define WC_VISITOR_H

#include "CommandVisitor.h"
#include "Token.h"
#include "Wc.h"

// Input: a wc command to check
// Output: true if valid, and the collected error messages
// Responsibility: interface for the wc command's validator
class WcVisitor :public CommandVisitor {
public:
	WcVisitor();

	virtual bool visit(Command& command) = 0;
	virtual std::string& sendWholeMessage() = 0;

	~WcVisitor();
};

#endif