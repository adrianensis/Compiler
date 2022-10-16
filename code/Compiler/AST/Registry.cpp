#include "Compiler/AST/Registry.hpp"

void Registry::registerClass(const ClassInfo& info)
{
    if(!isClass(info.mScope, info.mIdentifier.getLexeme()))
    {
        MAP_INSERT(mClassesInfo, info.getFullName(), info);
    }
}

bool Registry::isClass(const std::string& scope, const std::string& name)
{
    return MAP_CONTAINS(mClassesInfo, scope + name);
}

const ClassInfo& Registry::getClass(const std::string& scope, const std::string& name)
{
    return mClassesInfo.at(scope + name);
}

void Registry::registerVariable(const VariableInfo& info)
{
    if(!isVariable(info.mScope, info.mIdentifier.getLexeme()))
    {
        MAP_INSERT(mVariablesInfo, info.getFullName(), info);
    }
}

bool Registry::isVariable(const std::string& scope, const std::string& name)
{
    return MAP_CONTAINS(mVariablesInfo, scope + name);
}

const VariableInfo& Registry::getVariable(const std::string& scope, const std::string& name)
{
    return mVariablesInfo.at(scope + name);
}

void Registry::registerEnum(const EnumInfo& info)
{
    if(!isEnum(info.mScope, info.mIdentifier.getLexeme()))
    {
        MAP_INSERT(mEnumsInfo, info.getFullName(), info);
    }
}

bool Registry::isEnum(const std::string& scope, const std::string& name)
{
    return MAP_CONTAINS(mEnumsInfo, scope + name);
}

const EnumInfo& Registry::getEnum(const std::string& scope, const std::string& name)
{
    return mEnumsInfo.at(scope + name);
}

void Registry::registerFunction(const FunctionInfo& info)
{
    if(!isFunction(info.mScope, info.mIdentifier.getLexeme()))
    {
        MAP_INSERT(mFunctionsInfo, info.getFullName(), info);
    }
}

bool Registry::isFunction(const std::string& scope, const std::string& name)
{
    return MAP_CONTAINS(mFunctionsInfo, scope + name);
}

const FunctionInfo& Registry::getFunction(const std::string& scope, const std::string& name)
{
    return mFunctionsInfo.at(scope + name);
}