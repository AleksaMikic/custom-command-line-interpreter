#ifndef LEXER_PROCESSOR_H
#define LEXER_PROCESSOR_H

#include "Lexer.h"
#include "LexerException.h"
#include "LexerVisitor.h"
#include "Visitor.h"


// Input: the Lexer holding the raw line
// Output: true if valid; otherwise reports the positions of forbidden characters or an unclosed quote
// Responsibility: cuts the line to 512 characters and checks that it only contains allowed characters
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
