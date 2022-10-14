#ifndef REGISTRY_HPP
#define REGISTRY_HPP

#include "Core/Module.hpp"
#include "Compiler/Lexer/Token.hpp"

class BaseInfo
{
public:
    Token mIdentifier;
    std::string mPath;
};

class ClassInfo : public BaseInfo
{
};

class VariableInfo : public BaseInfo
{
public:
    Token mType;
};

class EnumInfo : public BaseInfo
{
};

class FunctionInfo : public BaseInfo
{
};

class Registry
{
private:
    std::map<std::string, ClassInfo> mClassesInfo;
    std::map<std::string, VariableInfo> mVariablesInfo;
    std::map<std::string, EnumInfo> mEnumsInfo;
    std::map<std::string, FunctionInfo> mFunctionsInfo;

public:
    Registry() = default;
    void init();
    void registerClass(const ClassInfo& info);
    bool isClass(const std::string& name);
    const ClassInfo& getClass(const std::string& name);
    void registerVariable(const VariableInfo& info);
    bool isVariable(const std::string& name);
    const VariableInfo& getVariable(const std::string& name);
    void registerEnum(const EnumInfo& info);
    bool isEnum(const std::string& name);
    const EnumInfo& getEnum(const std::string& name);
    void registerFunction(const FunctionInfo& info);
    bool isFunction(const std::string& name);
    const FunctionInfo& getFunction(const std::string& name);
};

#endif