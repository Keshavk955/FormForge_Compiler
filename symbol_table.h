#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include <iostream>
#include <map>
#include <string>
#include "ast.h"

using namespace std;

struct Symbol
{
    string name;
    FieldType type;
    bool required;
    bool hasMin;
    bool hasMax;
    int minValue;
    int maxValue;
};

class SymbolTable
{
private:
    map<string, Symbol> table;

public:
    bool insert(FieldNode field);
    bool exists(string name);
    Symbol* lookup(string name);
    void display();
};

#endif