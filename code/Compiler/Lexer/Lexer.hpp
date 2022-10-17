#ifndef LEXER_HPP
#define LEXER_HPP

#include "Core/Module.hpp"
#include "Compiler/Lexer/Token.hpp"

class Lexer 
{
public:
    Lexer() = default;
    Lexer(const char* inputStream) : mInputStream{inputStream} {}

    Token next();
    const std::string& getLine(u32 lineNumber) const;

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
    u32 mLineNumber = 0;
    std::string mCurrentLine = "";
    std::vector<std::string> mLines;

public:
    GET(LineNumber)
};

#endif