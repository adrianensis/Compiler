#ifndef CODEBUILDER_HPP
#define CODEBUILDER_HPP

#include <string>
#include <vector>

class Node;
class Token;
class TokenType;
class Context;

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

    void addScope(const Context& context);
    
    void includeInHeader(const std::string& string);
    void includeInSource(const std::string& string);
    void forwardInHeader(const std::string& string);
    void forwardInSource(const std::string& string);

    bool mGenerateHeaderCode = false;

    const std::string& generateCode();

private:
    void addSpaces(unsigned int spaces);

private:
    std::string mCode;
    std::string mFileName;
    unsigned int mIndent = 0;
    bool mIsNewLine = true;
    std::vector<std::string> mHeaderIncludes;
    std::vector<std::string> mSourceIncludes;
    std::vector<std::string> mHeaderForwards;
    std::vector<std::string> mSourceForwards;

public:
    void setFileName(const std::string& fileName) { mFileName = fileName; }
};

#endif