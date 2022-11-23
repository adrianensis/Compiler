#include "Compiler/AST/Context.hpp"
#include "Compiler/AST/Nodes/Node.hpp"
#include "Compiler/Lexer/Token.hpp"

Context::Context()
{
    // insert empty scope registry by default
    createRegistry(smGlobalScope);
}

Registry& Context::getRegistry()
{
    return mRegistryMap.at(mScopeBuilder.getScope());
}

Registry& Context::getRegistryFromScope(const std::string& string)
{
    return mRegistryMap.at(string);
}

Registry& Context::getRegistryGlobalScope()
{
    return getRegistryFromScope(smGlobalScope);
}

ScopeBuilder Context::getScopeBuilderCopy() const
{
    return mScopeBuilder;
}

void Context::pushScope(const std::string& string)
{
    mScopeBuilder.pushScope(string);
    
    if(! MAP_CONTAINS(mRegistryMap, mScopeBuilder.getScope()))
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

const TypeInfo* Context::findTypedDataTypeInfo(const std::string& identifier)
{
    return findTypeInfo(findTypedDataTypeInfoIdentifier(identifier));
}

std::string Context::findTypedDataTypeInfoIdentifier(const std::string& identifier)
{
    try
    {
        f32 floatValue = std::stof(identifier);
        return "float";
    }
    catch(std::exception& e)
    {
        try
        {
            i32 integerValue = std::stoi(identifier);
            return "int";
        }
        catch(std::exception& e)
        {
            const TypedDataInfo* typedDataInfo = findTypedData(identifier);
            if(typedDataInfo)
            {
                const TypeInfo* typeInfo = findTypeInfo(typedDataInfo->mType);
                if(typeInfo)
                {
                    if(!typeInfo->mAliasedType.empty())
                    {
                        const TypeInfo* aliasedTypeInfo = findTypeInfo(typeInfo->mAliasedType);
                        while(!aliasedTypeInfo->mAliasedType.empty())
                        {
                            aliasedTypeInfo = findTypeInfo(aliasedTypeInfo->mAliasedType);
                        }

                        return aliasedTypeInfo->mIdentifier;
                    }
                }

                return typedDataInfo->mType;
            }
        }
    }

    return std::string();
}

const TypedDataInfo* Context::findTypedData(const std::string& identifier)
{
    ScopeBuilder scopeBuilder = getScopeBuilderCopy();

    std::string currentScope = scopeBuilder.getScope();

    // check first scope
    bool found = getRegistryFromScope(currentScope).getInfo<TypedDataInfo>(identifier);

    scopeBuilder.popScope();

    while(!found && scopeBuilder.hasScope())
    {
        currentScope = scopeBuilder.getScope();
        found = getRegistryFromScope(currentScope).getInfo<TypedDataInfo>(identifier);
        scopeBuilder.popScope();
    }

    if(!found)
    {
        // check last scope
        found = getRegistryFromScope(scopeBuilder.getScope()).getInfo<TypedDataInfo>(identifier);
        if(found)
        {
            currentScope = scopeBuilder.getScope();
        }    
    }

    if(found)
    {
        return getRegistryFromScope(currentScope).getInfo<TypedDataInfo>(identifier);
    }
    
    return nullptr;
}

const TypeInfo* Context::findTypeInfo(const std::string& typeIdentifier)
{
    ScopeBuilder scopeBuilder = getScopeBuilderCopy();

    std::string currentScope = scopeBuilder.getScope();

    // check first scope
    bool found = getRegistryFromScope(currentScope).getInfo<TypeInfo>(typeIdentifier);

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
            found = getRegistryFromScope(currentScope).getInfo<TypeInfo>(typeIdentifier);
            scopeBuilder.popScope();
        }
    }

    if(!found)
    {
        // check last scope
        found = getRegistryFromScope(scopeBuilder.getScope()).getInfo<TypeInfo>(typeIdentifier);
        if(found)
        {
            currentScope = scopeBuilder.getScope();
        }      
    }

    if(found)
    {
        return getRegistryFromScope(currentScope).getInfo<TypeInfo>(typeIdentifier);
    }
    
    std::cout << "Undefined Type: " << typeIdentifier << std::endl;
    return nullptr;
}
