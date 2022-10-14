#include "Compiler/Lexer/Lexer.hpp"

bool LexerUtils::isNewLine(char c)
{
    return c == '\n';
}

bool LexerUtils::isSpace(char c)
{
    if(isNewLine(c))
    {
        return true;
    }

    switch (c)
    {
        case ' ':
        case '\t':
        // case '\r':
            return true;
        default:
            return false;
    }
}

bool LexerUtils::isDigit(char c)
{
    return '0' <= c && c <= '9';
}

bool LexerUtils::isAlpha(char c)
{
    return ('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z');
}

bool LexerUtils::isIdentifierStartingChar(char c)
{
    return (c == '_' || isAlpha(c));
}

bool LexerUtils::isIdentifierChar(char c)
{
    return (LexerUtils::isIdentifierStartingChar(c) || LexerUtils::isDigit(c));
}

bool LexerUtils::isComment(char c, char nextChar)
{
    return c == '/' && (nextChar == '/');
}

Token Lexer::next()
{
    skipSpaces();

    char currentChar = getCurrentChar();
    char nextChar = getNextChar();

    if(LexerUtils::isIdentifierStartingChar(currentChar))
    {
        return processIdentifier();
    }
    else if(LexerUtils::isDigit(currentChar))
    {
        return processNumber();
    }
    else if(LexerUtils::isComment(currentChar, nextChar))
    {
        return processComment();
    }
    else
    {
        switch (currentChar) {
        case '\0':
            return Token(TokensDefinitions::End, mInputStream, 1, mLineNumber - 1);
        default:
            return processSimpleToken(TokensDefinitions::Unexpected);
        case '(':
            return processSimpleToken(TokensDefinitions::LeftParen);
        case ')':
            return processSimpleToken(TokensDefinitions::RightParen);
        case '[':
            return processSimpleToken(TokensDefinitions::LeftSquare);
        case ']':
            return processSimpleToken(TokensDefinitions::RightSquare);
        case '{':
            return processSimpleToken(TokensDefinitions::LeftCurly);
        case '}':
            return processSimpleToken(TokensDefinitions::RightCurly);
        case '<':
            return processSimpleToken(TokensDefinitions::LessThan);
        case '>':
            return processSimpleToken(TokensDefinitions::GreaterThan);
        case '=':
            return processSimpleToken(TokensDefinitions::Equal);
        case '+':
            return processSimpleToken(TokensDefinitions::Plus);
        case '-':
            return processSimpleToken(TokensDefinitions::Minus);
        case '*':
            return processSimpleToken(TokensDefinitions::Asterisk);
        case '/':
            return processSimpleToken(TokensDefinitions::Slash);
        case '#':
            return processSimpleToken(TokensDefinitions::Hash);
        case '.':
            return processSimpleToken(TokensDefinitions::Dot);
        case ',':
            return processSimpleToken(TokensDefinitions::Comma);
        case ':':
            return processSimpleToken(TokensDefinitions::Colon);
        case ';':
            return processSimpleToken(TokensDefinitions::Semicolon);
        case '\'':
            return processSimpleToken(TokensDefinitions::SingleQuote);
        case '"':
            return processSimpleToken(TokensDefinitions::DoubleQuote);
        case '|':
            return processSimpleToken(TokensDefinitions::Pipe);
        }
    }
}

Token Lexer::processIdentifier()
{
    const char* start = mInputStream;
    advance();
    skipIdentifierChars();

    Token identifier(TokensDefinitions::Identifier, start, mInputStream, mLineNumber);

    if(identifier.isKeyword())
    {
        return Token(TokenType::getKeywordTokenType(identifier.getLexeme()), start, mInputStream, mLineNumber);
    }

    return identifier;
}

Token Lexer::processNumber()
{
    const char* start = mInputStream;
    advance();
    skipDigits();
    return Token(TokensDefinitions::Number, start, mInputStream, mLineNumber);
}

Token Lexer::processComment()
{
    const char* start = mInputStream;
    advance();
    advance();
    start = mInputStream;
    while (getCurrentChar() != '\0')
    {
        char nextChar = advance();
        if (LexerUtils::isNewLine(nextChar) || /*nextChar == '\r' ||*/ nextChar == '\0')
        {
            return Token(TokensDefinitions::Comment, start, std::distance(start, mInputStream) - 1, mLineNumber);
        }
    }
    return Token(TokensDefinitions::Comment, start, mInputStream, mLineNumber);
}

Token Lexer::processSimpleToken(const TokenType& tokenType)
{
    Token simple(tokenType, mInputStream, 1, mLineNumber);
    advance();
    return simple;
}

void Lexer::skipSpaces()
{
    while (LexerUtils::isSpace(getCurrentChar()))
    {
        advance();
    }
}

void Lexer::skipDigits()
{
    while (LexerUtils::isDigit(getCurrentChar()))
    {
        advance();
    }
}

void Lexer::skipIdentifierChars()
{
    while (LexerUtils::isIdentifierChar(getCurrentChar()))
    {
        advance();
    }
}

char Lexer::advance()
{
    char currentChar = getCurrentChar();
    if(LexerUtils::isNewLine(currentChar))
    {
        mLineNumber++;
        mLines.push_back(mCurrentLine);
        mCurrentLine.clear();
    }
    else
    {
        mCurrentLine += currentChar;
    }

    return *mInputStream++;
}

const std::string& Lexer::getLine(u32 lineNumber) const
{
    return mLines.at(lineNumber);
}
