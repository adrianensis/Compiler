#include "Compiler/Predefined/Predefined.hpp"
#include "Compiler/AST/Nodes/Statements/Statement.hpp"
#include "Compiler/AST/Nodes/CodeBuilder.hpp"

void Predefined::predefine()
{
    TypeInfo typeInfoFloat;
    typeInfoFloat.mIdentifier = TokensDefinitions::Float.getValue();
    typeInfoFloat.mDataType = DataType::TYPE_PRIMITIVE;
    typeInfoFloat.mIsStack = true;
    mContext->getRegistry().registerInfo(typeInfoFloat);
    
    TypeInfo typeInfoInt;
    typeInfoInt.mIdentifier = TokensDefinitions::Int.getValue();
    typeInfoInt.mDataType = DataType::TYPE_PRIMITIVE;
    typeInfoInt.mIsStack = true;
    mContext->getRegistry().registerInfo(typeInfoInt);
    
    VariableInfo variableInfoTrue;
    variableInfoTrue.mIdentifier = TokensDefinitions::True.getValue();
    variableInfoTrue.mType = TokensDefinitions::Bool.getValue();
    mContext->getRegistry().registerInfo(variableInfoTrue);
    
    VariableInfo variableInfoFalse;
    variableInfoFalse.mIdentifier = TokensDefinitions::False.getValue();
    variableInfoFalse.mType = TokensDefinitions::Bool.getValue();
    mContext->getRegistry().registerInfo(variableInfoFalse);
    
    mContext->pushScope("std");
    
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