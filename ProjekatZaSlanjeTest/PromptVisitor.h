#ifndef PROMPT_VISITOR_H
#define PROMPT_VISITOR_H
#include "CommandVisitor.h"
#include "Token.h"
#include "Prompt.h"
class PromptVisitor :public CommandVisitor {
public:
	PromptVisitor();

	virtual bool visit(Command& command) = 0;
	virtual std::string& sendWholeMessage() = 0;

	~PromptVisitor();
};

#endif