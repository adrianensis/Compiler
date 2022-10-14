#ifndef PARSER_HPP
#define PARSER_HPP

#include "Core/Module.hpp"
#include "Compiler/Lexer/Lexer.hpp"

class Registry;

class ParserUtils 
{
public:
};

class Node;

class Parser 
{
public:
    Parser() = default;
    Parser(const std::string& inputString, Registry& registry) 
    {
        mRegistry = &registry;
        mInputString = inputString;
        mLexer = Lexer(mInputString.c_str());
    }

    void parse();
    const Token* getCurrentToken() const;
    const Token* getNextToken() const;
    const Token* advance();
    const Token* goBack();

    const std::string& getLine(u32 lineNumber) const;

    Registry& getRegistry() const
    {
        return *mRegistry;
    }

public:
    public: std::string mPath;

private:
    Lexer mLexer;
    std::string mInputString;
    Registry* mRegistry = nullptr;
    std::vector<Token> mTokens;
    u32 mTokenIndex = 0;
};

#endif