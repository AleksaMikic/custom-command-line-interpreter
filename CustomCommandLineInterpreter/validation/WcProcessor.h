#ifndef WC_PROCESSOR_H
#define WC_PROCESSOR_H

#include <unordered_set>
#include "Token.h"
#include "Visitor.h"
#include "WcVisitor.h"

// Input: a Wc command
// Output: true if valid; otherwise messages describing every rule broken
// Responsibility: checks wc has -w or -c as its second token, at most one argument and valid files
class WcProcessor : public Visitor, public WcVisitor {
public:
	WcProcessor(std::vector<unsigned int>& ps, std::vector<std::string>& ms, unsigned int cp);
	WcProcessor(WcProcessor& other);
	WcProcessor(unsigned int cp);
	virtual bool visit(Command& command) override;

	std::unordered_set<std::string>& getAllowedOptions();
	unsigned int getCurrentPosition();
	std::vector<unsigned int>& getPositions() override;
	std::vector<std::string>& getMessages() override;

	std::string& sendWholeMessage() override;

	~WcProcessor();
private:
	int convert(Token t);
	std::string reconvert(int p);

	bool checkMandatory(CommandBlock& cb);
	bool checkIfOptionIsAllowed(Token& t);

	bool checkForDuplicates(CommandBlock& cb);

	bool checkIfAllFilePathsAreFilePaths(CommandBlock& cb);
	bool checkIfIsFilePath(Token& t);

	bool checkAllFilePaths(CommandBlock& cb);
	bool checkIfFileExists(Token& t);

	void setAllowedOptions();
	void pushException(std::string m, unsigned int p);

	std::unordered_set<std::string> allowedOptions;
	std::string wholeMessage;
	unsigned int currentPosition;
};

#endif
