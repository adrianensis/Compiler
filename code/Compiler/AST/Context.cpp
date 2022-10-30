#include "Compiler/AST/Context.hpp"
#include "Compiler/AST/Nodes/Node.hpp"
#include "Compiler/Lexer/Token.hpp"

Context::Context()
{
    // insert empty scope registry by default
    createRegistry(std::string());
}

Registry& Context::getRegistry()
{
    return mRegistryMap.at(mScopeBuilder.getScope());
}

Registry& Context::getRegistryFromScope(const std::string& string)
{
    return mRegistryMap.at(string);
}

ScopeBuilder Context::getScopeBuilderCopy() const
{
    return mScopeBuilder;
}

void Context::pushScope(const std::string& string)
{
    mScopeBuilder.pushScope(string);
    
    if(!MAP_CONTAINS(mRegistryMap, mScopeBuilder.getScope()))
    {
        createRegistry(mScopeBuilder.getScope());
    }
}

void Context::popScope()
{
    mScopeBuilder.popScope();
}

void Context::pushClassScope(const std::string& string)
{
    mClassScopeBuilder.pushScope(string);
}

void Context::popClassScope()
{
    mClassScopeBuilder.popScope();
}

bool Context::hasScope() const
{
    return mScopeBuilder.hasScope();
}

void Context::createRegistry(const std::string& string)
{
    MAP_INSERT(mRegistryMap, string, Registry());
}

const std::string& Context::getScope() const
{
    return mScopeBuilder.getScope();
}

const std::string& Context::getClassScope() const
{
    return mClassScopeBuilder.getScope();
}

const std::string& Context::getCurrentClass() const
{
    if(!mClassScopeBuilder.hasScope())
    {
        // return empty scope
        return mClassScopeBuilder.getScope();
    }

    return mClassScopeBuilder.getScopeVector().back();
}

const TypedDataInfo* Context::findTypedData(const std::string& identifier)
{
    ScopeBuilder scopeBuilder = getScopeBuilderCopy();

    std::string currentScope = scopeBuilder.getScope();

    TypedDataInfo typedDataInfo;
    typedDataInfo.mIdentifier = identifier;

    // check first scope
    bool found = getRegistryFromScope(currentScope).getInfo(typedDataInfo);

    scopeBuilder.popScope();

    while(!found && scopeBuilder.hasScope())
    {
        currentScope = scopeBuilder.getScope();
        found = getRegistryFromScope(currentScope).getInfo(typedDataInfo);
        scopeBuilder.popScope();
    }

    if(!found)
    {
        // check last scope
        found = getRegistryFromScope(scopeBuilder.getScope()).getInfo(typedDataInfo);
        if(found)
        {
            currentScope = scopeBuilder.getScope();
        }    
    }

    if(found)
    {
        return getRegistryFromScope(currentScope).getInfo(typedDataInfo);
    }
    
    return nullptr;
}

const TypeInfo* Context::findTypeInfo(const std::string& typeIdentifier)
{
    ScopeBuilder scopeBuilder = getScopeBuilderCopy();

    std::string currentScope = scopeBuilder.getScope();

    TypeInfo typeInfo;
    typeInfo.mIdentifier = typeIdentifier;

    // check first scope
    bool found = getRegistryFromScope(currentScope).getInfo(typeInfo);

    if(!found)
    {
        if(!scopeBuilder.hasScope())
        {
            std::cout << "Undefined Type: " << typeIdentifier << std::endl;
            return nullptr;
        }

        scopeBuilder.popScope();

        while(!found && scopeBuilder.hasScope())
        {
            currentScope = scopeBuilder.getScope();
            found = getRegistryFromScope(currentScope).getInfo(typeInfo);
            scopeBuilder.popScope();
        }
    }

    if(!found)
    {
        // check last scope
        found = getRegistryFromScope(scopeBuilder.getScope()).getInfo(typeInfo);
        if(found)
        {
            currentScope = scopeBuilder.getScope();
        }      
    }

    if(found)
    {
        return getRegistryFromScope(currentScope).getInfo(typeInfo);
    }
    
    std::cout << "Undefined Type: " << typeIdentifier << std::endl;
    return nullptr;
}
