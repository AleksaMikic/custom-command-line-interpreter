#ifndef TR_VISITOR_H
#define TR_VISITOR_H

#include "CommandVisitor.h"
#include "Token.h"
#include "Tr.h"

// Input: a tr command to check
// Output: true if valid, and the collected error messages
// Responsibility: interface for the tr command's validator
class TrVisitor :public CommandVisitor {
public:
	TrVisitor();

	virtual bool visit(Command& command) = 0;
	virtual std::string& sendWholeMessage() = 0;

	~TrVisitor();
};


#endif 
