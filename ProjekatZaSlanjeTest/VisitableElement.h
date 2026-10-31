#ifndef VISITABLE_VISITOR_H
#define VISITABLE_VISITOR_H

#include "Visitor.h"

class VisitableElement {
public:
	virtual ~VisitableElement();
	virtual bool accept(Visitor& v) = 0;
};

#endif