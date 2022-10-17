#ifndef CHARUTILS_HPP
#define CHARUTILS_HPP

#include "Core/Module.hpp"

class CharUtils 
{
public:
    static bool isSpace(char c);
    static bool isNewLine(char c);
    static bool isDigit(char c);
    static bool isAlpha(char c);
    static bool isIdentifierStartingChar(char c);
    static bool isIdentifierChar(char c);
    static bool isComment(char c, char nextChar);
};

#endif