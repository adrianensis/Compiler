#include "Compiler/Utils/CharUtils.hpp"

bool CharUtils::isNewLine(char c)
{
    return c == '\n';
}

bool CharUtils::isSpace(char c)
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

bool CharUtils::isDigit(char c)
{
    return '0' <= c && c <= '9';
}

bool CharUtils::isAlpha(char c)
{
    return ('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z');
}

bool CharUtils::isIdentifierStartingChar(char c)
{
    return (c == '_' || isAlpha(c));
}

bool CharUtils::isIdentifierChar(char c)
{
    return (CharUtils::isIdentifierStartingChar(c) || CharUtils::isDigit(c));
}

bool CharUtils::isComment(char c, char nextChar)
{
    return c == '/' && (nextChar == '/');
}