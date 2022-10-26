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
    Token mIdentifier;
    Token mType;
    std::string mScope = "";
    std::string mPath = "";

    virtual std::string getFullName() const { return mScope + mIdentifier.getLexeme(); }
};

class TypeInfo : public BaseInfo
{
public:
    DataType mDataType = DataType::TYPE_PRIMITIVE;
    bool mIsStack = false;
};

class VariableInfo : public BaseInfo
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
    std::map<std::string, TypeInfo> mTypesInfo;
    std::map<std::string, VariableInfo> mVariablesInfo;
    std::map<std::string, FunctionInfo> mFunctionsInfo;

public:
    Registry() = default;
    void init();

    template<class T>
    bool registerInfo(const T& info)
    {
        if(getInfo(info))
        {
            std::cout << "Duplicated " + info.mIdentifier.getLexeme() << std::endl;
        }
        else
        {
            std::cout << "REGISTER " << typeid(T).name() << " " << info.getFullName() << std::endl;

            if constexpr(std::is_same<T, TypeInfo>::value)
            {
                MAP_INSERT(mTypesInfo, info.getFullName(), info);
                return true;
            }
            else if constexpr(std::is_same<T, VariableInfo>::value)
            {
                MAP_INSERT(mVariablesInfo, info.getFullName(), info);
                return true;
            }
            else if constexpr(std::is_same<T, FunctionInfo>::value)
            {
                MAP_INSERT(mFunctionsInfo, info.getFullName(), info);
                return true;
            }
            else
            {
                ASSERT_MSG(false, std::string(typeid(T).name()) + " is not supported!");
            }
        }

        return false;
    }

    template<class T>
    const T* getInfo(const T& info) const
    {
        std::string key = info.getFullName();

        if constexpr(std::is_same<T, TypeInfo>::value)
        {
            return MAP_CONTAINS(mTypesInfo, key) ? &mTypesInfo.at(key) : nullptr;
        }
        else if constexpr(std::is_same<T, VariableInfo>::value)
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