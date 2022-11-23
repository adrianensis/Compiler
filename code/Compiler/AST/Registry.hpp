#ifndef REGISTRY_HPP
#define REGISTRY_HPP

#include "Core/Module.hpp"
#include "Compiler/Lexer/Token.hpp"

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

    virtual std::string getKey() const { return mIdentifier; }
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
    std::map<std::string, TypeInfo> mTypesInfo;
    std::map<std::string, VariableInfo> mVariablesInfo;
    std::map<std::string, FunctionInfo> mFunctionsInfo;

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
                MAP_INSERT(mTypesInfo, key, info);
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
                MAP_INSERT(mVariablesInfo, key, info);
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
                MAP_INSERT(mFunctionsInfo, key, info);
                registered = true;
            }
        }
        else
        {
            ASSERT_MSG(false, std::string(typeid(T).name()) + " is not supported!");
        }

        if(registered)
        {
            std::cout << "REGISTER " << typeid(T).name() << " " << info.getKey() << std::endl;
        }

        return registered;
    }

    template<class T>
    const T* getInfo(const std::string& key) const
    {
        if constexpr(std::is_same<T, TypeInfo>::value)
        {
            return MAP_CONTAINS(mTypesInfo, key) ? &mTypesInfo.at(key) : nullptr;
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
            return MAP_CONTAINS(mVariablesInfo, key) ? &mVariablesInfo.at(key) : nullptr;
        }
        else if constexpr(std::is_same<T, FunctionInfo>::value)
        {
            return MAP_CONTAINS(mFunctionsInfo, key) ? &mFunctionsInfo.at(key) : nullptr;
        }
        else
        {
            ASSERT_MSG(false, std::string(typeid(T).name()) + " is not supported!");
        }

        return nullptr;
    }
};

#endif