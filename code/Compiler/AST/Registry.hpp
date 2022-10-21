#ifndef REGISTRY_HPP
#define REGISTRY_HPP

#include "Core/Module.hpp"
#include "Compiler/Lexer/Token.hpp"

class BaseInfo
{
public:
    Token mIdentifier;
    Token mType;
    std::string mScope;
    std::string mPath;

    virtual std::string getFullName() const { return mScope + mIdentifier.getLexeme(); }
};

class TypeInfo : public BaseInfo
{
};

class ClassInfo : public BaseInfo
{
};

class VariableInfo : public BaseInfo
{
public:
    std::string getFullName() const override { return mScope + mType.getLexeme() + mIdentifier.getLexeme(); }
};

class EnumInfo : public BaseInfo
{
};

class FunctionInfo : public BaseInfo
{
public:
    std::string getFullName() const override { return mScope + mType.getLexeme() + mIdentifier.getLexeme(); }
};

class Registry
{
private:
    std::map<std::string, ClassInfo> mClassesInfo;
    std::map<std::string, TypeInfo> mTypesInfo;
    std::map<std::string, VariableInfo> mVariablesInfo;
    std::map<std::string, EnumInfo> mEnumsInfo;
    std::map<std::string, FunctionInfo> mFunctionsInfo;

public:
    Registry() = default;
    void init();

    void registerType(const TypeInfo& info);
    const TypeInfo* getType(const TypeInfo& info);    

    void registerClass(const ClassInfo& info);
    const ClassInfo* getClass(const ClassInfo& info);

    void registerVariable(const VariableInfo& info);
    const VariableInfo* getVariable(const VariableInfo& info);

    void registerEnum(const EnumInfo& info);
    const EnumInfo* getEnum(const EnumInfo& info);

    void registerFunction(const FunctionInfo& info);
    const FunctionInfo* getFunction(const FunctionInfo& info);
};

#endif