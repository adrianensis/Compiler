#ifndef SCOPEBUILDER_HPP
#define SCOPEBUILDER_HPP

#include "Core/Module.hpp"

class ScopeBuilder
{
public:
    ScopeBuilder() = default;
    
    void pushScope(const std::string& string);
    void popScope();
    bool hasScope() const;

private:
    void regenerateScope();

public:
    std::string mScope;
    std::vector<std::string> mScopeVector;

public:
    CRGET(Scope)
    CRGET(ScopeVector)
};

#endif