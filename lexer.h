#ifndef LEXER_H
#define LEXER_H

#include <iostream>
#include <string>
#include <vector>

using namespace std;

enum class TokenType
{
    FORM,
    FIELD,
    STRING_TYPE,
    INTEGER_TYPE,
    EMAIL_TYPE,
    REQUIRED,
    MIN,
    MAX,
    SUBMIT,

    IDENTIFIER,
    NUMBER,
    STRING_LITERAL,

    LBRACE,
    RBRACE,
    SEMICOLON,

    INVALID,
    END_OF_FILE
};

struct Token
{
    TokenType type;
    string lexeme;
    int line;
};

class Lexer
{
private:
    string source;
    int position;
    int line;

    char currentChar();
    void advance();
    bool isAlpha(char c);
    bool isDigit(char c);
    bool isAlphaNumeric(char c);

public:
    Lexer(string source);
    vector<Token> tokenize();
};

string tokenTypeToString(TokenType type);

#endif