#ifndef IR_H
#define IR_H

#include <iostream>
#include <vector>
#include <string>
#include "ast.h"

using namespace std;

struct IRField
{
    string name;
    string type;
    bool required;
    bool hasMin;
    bool hasMax;
    int minValue;
    int maxValue;
};

struct FormIR
{
    string formName;
    vector<IRField> fields;
    string submitText;
};

class IRGenerator
{
public:
    FormIR generate(FormNode& form);
    void display(FormIR& ir);
};

#endif