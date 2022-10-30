#ifndef PARSER_HPP
#define PARSER_HPP

#include "Core/Module.hpp"
#include "Compiler/Lexer/Lexer.hpp"
#include "Compiler/AST/ScopeBuilder.hpp"

class Registry;
class Node;

class Parser 
{
public:
    Parser() = default;
    Parser(const std::string& inputString) 
    {
        mInputString = inputString;
        mLexer = Lexer(mInputString.c_str());
    }

    void parse();
    const Token* getCurrentToken() const;
    const Token* getNextToken() const;
    const Token* advance();
    const Token* goBack();

    const std::string& getLine(u32 lineNumber) const;

public:
    public: std::string mPath;

private:
    Lexer mLexer;
    std::string mInputString;
    std::vector<Token> mTokens;
    u32 mTokenIndex = 0;
};

#endif