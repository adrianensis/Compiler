#ifndef REGISTRY_HPP
#define REGISTRY_HPP

#include "Compiler/Lexer/Token.hpp"
#include "Core/Assert/Assert.hpp"
#include <unordered_map>
#include <typeinfo>
#include <iostream>

enum class DataType {
    TYPE_PRIMITIVE,
    TYPE_CLASS,
    TYPE_ENUM
};

class BaseInfo
{
public:
    std::string mIdentifier = "";
    std::string mPath = "";

    virtual const std::string& getKey() const { return mIdentifier; }
};

class TypeInfo : public BaseInfo
{
public:
    DataType mDataType = DataType::TYPE_PRIMITIVE;
    bool mIsStack = false;
    std::string mAliasedType = "";
};

class TypedDataInfo : public BaseInfo
{
public:
    std::string mType = "";
    bool mIsConst = false;
    bool mIsStatic = false;
};

class VariableInfo : public TypedDataInfo
{
};

class FunctionInfo : public TypedDataInfo
{
public:
    bool mIsConstructor = false;
};

class Registry
{
private:
    std::unordered_map<std::string, TypeInfo> mTypesInfo;
    std::unordered_map<std::string, VariableInfo> mVariablesInfo;
    std::unordered_map<std::string, FunctionInfo> mFunctionsInfo;

public:
    Registry() = default;
    void init();

    template<class T>
    bool registerInfo(const T& info)
    {
        const std::string key = info.getKey();
        bool registered = false;

        if constexpr(std::is_same<T, TypeInfo>::value)
        {
            if(getInfo<TypeInfo>(key))
            {
                std::cout << "Duplicated " + info.getKey() << std::endl;
            }
            else
            {
                mTypesInfo.emplace(key, info);
                registered = true;
            }
        }
        else if constexpr(std::is_same<T, VariableInfo>::value)
        {
            if(getInfo<VariableInfo>(key))
            {
                std::cout << "Duplicated " + info.getKey() << std::endl;
            }
            else
            {
                mVariablesInfo.emplace(key, info);
                registered = true;
            }
        }
        else if constexpr(std::is_same<T, FunctionInfo>::value)
        {
            if(getInfo<FunctionInfo>(key))
            {
                std::cout << "Duplicated " + info.getKey() << std::endl;
            }
            else
            {
                mFunctionsInfo.emplace(key, info);
                registered = true;
            }
        }
        else
        {
            CHECK_MSG(false, std::string(typeid(T).name()) + " is not supported!");
        }

        return registered;
    }

    template<class T>
    const T* getInfo(const std::string& key) const
    {
        if constexpr(std::is_same<T, TypeInfo>::value)
        {
            return mTypesInfo.contains(key) ? &mTypesInfo.at(key) : nullptr;
        }
        else if constexpr(std::is_same<T, TypedDataInfo>::value)
        {
            const VariableInfo* variableInfo = getInfo<VariableInfo>(key);
            const FunctionInfo* functionInfo = getInfo<FunctionInfo>(key);
            if(variableInfo)
            {
                return variableInfo;
            }
            else
            {
                return functionInfo;
            }
        }
        if constexpr(std::is_same<T, VariableInfo>::value)
        {
            return mVariablesInfo.contains(key) ? &mVariablesInfo.at(key) : nullptr;
        }
        else if constexpr(std::is_same<T, FunctionInfo>::value)
        {
            return mFunctionsInfo.contains(key) ? &mFunctionsInfo.at(key) : nullptr;
        }
        else
        {
            CHECK_MSG(false, std::string(typeid(T).name()) + " is not supported!");
        }

        return nullptr;
    }
};

#endif