#include "symbol_table.h"

using namespace std;

bool SymbolTable::insert(FieldNode field)
{
    if(exists(field.name))
    {
        return false;
    }

    Symbol symbol;

    symbol.name = field.name;
    symbol.type = field.type;
    symbol.required = field.required;
    symbol.hasMin = field.hasMin;
    symbol.hasMax = field.hasMax;
    symbol.minValue = field.minValue;
    symbol.maxValue = field.maxValue;

    table[field.name] = symbol;

    return true;
}

bool SymbolTable::exists(string name)
{
    return table.find(name) != table.end();
}

Symbol* SymbolTable::lookup(string name)
{
    auto it = table.find(name);

    if(it == table.end())
    {
        return nullptr;
    }

    return &it->second;
}

void SymbolTable::display()
{
    cout << "Name\tType\tRequired\tMin\tMax" << endl;
    cout << "---------------------------------------------" << endl;

    for(auto entry : table)
    {
        Symbol symbol = entry.second;

        cout << symbol.name << "\t"
             << fieldTypeToString(symbol.type) << "\t"
             << (symbol.required ? "yes" : "no") << "\t\t";

        if(symbol.hasMin)
        {
            cout << symbol.minValue;
        }
        else
        {
            cout << "-";
        }

        cout << "\t";

        if(symbol.hasMax)
        {
            cout << symbol.maxValue;
        }
        else
        {
            cout << "-";
        }

        cout << endl;
    }
}