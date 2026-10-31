#ifndef TRUNCATE_VISITOR_H
#define TRUNCATE_VISITOR_H
#include "CommandVisitor.h"
#include "Token.h"
#include "Truncate.h"

class TruncateVisitor :public CommandVisitor {
public:
	TruncateVisitor();

	virtual bool visit(Command& command) = 0;
	virtual std::string& sendWholeMessage() = 0;

	~TruncateVisitor();
};

#endif
