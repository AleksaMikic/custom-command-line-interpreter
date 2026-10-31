#ifndef ECHO_PROCESSOR_H
#define ECHO_PROCESSOR_H

#include "Token.h"
#include "Visitor.h"
#include "EchoVisitor.h"

class EchoProcessor :public Visitor, public EchoVisitor {
public:
	EchoProcessor(std::vector<unsigned int>& ps, std::vector<std::string>& ms,unsigned int cp);
	EchoProcessor(EchoProcessor& ep);
	EchoProcessor(unsigned int cp);
	virtual bool visit(Command& command) override;

	unsigned int getCurrentPosition();
	std::vector<unsigned int>& getPositions() override;
	std::vector<std::string>& getMessages() override;

	std::string& sendWholeMessage() override;

	~EchoProcessor();
private:
	int convert(Token t);
	std::string reconvert(int p);

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