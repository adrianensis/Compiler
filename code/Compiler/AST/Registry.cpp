#include "Compiler/AST/Registry.hpp"

void Registry::registerType(const TypeInfo& info)
{
    if(!getType(info))
    {
        MAP_INSERT(mTypesInfo, info.getFullName(), info);
    }
}

const TypeInfo* Registry::getType(const TypeInfo& info)
{
    std::string key = info.getFullName();
    return MAP_CONTAINS(mTypesInfo, key) ? &mTypesInfo.at(key) : nullptr;
}

void Registry::registerClass(const ClassInfo& info)
{
    if(!getClass(info))
    {
        MAP_INSERT(mClassesInfo, info.getFullName(), info);
    }
}

const ClassInfo* Registry::getClass(const ClassInfo& info)
{
    std::string key = info.getFullName();
    return MAP_CONTAINS(mClassesInfo, key) ? &mClassesInfo.at(key) : nullptr;
}

void Registry::registerVariable(const VariableInfo& info)
{
    if(!getVariable(info))
    {
        MAP_INSERT(mVariablesInfo, info.getFullName(), info);
    }
}

const VariableInfo* Registry::getVariable(const VariableInfo& info)
{
    std::string key = info.getFullName();
    return MAP_CONTAINS(mVariablesInfo, key) ? &mVariablesInfo.at(key) : nullptr;
}

void Registry::registerEnum(const EnumInfo& info)
{
    if(!getEnum(info))
    {
        MAP_INSERT(mEnumsInfo, info.getFullName(), info);
    }
}

const EnumInfo* Registry::getEnum(const EnumInfo& info)
{
    std::string key = info.getFullName();
    return MAP_CONTAINS(mEnumsInfo, key) ? &mEnumsInfo.at(key) : nullptr;
}

void Registry::registerFunction(const FunctionInfo& info)
{
    if(!getFunction(info))
    {
        MAP_INSERT(mFunctionsInfo, info.getFullName(), info);
    }
}

const FunctionInfo* Registry::getFunction(const FunctionInfo& info)
{
    std::string key = info.getFullName();
    return MAP_CONTAINS(mFunctionsInfo, key) ? &mFunctionsInfo.at(key) : nullptr;
}