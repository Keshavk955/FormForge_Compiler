#include "lexer.h"
#include <cctype>

using namespace std;

Lexer::Lexer(string source)
{
    this->source = source;
    position = 0;
    line = 1;
}

char Lexer::currentChar()
{
    if(position >= source.length())
    {
        return '\0';
    }

    return source[position];
}

void Lexer::advance()
{
    if(position < source.length())
    {
        if(source[position] == '\n')
        {
            line++;
        }

        position++;
    }
}

bool Lexer::isAlpha(char c)
{
    return isalpha(static_cast<unsigned char>(c)) || c == '_';
}

bool Lexer::isDigit(char c)
{
    return isdigit(static_cast<unsigned char>(c));
}

bool Lexer::isAlphaNumeric(char c)
{
    return isAlpha(c) || isDigit(c);
}

vector<Token> Lexer::tokenize()
{
    vector<Token> tokens;

    while(currentChar() != '\0')
    {
        char c = currentChar();

        if(isspace(static_cast<unsigned char>(c)))
        {
            advance();
            continue;
        }

        int startLine = line;

        if(isAlpha(c))
        {
            string word;

            while(isAlphaNumeric(currentChar()))
            {
                word += currentChar();
                advance();
            }

            if(word == "form")
            {
                tokens.push_back({TokenType::FORM, word, startLine});
            }
            else if(word == "field")
            {
                tokens.push_back({TokenType::FIELD, word, startLine});
            }
            else if(word == "string")
            {
                tokens.push_back({TokenType::STRING_TYPE, word, startLine});
            }
            else if(word == "integer")
            {
                tokens.push_back({TokenType::INTEGER_TYPE, word, startLine});
            }
            else if(word == "email")
            {
                tokens.push_back({TokenType::EMAIL_TYPE, word, startLine});
            }
            else if(word == "required")
            {
                tokens.push_back({TokenType::REQUIRED, word, startLine});
            }
            else if(word == "min")
            {
                tokens.push_back({TokenType::MIN, word, startLine});
            }
            else if(word == "max")
            {
                tokens.push_back({TokenType::MAX, word, startLine});
            }
            else if(word == "submit")
            {
                tokens.push_back({TokenType::SUBMIT, word, startLine});
            }
            else
            {
                tokens.push_back({TokenType::IDENTIFIER, word, startLine});
            }

            continue;
        }

        if(isDigit(c))
        {
            string number;

            while(isDigit(currentChar()))
            {
                number += currentChar();
                advance();
            }

            tokens.push_back({TokenType::NUMBER, number, startLine});
            continue;
        }

        if(c == '"')
        {
            advance();

            string text;

            while(currentChar() != '"' && currentChar() != '\0')
            {
                if(currentChar() == '\n')
                {
                    break;
                }

                text += currentChar();
                advance();
            }

            if(currentChar() == '"')
            {
                advance();
                tokens.push_back({TokenType::STRING_LITERAL, text, startLine});
            }
            else
            {
                tokens.push_back({TokenType::INVALID, text, startLine});
            }

            continue;
        }

        if(c == '{')
        {
            tokens.push_back({TokenType::LBRACE, "{", startLine});
            advance();
            continue;
        }

        if(c == '}')
        {
            tokens.push_back({TokenType::RBRACE, "}", startLine});
            advance();
            continue;
        }

        if(c == ';')
        {
            tokens.push_back({TokenType::SEMICOLON, ";", startLine});
            advance();
            continue;
        }

        tokens.push_back({TokenType::INVALID, string(1, c), startLine});
        advance();
    }

    tokens.push_back({TokenType::END_OF_FILE, "", line});

    return tokens;
}

string tokenTypeToString(TokenType type)
{
    switch(type)
    {
        case TokenType::FORM: return "FORM";
        case TokenType::FIELD: return "FIELD";
        case TokenType::STRING_TYPE: return "STRING_TYPE";
        case TokenType::INTEGER_TYPE: return "INTEGER_TYPE";
        case TokenType::EMAIL_TYPE: return "EMAIL_TYPE";
        case TokenType::REQUIRED: return "REQUIRED";
        case TokenType::MIN: return "MIN";
        case TokenType::MAX: return "MAX";
        case TokenType::SUBMIT: return "SUBMIT";
        case TokenType::IDENTIFIER: return "IDENTIFIER";
        case TokenType::NUMBER: return "NUMBER";
        case TokenType::STRING_LITERAL: return "STRING_LITERAL";
        case TokenType::LBRACE: return "LBRACE";
        case TokenType::RBRACE: return "RBRACE";
        case TokenType::SEMICOLON: return "SEMICOLON";
        case TokenType::INVALID: return "INVALID";
        case TokenType::END_OF_FILE: return "END_OF_FILE";
    }

    return "UNKNOWN";
}