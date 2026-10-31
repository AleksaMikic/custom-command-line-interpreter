#ifndef SHELL_H
#define SHELL_H

#include <stack>
#include "Lexer.h"
#include "Parser.h"
#include "SemanticAnalyzer.h"
#include "Executor.h"

// Input: lines typed by the user, or a list of lines from the batch command
// Output: prompt sign and command results on the console
// Responsibility: runs the read-lex-parse-analyze-execute loop until exit or end of input; instances are kept on a stack so batch can run a nested shell
class Shell {
public:
	Shell();
	Shell(std::string& rs);
	Shell(const Shell& other) = delete;
	void run();
	void run(std::vector<std::string>& inputs);

	std::string& getReadinessSign() { return readinessSign; }
	void setReadinessSign(std::string& rs) { readinessSign = rs; }
	void setTempReadinessSign(std::string& trs) { tempReadinessSign = trs; isTempOn = true; }
	~Shell() = default;
	static Shell& getInstance();
	static std::stack<std::unique_ptr<Shell>>& getInstances();
private:
	static std::stack<std::unique_ptr<Shell>> instances;

	Lexer lexer;
	Parser parser;
	SemanticAnalyzer semanticAnalyzer;
	Executor executor;
	std::string tempReadinessSign;
	std::string readinessSign;
	bool isTempOn = false;
};

#endif