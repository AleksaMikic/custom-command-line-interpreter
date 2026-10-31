#ifndef BATCH_VISITOR_H
#define BATCH_VISITOR_H

#include "CommandVisitor.h"
#include "Token.h"
#include "Batch.h"

class BatchVisitor :public CommandVisitor {
public:
	BatchVisitor();

	virtual bool visit(Command& command) = 0;
	virtual std::string& sendWholeMessage() = 0;

	~BatchVisitor();
};

#endif
