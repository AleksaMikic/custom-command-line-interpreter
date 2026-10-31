#ifndef PROMPT_VISITOR_H
#define PROMPT_VISITOR_H
#include "CommandVisitor.h"
#include "Token.h"
#include "Prompt.h"
// Input: a prompt command to check
// Output: true if valid, and the collected error messages
// Responsibility: interface for the prompt command's validator
class PromptVisitor :public CommandVisitor {
public:
	PromptVisitor();

	virtual bool visit(Command& command) = 0;
	virtual std::string& sendWholeMessage() = 0;

	~PromptVisitor();
};

#endif