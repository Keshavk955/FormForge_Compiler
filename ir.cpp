#include "ir.h"

using namespace std;

FormIR IRGenerator::generate(FormNode& form)
{
    FormIR ir;

    ir.formName = form.name;
    ir.submitText = form.submitText;

    for(FieldNode field : form.fields)
    {
        IRField irField;

        irField.name = field.name;
        irField.type = fieldTypeToString(field.type);
        irField.required = field.required;
        irField.hasMin = field.hasMin;
        irField.hasMax = field.hasMax;
        irField.minValue = field.minValue;
        irField.maxValue = field.maxValue;

        ir.fields.push_back(irField);
    }

    return ir;
}

void IRGenerator::display(FormIR& ir)
{
    cout << "CREATE_FORM " << ir.formName << endl;

    for(IRField field : ir.fields)
    {
        cout << "ADD_FIELD "
             << field.name << " "
             << field.type;

        if(field.required)
        {
            cout << " REQUIRED";
        }

        if(field.hasMin)
        {
            cout << " MIN " << field.minValue;
        }

        if(field.hasMax)
        {
            cout << " MAX " << field.maxValue;
        }

        cout << endl;
    }

    cout << "ADD_SUBMIT \"" 
         << ir.submitText 
         << "\"" << endl;
}