#include "Compiler/AST/Registry.hpp"

void Registry::registerClass(const ClassInfo& info)
{
    if(!isClass(info.mIdentifier.getLexeme()))
    {
        MAP_INSERT(mClassesInfo, info.mIdentifier.getLexeme(), info);
    }
}

bool Registry::isClass(const std::string& name)
{
    return MAP_CONTAINS(mClassesInfo, name);
}

const ClassInfo& Registry::getClass(const std::string& name)
{
    return mClassesInfo[name];
}

void Registry::registerVariable(const VariableInfo& info)
{
    if(!isVariable(info.mIdentifier.getLexeme()))
    {
        MAP_INSERT(mVariablesInfo, info.mIdentifier.getLexeme(), info);
    }
}

bool Registry::isVariable(const std::string& name)
{
    return MAP_CONTAINS(mVariablesInfo, name);
}

const VariableInfo& Registry::getVariable(const std::string& name)
{
    return mVariablesInfo[name];
}

void Registry::registerEnum(const EnumInfo& info)
{
    if(!isEnum(info.mIdentifier.getLexeme()))
    {
        MAP_INSERT(mEnumsInfo, info.mIdentifier.getLexeme(), info);
    }
}

bool Registry::isEnum(const std::string& name)
{
    return MAP_CONTAINS(mEnumsInfo, name);
}

const EnumInfo& Registry::getEnum(const std::string& name)
{
    return mEnumsInfo[name];
}

void Registry::registerFunction(const FunctionInfo& info)
{
    if(!isFunction(info.mIdentifier.getLexeme()))
    {
        MAP_INSERT(mFunctionsInfo, info.mIdentifier.getLexeme(), info);
    }
}

bool Registry::isFunction(const std::string& name)
{
    return MAP_CONTAINS(mFunctionsInfo, name);
}

const FunctionInfo& Registry::getFunction(const std::string& name)
{
    return mFunctionsInfo[name];
}