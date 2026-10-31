#ifndef WC_VISITOR_H
#define WC_VISITOR_H

#include "CommandVisitor.h"
#include "Token.h"
#include "Wc.h"

class WcVisitor :public CommandVisitor {
public:
	WcVisitor();

	virtual bool visit(Command& command) = 0;
	virtual std::string& sendWholeMessage() = 0;

	~WcVisitor();
};

#endif