#ifndef VISITABLE_VISITOR_H
#define VISITABLE_VISITOR_H

#include "Visitor.h"

// Input: a validator
// Output: true if the validator found no errors
// Responsibility: base class of everything that can be validated (lexer, parser, analyzer, commands)
class VisitableElement {
public:
	virtual ~VisitableElement();
	virtual bool accept(Visitor& v) = 0;
};

#endif