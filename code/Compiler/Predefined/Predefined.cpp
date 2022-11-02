#include "Compiler/Predefined/Predefined.hpp"
#include "Compiler/AST/Nodes/Statements/Statement.hpp"
#include "Compiler/AST/Nodes/CodeBuilder.hpp"

void Predefined::predefine()
{
    TypeInfo typeInfoFloat;
    typeInfoFloat.mIdentifier = TokensDefinitions::Float.getValue();
    typeInfoFloat.mDataType = DataType::TYPE_PRIMITIVE;
    mContext->getRegistry().registerInfo(typeInfoFloat);
    
    TypeInfo typeInfoInt;
    typeInfoInt.mIdentifier = TokensDefinitions::Int.getValue();
    typeInfoInt.mDataType = DataType::TYPE_PRIMITIVE;
    mContext->getRegistry().registerInfo(typeInfoInt);
    
    VariableInfo variableInfoTrue;
    variableInfoTrue.mIdentifier = TokensDefinitions::True.getValue();
    variableInfoTrue.mType = TokensDefinitions::Bool.getValue();
    mContext->getRegistry().registerInfo(variableInfoTrue);
    
    VariableInfo variableInfoFalse;
    variableInfoFalse.mIdentifier = TokensDefinitions::False.getValue();
    variableInfoFalse.mType = TokensDefinitions::Bool.getValue();
    mContext->getRegistry().registerInfo(variableInfoFalse);
    
    FunctionInfo functionInfoAcos;
    functionInfoAcos.mIdentifier = "acosfloat";
    functionInfoAcos.mType = "float";
    mContext->getRegistry().registerInfo(functionInfoAcos);
}