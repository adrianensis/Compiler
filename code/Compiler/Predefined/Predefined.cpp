#include "Compiler/Predefined/Predefined.hpp"
#include "Compiler/AST/Nodes/Statements/Statement.hpp"
#include "Compiler/AST/Nodes/CodeBuilder.hpp"

void Predefined::predefine()
{
    auto predefinePrimitiveType = [&](const std::string& name)
    {
        TypeInfo typeInfoPrimitive;
        typeInfoPrimitive.mIdentifier = name;
        typeInfoPrimitive.mDataType = DataType::TYPE_PRIMITIVE;
        typeInfoPrimitive.mIsStack = true;
        mContext->getRegistry().registerInfo(typeInfoPrimitive);
    };

    predefinePrimitiveType(TokensDefinitions::Float.getValue());
    predefinePrimitiveType(TokensDefinitions::Double.getValue());
    predefinePrimitiveType(TokensDefinitions::Int.getValue());
    predefinePrimitiveType(TokensDefinitions::Bool.getValue());
    predefinePrimitiveType(TokensDefinitions::Void.getValue());

    VariableInfo variableInfoTrue;
    variableInfoTrue.mIdentifier = TokensDefinitions::True.getValue();
    variableInfoTrue.mType = TokensDefinitions::Bool.getValue();
    mContext->getRegistry().registerInfo(variableInfoTrue);
    
    VariableInfo variableInfoFalse;
    variableInfoFalse.mIdentifier = TokensDefinitions::False.getValue();
    variableInfoFalse.mType = TokensDefinitions::Bool.getValue();
    mContext->getRegistry().registerInfo(variableInfoFalse);
    
    mContext->pushScope("std");

    predefinePrimitiveType("int8_t");
    predefinePrimitiveType("int16_t");
    predefinePrimitiveType("int32_t");
    predefinePrimitiveType("int64_t");
    predefinePrimitiveType("uint8_t");
    predefinePrimitiveType("uint8_t");
    predefinePrimitiveType("uint16_t");
    predefinePrimitiveType("uint32_t");
    predefinePrimitiveType("uint64_t");

    FunctionInfo functionInfoAcos;
    functionInfoAcos.mIdentifier = "acos;float";
    functionInfoAcos.mType = "float";
    mContext->getRegistry().registerInfo(functionInfoAcos);
    
    FunctionInfo functionInfoSqrt;
    functionInfoSqrt.mIdentifier = "sqrt;float";
    functionInfoSqrt.mType = "float";
    mContext->getRegistry().registerInfo(functionInfoSqrt);

    mContext->popScope();
}