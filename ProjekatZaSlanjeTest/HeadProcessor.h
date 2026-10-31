#ifndef HEAD_PROCESSOR_H
#define HEAD_PROCESSOR_H

#include "Token.h"
#include "Visitor.h"
#include "HeadVisitor.h"

class HeadProcessor : public Visitor, public HeadVisitor {
public:
	HeadProcessor(std::vector<unsigned int>& ps, std::vector<std::string>& ms, unsigned int cp);
	HeadProcessor(HeadProcessor& other);
	HeadProcessor(unsigned int cp);
	virtual bool visit(Command& command) override;

	unsigned int getCurrentPosition();
	std::vector<unsigned int>& getPositions() override;
	std::vector<std::string>& getMessages() override;

	std::string& sendWholeMessage() override;

	~HeadProcessor();
private:
	int convert(Token t);
	std::string reconvert(int p);

	bool checkMandatory(CommandBlock& cb);
	bool checkIfOptionIsAllowed(Token& t);
	bool checkOptionSize(Token& t);

	bool checkForDuplicates(CommandBlock& cb);

	bool checkIfAllFilePathsAreFilePaths(CommandBlock& cb);
	bool checkIfIsFilePath(Token& t);

	bool checkAllFilePaths(CommandBlock& cb);
	bool checkIfFileExists(Token& t);

	void pushException(std::string m, unsigned int p);

	std::string wholeMessage;
	unsigned int currentPosition;
};

#endif