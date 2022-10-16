#include "Compiler/Lexer/Token.hpp"

TokenType::TokenType(const std::string& name, const std::string& value, bool isKeyword)
{
    mId = smTokenTypeCounter;
    smTokenTypeCounter++;
    mName = name;
    mValue = value;

    if(isKeyword)
    {
        smKeywordsMap.emplace(value, *this);
    }
}

bool TokenType::isKeyword(const std::string& keyword) 
{
    return MAP_CONTAINS(smKeywordsMap, keyword);
}

const TokenType& TokenType::getKeywordTokenType(const std::string& keyword)
{
    return smKeywordsMap[keyword];
}

bool Token::isKeyword() 
{
    return TokenType::isKeyword(getLexeme());
}