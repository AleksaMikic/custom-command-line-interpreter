#ifndef SEMANTIC_ANALYZER_VISITOR_H
#define SEMANTIC_ANALYZER_VISITOR_H

#include <vector>
#include "SemanticAnalyzer.h"

class SemanticAnalyzerVisitor {
public:
	SemanticAnalyzerVisitor() {}

	virtual bool visit(SemanticAnalyzer& sa)=0;

	~SemanticAnalyzerVisitor();
};

#endif