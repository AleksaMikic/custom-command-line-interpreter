#ifndef BATCH_PROCESSOR_H
#define BATCH_PROCESSOR_H

#include "Token.h"
#include "Visitor.h"
#include "BatchVisitor.h"

// Input: a Batch command
// Output: true if valid; otherwise messages describing every rule broken
// Responsibility: checks batch has no options, at most one argument and valid files
class BatchProcessor :public Visitor, public BatchVisitor {
public:
	BatchProcessor(std::vector<unsigned int>& ps, std::vector<std::string>& ms, unsigned int cp);
	BatchProcessor(BatchProcessor& ep);
	BatchProcessor(unsigned int cp);
	virtual bool visit(Command& command) override;

	unsigned int getCurrentPosition();
	std::vector<unsigned int>& getPositions() override;
	std::vector<std::string>& getMessages() override;

	std::string& sendWholeMessage() override;

	~BatchProcessor();
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
