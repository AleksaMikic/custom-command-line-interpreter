#ifndef PARSER_PROCESSOR_H
#define PARSER_PROCESSOR_H

#include "ParserException.h"
#include "ParserVisitor.h"
#include "Visitor.h"
#include "Parser.h"

class ParserProcessor : public Visitor, public ParserVisitor{
public:
	ParserProcessor(std::vector<unsigned int> ps, std::vector<std::string> ms);
	ParserProcessor(ParserProcessor& pp);
	ParserProcessor() = default;
	virtual bool visit(Parser& p) override;

	std::vector<unsigned int>& getPositions() override;
	std::vector<std::string>& getMessages() override;

	~ParserProcessor();
private:

	int convert(Token t);
	std::string reconvert(int p);

	bool checkForDuplicates(std::vector<int> v, unsigned int  i);
	bool checkForSequence(std::vector<int> v, unsigned int i);
	bool checkRedirection(CommandBlock cb, unsigned int p, unsigned int i);
	bool checkCommandBlock(CommandBlock cb, unsigned int p);

	void pushException(std::string m, unsigned int p);
};

#endif
