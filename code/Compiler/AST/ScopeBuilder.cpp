#include "Compiler/AST/ScopeBuilder.hpp"
#include "Compiler/AST/Nodes/Node.hpp"
#include "Compiler/Lexer/Token.hpp"

void ScopeBuilder::pushScope(const std::string& string)
{
    mScopeVector.push_back(string);
    regenerateScope();
}

void ScopeBuilder::popScope()
{
    mScopeVector.pop_back();
    regenerateScope();
}

bool ScopeBuilder::hasScope() const
{
    return !mScopeVector.empty();
}

void ScopeBuilder::regenerateScope()
{
    mScope.clear();
    for (const std::string &scope: mScopeVector)
    {
        if(!mScope.empty())
        {
            mScope += "::";
        }
        mScope += scope;
    }
}
