#ifndef SEMANTIC_H
#define SEMANTIC_H

#include <iostream>
#include "ast.h"
#include "symbol_table.h"

using namespace std;

class SemanticAnalyzer
{
private:
    SymbolTable& symbolTable;

public:
    SemanticAnalyzer(SymbolTable& symbolTable);

    bool analyze(FormNode& form);
};

#endif