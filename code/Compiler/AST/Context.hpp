#ifndef CONTEXT_HPP
#define CONTEXT_HPP

#include "Core/Module.hpp"
#include "Compiler/AST/ScopeBuilder.hpp"
#include "Compiler/AST/Registry.hpp"

class Context
{
public:
    Context();
    Registry& getRegistry();
    Registry& getRegistryFromScope(const std::string& string);
    Registry& getRegistryGlobalScope();

    ScopeBuilder getScopeBuilderCopy() const;

    void pushScope(const std::string& string);
    void popScope();
    void pushClassScope(const std::string& string);
    void popClassScope();
    bool hasScope() const;
    const std::string& getScope() const;
    const std::string& getClassScope() const;
    const std::string& getCurrentClass() const;

    const TypedDataInfo* findTypedData(const std::string& identifier);
    const TypeInfo* findTypedDataTypeInfo(const std::string& identifier);
    std::string findTypedDataTypeInfoIdentifier(const std::string& identifier);
    const TypeInfo* findTypeInfo(const std::string& typeIdentifier);

private:
    void createRegistry(const std::string& string);

public:
    inline static const std::string smGlobalScope = "";
private:
    ScopeBuilder mScopeBuilder;
    ScopeBuilder mClassScopeBuilder;
    
    // <Scope, Registry>
    std::map<std::string, Registry> mRegistryMap; 
};

#endif