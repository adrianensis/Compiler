#ifndef SCOPEBUILDER_HPP
#define SCOPEBUILDER_HPP

#include <vector>
#include <string>

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
    const std::string& getScope() const { return mScope; }
    const std::vector<std::string>& getScopeVector() const { return mScopeVector; }
};

#endif