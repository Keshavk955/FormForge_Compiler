#include <iostream>
#include <fstream>
#include <sstream>

#include "lexer.h"
#include "parser.h"
#include "symbol_table.h"
#include "semantic.h"
#include "ir.h"
#include "generator.h"

using namespace std;

int main(int argc, char* argv[])
{
    string fileName = "program.form";

    if(argc > 1)
    {
        fileName = argv[1];
    }

    ifstream file(fileName);

    if(!file)
    {
        cout << "Error: Could not open " << fileName << endl;
        return 1;
    }

    stringstream buffer;
    buffer << file.rdbuf();

    string source = buffer.str();

    cout << "=== FORMFORGE COMPILER ===" << endl;
    cout << "Source file: " << fileName << endl;
    cout << endl;

    Lexer lexer(source);
    vector<Token> tokens = lexer.tokenize();

    cout << "STEP 1: LEXICAL ANALYSIS" << endl;
    cout << "Tokens generated: " << tokens.size() - 1 << endl;

    for(const Token& token : tokens)
    {
        if(token.type == TokenType::INVALID)
        {
            cout << "Lexical Error at line "
                 << token.line << ": ";

            if(!token.errorMessage.empty())
            {
                cout << token.errorMessage << endl;
            }
            else
            {
                cout << "Invalid token '"
                     << token.lexeme << "'." << endl;
            }

            return 1;
        }
    }

    cout << "Lexical analysis: PASSED" << endl;
    cout << endl;

    try
    {
        Parser parser(tokens);
        FormNode form = parser.parse();

        cout << "STEP 2: SYNTAX ANALYSIS" << endl;
        cout << "Syntax: VALID" << endl;
        cout << endl;

        cout << "STEP 3: ABSTRACT SYNTAX TREE" << endl;
        cout << "FORM: " << form.name << endl;

        for(const FieldNode& field : form.fields)
        {
            cout << "|-- FIELD: " << field.name << endl;

            cout << "|   |-- TYPE: "
                 << fieldTypeToString(field.type) << endl;

            if(field.required)
            {
                cout << "|   |-- REQUIRED" << endl;
            }

            if(field.hasMin)
            {
                cout << "|   |-- MIN: "
                     << field.minValue << endl;
            }

            if(field.hasMax)
            {
                cout << "|   |-- MAX: "
                     << field.maxValue << endl;
            }
        }

        cout << "|-- SUBMIT: "
             << form.submitText << endl;

        cout << endl;

        SymbolTable symbolTable;

        SemanticAnalyzer semanticAnalyzer(symbolTable);

        cout << "STEP 4: SEMANTIC ANALYSIS" << endl;

        if(!semanticAnalyzer.analyze(form))
        {
            return 1;
        }

        cout << "Semantic checks: PASSED" << endl;
        cout << endl;

        cout << "STEP 5: SYMBOL TABLE" << endl;
        symbolTable.display();

        cout << endl;

        IRGenerator irGenerator;
        FormIR ir = irGenerator.generate(form);

        cout << "STEP 6: INTERMEDIATE REPRESENTATION" << endl;
        irGenerator.display(ir);

        cout << endl;

        CodeGenerator generator;

        cout << "STEP 7: CODE GENERATION" << endl;

        if(generator.generate(ir))
        {
            cout << "HTML generated successfully." << endl;
            cout << "Output file: "
                 << ir.formName << ".html" << endl;
        }
        else
        {
            cout << "Code Generation Error: Could not create HTML file."
                 << endl;

            return 1;
        }

        cout << endl;
        cout << "=== COMPILATION SUCCESSFUL ===" << endl;
    }
    catch(const exception& e)
    {
        cout << e.what() << endl;
        return 1;
    }

    return 0;
}