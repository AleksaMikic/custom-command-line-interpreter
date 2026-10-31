#ifndef SEMANTIC_ANALYZER_VISITOR_H
#define SEMANTIC_ANALYZER_VISITOR_H

#include <vector>
#include "SemanticAnalyzer.h"

// Input: a SemanticAnalyzer to check
// Output: true if every command in the line is valid
// Responsibility: interface for semantic validators
class SemanticAnalyzerVisitor {
public:
	SemanticAnalyzerVisitor() {}

	virtual bool visit(SemanticAnalyzer& sa)=0;

	~SemanticAnalyzerVisitor();
};

#endif