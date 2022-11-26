#include "Compiler/Parser/Parser.hpp"
#include "Compiler/AST/Nodes/Statements/Statement.hpp"

void Parser::parse() 
{
    Token token;
    for (token = mLexer.next(); ! token.isOneOf(TokensDefinitions::End, TokensDefinitions::Unexpected); token = mLexer.next()) 
    {
        if(!token.is(TokensDefinitions::Comment))
        {
            mTokens.emplace_back(token);
        }

    }

    if(token.is(TokensDefinitions::End))
    {
        mTokens.emplace_back(token);
    }
    else
    {
        // TODO: Token::Unexpected case
    }
}

const Token* Parser::getCurrentToken() const 
{
    return &mTokens.at(mTokenIndex);
}

const Token* Parser::getNextToken() const
{
    if((mTokenIndex + 1) < mTokens.size())
    {
        return &mTokens.at(mTokenIndex + 1);
    }

    return nullptr;
}

const Token* Parser::advance() 
{
    // if(mTokenIndex + 1 == mTokens.size())
    // {
    //     return nullptr;
    // }

    mTokenIndex++;
    return &mTokens.at(mTokenIndex);
}

const Token* Parser::goBack() 
{
    if(mTokenIndex > 0)
    {
        mTokenIndex--;
    }

    return &mTokens.at(mTokenIndex);
}

const std::string& Parser::getLine(u32 lineNumber) const
{
    return mLexer.getLine(lineNumber);
}