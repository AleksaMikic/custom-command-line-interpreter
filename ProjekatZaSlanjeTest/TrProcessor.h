#ifndef TR_PROCESSOR_H
#define TR_PROCESSOR_H

#include "Token.h"
#include "Visitor.h"
#include "TrVisitor.h"

class TrProcessor :public Visitor, public TrVisitor {
public:
	TrProcessor(std::vector<unsigned int>& ps, std::vector<std::string>& ms, unsigned int cp);
	TrProcessor(TrProcessor& other);
	TrProcessor(unsigned int cp);

	virtual bool visit(Command& command) override;

	unsigned int getCurrentPosition();
	std::vector<unsigned int>& getPositions() override;
	std::vector<std::string>& getMessages() override;

	std::string& sendWholeMessage() override;

	~TrProcessor();
private:
	int convert(Token t);
	std::string reconvert(int p);

	bool checkMandatory(CommandBlock& cb);
	bool checkForSequence(CommandBlock& cb);

	unsigned int convertSize(CommandBlock& cb);
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