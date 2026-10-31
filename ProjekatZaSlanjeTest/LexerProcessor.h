#ifndef LEXER_PROCESSOR_H
#define LEXER_PROCESSOR_H

#include "Lexer.h"
#include "LexerException.h"
#include "LexerVisitor.h"
#include "Visitor.h"


class LexerProcessor: public Visitor,public LexerVisitor{
public:
	LexerProcessor(std::vector<unsigned int> ps, std::vector<std::string> ms);
	LexerProcessor(LexerProcessor& lp);
	LexerProcessor() = default;
	virtual bool visit(Lexer& l)override;

	std::vector<unsigned int>& getPositions() override;
	std::vector<std::string>& getMessages() override;

	~LexerProcessor();
private:

	void resizeInput(Lexer& l);
	bool processLine(Lexer& l);
};

#endif
