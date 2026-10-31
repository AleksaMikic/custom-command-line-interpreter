#ifndef BATCH_VISITOR_H
#define BATCH_VISITOR_H

#include "CommandVisitor.h"
#include "Token.h"
#include "Batch.h"

// Input: a batch command to check
// Output: true if valid, and the collected error messages
// Responsibility: interface for the batch command's validator
class BatchVisitor :public CommandVisitor {
public:
	BatchVisitor();

	virtual bool visit(Command& command) = 0;
	virtual std::string& sendWholeMessage() = 0;

	~BatchVisitor();
};

#endif
