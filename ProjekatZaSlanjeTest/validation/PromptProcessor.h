#ifndef PROMPT_PROCESSOR_H
#define PROMPT_PROCESSOR_H

#include "Token.h"
#include "Visitor.h"
#include "PromptVisitor.h"

// Input: a Prompt command
// Output: true if valid; otherwise messages describing every rule broken
// Responsibility: checks prompt has an argument, only the -t option, only .txt files and that input files exist
class PromptProcessor :public Visitor, public PromptVisitor {
public:
	PromptProcessor(std::vector<unsigned int>& ps, std::vector<std::string>& ms, unsigned int cp);
	PromptProcessor(PromptProcessor& ep);
	PromptProcessor(unsigned int cp);
	virtual bool visit(Command& command) override;

	unsigned int getCurrentPosition();
	std::vector<unsigned int>& getPositions() override;
	std::vector<std::string>& getMessages() override;

	std::string& sendWholeMessage() override;

	~PromptProcessor();
private:
	int convert(Token& t);
	std::string reconvert(int p);
	
	bool checkMandatory(CommandBlock& cb);

	bool checkForDuplicates(CommandBlock& cb);

	bool checkAllowedTokens(CommandBlock& cb);
	bool checkAllowedToken(Token& t);

	bool checkIfAllFilePathsAreFilePaths(CommandBlock& cb);
	bool checkIfIsFilePath(Token& t);

	bool checkAllFilePaths(CommandBlock& cb);
	bool checkIfFileExists(Token& t);

	void pushException(std::string m, unsigned int p);

	std::string wholeMessage;
	unsigned int currentPosition;
};

#endif