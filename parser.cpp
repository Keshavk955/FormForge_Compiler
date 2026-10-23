#include "parser.h"
#include <stdexcept>
#include <limits>

using namespace std;

namespace
{
    int parseIntegerLiteral(const Token& token)
    {
        unsigned long long value;

        try
        {
            value = stoull(token.lexeme);
        }
        catch(const exception&)
        {
            throw runtime_error(
                "Syntax Error at line " +
                to_string(token.line) +
                ": Integer literal is outside the supported range."
            );
        }

        if(value > static_cast<unsigned long long>(
                       numeric_limits<int>::max()))
        {
            throw runtime_error(
                "Syntax Error at line " +
                to_string(token.line) +
                ": Integer literal is outside the supported range."
            );
        }

        return static_cast<int>(value);
    }
}

Parser::Parser(vector<Token> tokens)
{
    this->tokens = tokens;
    current = 0;
}

Token Parser::peek()
{
    return tokens[current];
}

Token Parser::previous()
{
    return tokens[current - 1];
}

bool Parser::check(TokenType type)
{
    return peek().type == type;
}

bool Parser::match(TokenType type)
{
    if(check(type))
    {
        current++;
        return true;
    }

    return false;
}

Token Parser::consume(TokenType type, string message)
{
    if(check(type))
    {
        Token token = peek();
        current++;
        return token;
    }

    throw runtime_error(
        "Syntax Error at line " +
        to_string(peek().line) +
        ": " + message
    );
}

FormNode Parser::parse()
{
    FormNode form = parseForm();

    consume(
        TokenType::END_OF_FILE,
        "Expected end of file."
    );

    return form;
}

FormNode Parser::parseForm()
{
    FormNode form;

    consume(
        TokenType::FORM,
        "Expected 'form'."
    );

    Token name = consume(
        TokenType::IDENTIFIER,
        "Expected form name."
    );

    form.name = name.lexeme;

    consume(
        TokenType::LBRACE,
        "Expected '{' after form name."
    );

    while(check(TokenType::FIELD))
    {
        form.fields.push_back(parseField());
    }

    form.submitText = parseSubmit();

    consume(
        TokenType::RBRACE,
        "Expected '}' after form."
    );

    return form;
}

FieldNode Parser::parseField()
{
    FieldNode field;

    field.required = false;
    field.hasMin = false;
    field.hasMax = false;
    field.minValue = 0;
    field.maxValue = 0;

    bool seenRequired = false;
    bool seenMin = false;
    bool seenMax = false;

    consume(
        TokenType::FIELD,
        "Expected 'field'."
    );

    Token name = consume(
        TokenType::IDENTIFIER,
        "Expected field name."
    );

    field.name = name.lexeme;

    if(match(TokenType::STRING_TYPE))
    {
        field.type = FieldType::STRING;
    }
    else if(match(TokenType::INTEGER_TYPE))
    {
        field.type = FieldType::INTEGER;
    }
    else if(match(TokenType::EMAIL_TYPE))
    {
        field.type = FieldType::EMAIL;
    }
    else
    {
        throw runtime_error(
            "Syntax Error at line " +
            to_string(peek().line) +
            ": Expected field type."
        );
    }

    while(!check(TokenType::SEMICOLON))
    {
        if(check(TokenType::FIELD) ||
           check(TokenType::SUBMIT) ||
           check(TokenType::RBRACE) ||
           check(TokenType::END_OF_FILE))
        {
            throw runtime_error(
                "Syntax Error at line " +
                to_string(peek().line) +
                ": Expected ';' after field definition."
            );
        }

        if(match(TokenType::REQUIRED))
        {
            if(seenRequired)
            {
                throw runtime_error(
                    "Syntax Error at line " +
                    to_string(previous().line) +
                    ": Duplicate 'required' constraint."
                );
            }

            seenRequired = true;
            field.required = true;
        }
        else if(match(TokenType::MIN))
        {
            if(seenMin)
            {
                throw runtime_error(
                    "Syntax Error at line " +
                    to_string(previous().line) +
                    ": Duplicate 'min' constraint."
                );
            }

            seenMin = true;

            Token value = consume(
                TokenType::NUMBER,
                "Expected number after 'min'."
            );

            field.hasMin = true;
            field.minValue = parseIntegerLiteral(value);
        }
        else if(match(TokenType::MAX))
        {
            if(seenMax)
            {
                throw runtime_error(
                    "Syntax Error at line " +
                    to_string(previous().line) +
                    ": Duplicate 'max' constraint."
                );
            }

            seenMax = true;

            Token value = consume(
                TokenType::NUMBER,
                "Expected number after 'max'."
            );

            field.hasMax = true;
            field.maxValue = parseIntegerLiteral(value);
        }
        else
        {
            throw runtime_error(
                "Syntax Error at line " +
                to_string(peek().line) +
                ": Unexpected token '" +
                peek().lexeme + "'."
            );
        }
    }

    consume(
        TokenType::SEMICOLON,
        "Expected ';' after field."
    );

    return field;
}

string Parser::parseSubmit()
{
    consume(
        TokenType::SUBMIT,
        "Expected 'submit'."
    );

    Token text = consume(
        TokenType::STRING_LITERAL,
        "Expected submit text in quotes."
    );

    consume(
        TokenType::SEMICOLON,
        "Expected ';' after submit."
    );

    return text.lexeme;
}

string fieldTypeToString(FieldType type)
{
    switch(type)
    {
        case FieldType::STRING:
            return "string";

        case FieldType::INTEGER:
            return "integer";

        case FieldType::EMAIL:
            return "email";
    }

    return "unknown";
}