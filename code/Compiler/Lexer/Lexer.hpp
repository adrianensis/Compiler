#ifndef LEXER_HPP
#define LEXER_HPP

#include "Compiler/Lexer/Token.hpp"
#include <vector>

class Lexer 
{
public:
    Lexer() = default;
    Lexer(const char* inputStream) : mInputStream{inputStream} {}

    Token next();
    const std::string& getLine(unsigned int lineNumber) const;

private:
    Token processIdentifier();
    Token processNumber();
    Token processComment();
    Token processSimpleToken(const TokenType& tokenType);

    void skipSpaces();
    void skipDigits();
    void skipIdentifierChars();

    char getCurrentChar() const { return *mInputStream; }
    char getNextChar() const { return *(mInputStream + 1); }
    char advance();

private:
    const char* mInputStream = nullptr;
    unsigned int mLineNumber = 0;
    std::string mCurrentLine = "";
    std::vector<std::string> mLines;

public:
    unsigned int getLineNumber() const { return mLineNumber; }
};

#endif