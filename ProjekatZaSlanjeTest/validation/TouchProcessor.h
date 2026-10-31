#ifndef TOUCH_PROCESSOR_H
#define TOUCH_PROCESSOR_H

#include "Token.h"
#include "Visitor.h"
#include "TouchVisitor.h"

// Input: a Touch command
// Output: true if valid; otherwise messages describing every rule broken
// Responsibility: checks touch has exactly one .txt file name that does not exist yet
class TouchProcessor :public Visitor, public TouchVisitor {
public:
	TouchProcessor(std::vector<unsigned int>& ps, std::vector<std::string>& ms, unsigned int cp);
	TouchProcessor(TouchProcessor& ep);
	TouchProcessor(unsigned int cp);
	virtual bool visit(Command& command) override;

	unsigned int getCurrentPosition();
	std::vector<unsigned int>& getPositions() override;
	std::vector<std::string>& getMessages() override;

	std::string& sendWholeMessage() override;

	~TouchProcessor();
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
	bool checkIfCreatingFileExists(Token& t);

	void pushException(std::string m, unsigned int p);

	std::string wholeMessage;
	unsigned int currentPosition;
};

#endif