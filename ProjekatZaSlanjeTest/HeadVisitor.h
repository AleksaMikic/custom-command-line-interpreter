#ifndef HEAD_VISITOR_H
#define HEAD_VISITOR_H

#include "CommandVisitor.h"
#include "Token.h"
#include "Head.h"

class HeadVisitor :public CommandVisitor {
public:
	HeadVisitor();

	virtual bool visit(Command& command) = 0;
	virtual std::string& sendWholeMessage() = 0;

	~HeadVisitor();
};


#endif 