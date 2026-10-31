#include <iostream>
#include "Lexer.h"
#include "Parser.h"
#include "SemanticAnalyzer.h"
#include "Executor.h"
#include "Shell.h"
int main()
{
    Shell::getInstance().run();
    return 0;
}