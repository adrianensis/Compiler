#include "Compiler/Lexer/Lexer.hpp"
#include "Compiler/Utils/CharUtils.hpp"

Token Lexer::next()
{
    skipSpaces();

    char currentChar = getCurrentChar();
    char nextChar = getNextChar();

    if(CharUtils::isIdentifierStartingChar(currentChar))
    {
        return processIdentifier();
    }
    else if(CharUtils::isDigit(currentChar))
    {
        return processNumber();
    }
    else if(CharUtils::isComment(currentChar, nextChar))
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
            if(TokensDefinitions::Equal.matchString(nextChar))
            {
                return processSimpleToken(TokensDefinitions::LessThanEqual);
            }
            return processSimpleToken(TokensDefinitions::LessThan);
        case '>':
            if(TokensDefinitions::Equal.matchString(nextChar))
            {
                return processSimpleToken(TokensDefinitions::GreaterThanEqual);
            }
            return processSimpleToken(TokensDefinitions::GreaterThan);
        case '=':
            if(TokensDefinitions::Equal.matchString(nextChar))
            {
                return processSimpleToken(TokensDefinitions::EqualTo);
            }
            return processSimpleToken(TokensDefinitions::Equal);
        case '+':
            if(TokensDefinitions::Plus.matchString(nextChar))
            {
                return processSimpleToken(TokensDefinitions::Increment);
            }
            else if(TokensDefinitions::Equal.matchString(nextChar))
            {
                return processSimpleToken(TokensDefinitions::PlusEqual);
            }
            return processSimpleToken(TokensDefinitions::Plus);
        case '-':
            if(TokensDefinitions::Minus.matchString(nextChar))
            {
                return processSimpleToken(TokensDefinitions::Decrement);
            }
            else if(TokensDefinitions::Equal.matchString(nextChar))
            {
                return processSimpleToken(TokensDefinitions::MinusEqual);
            }
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
            if(TokensDefinitions::Colon.matchString(nextChar))
            {
                return processSimpleToken(TokensDefinitions::Scope);
            }
            return processSimpleToken(TokensDefinitions::Colon);
        case ';':
            return processSimpleToken(TokensDefinitions::Semicolon);
        case '\'':
            return processSimpleToken(TokensDefinitions::SingleQuote);
        case '"':
            return processSimpleToken(TokensDefinitions::DoubleQuote);
        case '|':
            if(TokensDefinitions::Pipe.matchString(nextChar))
            {
                return processSimpleToken(TokensDefinitions::Or);
            }
            return processSimpleToken(TokensDefinitions::Pipe);
        case '&':
            if(TokensDefinitions::Ampersand.matchString(nextChar))
            {
                return processSimpleToken(TokensDefinitions::And);
            }
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
        if (CharUtils::isNewLine(nextChar) || /*nextChar == '\r' ||*/ nextChar == '\0')
        {
            return Token(TokensDefinitions::Comment, start, std::distance(start, mInputStream) - 1, mLineNumber);
        }
    }
    return Token(TokensDefinitions::Comment, start, mInputStream, mLineNumber);
}

Token Lexer::processSimpleToken(const TokenType& tokenType)
{
    u32 tokenCharsSize = tokenType.getValue().size();
    Token simple(tokenType, mInputStream, tokenCharsSize, mLineNumber);
    FOR_RANGE(i, 0, tokenCharsSize)
    {
        advance();
    }
    
    return simple;
}

void Lexer::skipSpaces()
{
    while (CharUtils::isSpace(getCurrentChar()))
    {
        advance();
    }
}

void Lexer::skipDigits()
{
    while (CharUtils::isDigit(getCurrentChar()))
    {
        advance();
    }
}

void Lexer::skipIdentifierChars()
{
    while (CharUtils::isIdentifierChar(getCurrentChar()))
    {
        advance();
    }
}

char Lexer::advance()
{
    char currentChar = getCurrentChar();
    if(CharUtils::isNewLine(currentChar))
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
