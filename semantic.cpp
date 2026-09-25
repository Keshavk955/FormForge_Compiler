#include "semantic.h"

using namespace std;

SemanticAnalyzer::SemanticAnalyzer(SymbolTable& symbolTable)
    : symbolTable(symbolTable)
{
}

bool SemanticAnalyzer::analyze(FormNode& form)
{
    if(form.fields.empty())
    {
        cout << "Semantic Error: Form must contain at least one field." << endl;
        return false;
    }

    for(FieldNode& field : form.fields)
    {
        if(field.name.empty())
        {
            cout << "Semantic Error: Field name cannot be empty." << endl;
            return false;
        }

        if(symbolTable.exists(field.name))
        {
            cout << "Semantic Error: Duplicate field '"
                 << field.name << "'." << endl;
            return false;
        }

        if((field.hasMin || field.hasMax) &&
           field.type != FieldType::INTEGER)
        {
            cout << "Semantic Error: min and max can only be used "
                 << "with integer fields. Field '"
                 << field.name << "' is "
                 << fieldTypeToString(field.type) << "." << endl;
            return false;
        }

        if(field.hasMin && field.hasMax &&
           field.minValue > field.maxValue)
        {
            cout << "Semantic Error: Minimum value cannot be greater "
                 << "than maximum value for field '"
                 << field.name << "'." << endl;
            return false;
        }

        if(!symbolTable.insert(field))
        {
            cout << "Semantic Error: Could not insert field '"
                 << field.name << "' into symbol table." << endl;
            return false;
        }
    }

    if(form.submitText.empty())
    {
        cout << "Semantic Error: Submit text cannot be empty." << endl;
        return false;
    }

    return true;
}