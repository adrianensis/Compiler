#ifndef CODEBUILDER_HPP
#define CODEBUILDER_HPP

#include "Core/Module.hpp"
#include "Compiler/AST/Nodes/ScopeBuilder.hpp"

class Node;
class Token;
class TokenType;

class CodeBuilder
{
public:
    CodeBuilder() = default;
    
    void addString(const std::string& string);
    void addToken(const Token& token);
    void addTokenType(const TokenType& tokenType);

    void newLine();
    void indent();
    void unindent();

    void addScope();
    
    void includeInHeader(const std::string& string);
    void includeInSource(const std::string& string);

    bool mGenerateHeaderCode = false;

    const std::string& generateCode();

private:
    void addSpaces(u32 spaces);

private:
    std::string mCode;
    std::string mFileName;
    u32 mIndent = 0;
    bool mIsNewLine = true;
    ScopeBuilder mScopeBuilder;
    std::vector<std::string> mHeaderIncludes;
    std::vector<std::string> mSourceIncludes;

public:
    RGET(ScopeBuilder)
    SET(FileName)
};

#endif