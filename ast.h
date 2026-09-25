#ifndef AST_H
#define AST_H

#include <string>
#include <vector>

using namespace std;

enum class FieldType
{
    STRING,
    INTEGER,
    EMAIL
};

struct FieldNode
{
    string name;
    FieldType type;
    bool required;

    bool hasMin;
    bool hasMax;

    int minValue;
    int maxValue;
};

struct FormNode
{
    string name;
    vector<FieldNode> fields;
    string submitText;
};

string fieldTypeToString(FieldType type);

#endif