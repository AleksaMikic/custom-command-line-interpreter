#ifndef HEAD_VISITOR_H
#define HEAD_VISITOR_H

#include "CommandVisitor.h"
#include "Token.h"
#include "Head.h"

// Input: a head command to check
// Output: true if valid, and the collected error messages
// Responsibility: interface for the head command's validator
class HeadVisitor :public CommandVisitor {
public:
	HeadVisitor();

	virtual bool visit(Command& command) = 0;
	virtual std::string& sendWholeMessage() = 0;

	~HeadVisitor();
};


#endif 