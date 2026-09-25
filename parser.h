#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"
#include "ast.h"

using namespace std;

class Parser
{
private:
    vector<Token> tokens;
    int current;

    Token peek();
    Token previous();

    bool check(TokenType type);
    bool match(TokenType type);

    Token consume(TokenType type, string message);

    FormNode parseForm();
    FieldNode parseField();
    string parseSubmit();

public:
    Parser(vector<Token> tokens);
    FormNode parse();
};

#endif