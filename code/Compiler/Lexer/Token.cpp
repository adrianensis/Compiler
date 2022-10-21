#include "Compiler/Lexer/Token.hpp"

TokenType::TokenType(const std::string& name, const std::string& value, bool isKeyword, bool isPrimitive)
{
    mId = smTokenTypeCounter;
    smTokenTypeCounter++;
    mName = name;
    mValue = value;

    if(isKeyword)
    {
        smKeywordsMap.emplace(value, *this);
    }
    
    if(isPrimitive)
    {
        smPrimitivesMap.emplace(value, *this);
    }
}

bool TokenType::isKeyword(const std::string& keyword) 
{
    return MAP_CONTAINS(smKeywordsMap, keyword);
}

bool TokenType::isPrimitive(const std::string& primitive) 
{
    return MAP_CONTAINS(smPrimitivesMap, primitive);
}

const TokenType& TokenType::getKeywordTokenType(const std::string& keyword)
{
    return smKeywordsMap.at(keyword);
}

bool Token::isKeyword() const
{
    return TokenType::isKeyword(getLexeme());
}

bool Token::isPrimitive() const
{
    return TokenType::isPrimitive(getLexeme());
}