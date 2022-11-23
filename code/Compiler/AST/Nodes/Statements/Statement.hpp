#ifndef STATEMENT_HPP
#define STATEMENT_HPP

#include "Core/Module.hpp"
#include "Compiler/AST/Nodes/Node.hpp"
#include "Compiler/Lexer/Token.hpp"
#include "Compiler/AST/FunctionSignatureBuilder.hpp"

// ******* MODULE *******
DECL_NODE(StatementModule)
    DECL_CHILD(StatementDeclareModule, mStatementDeclareModule);
    DECL_CHILD(StatementImportModule, mStatementImportModule);
public:
    std::string mPath;
    bool mParsed = false;
    std::vector<std::string> mDependencies;
    void parseHeader();
END_NODE()

DECL_NODE(StatementDeclareModule)
    DECL_TOKEN(mTokenIdentifier);
END_NODE()

DECL_NODE(StatementImportModule)
    DECL_TOKEN(mTokenIdentifier);
END_NODE()

// ******* STATEMENT *******
DECL_NODE(Statement)
    DECL_CODEGEN()
END_NODE()

DECL_NODE(StatementBlock)
    DECL_CODEGEN()
END_NODE()

DECL_NODE(StatementJump)
    DECL_CODEGEN()
    DECL_TOKEN(mTokenJump)
    DECL_CHILD(StatementExpression, mStatementExpression);
END_NODE()

DECL_NODE(StatementBranch)
    DECL_CODEGEN()
    DECL_CHILD(StatementExpression, mStatementExpression);
    DECL_CHILD(StatementBlock, mStatementBody);
END_NODE()

DECL_NODE(StatementTypeAlias)
    DECL_CODEGEN()
    DECL_TOKEN(mTokenIdentifier);
    DECL_CHILD(StatementType, mStatementType);
END_NODE()

// ******* DEFINITION *******
DECL_NODE(StatementDefinition)
END_NODE()

// ******* CLASS DEFINITION *******
DECL_NODE(StatementClassDefinition)
    DECL_CODEGEN()
    DECL_TOKEN(mTokenStack)
    DECL_TOKEN(mTokenIdentifier)
END_NODE()

DECL_NODE(StatementClassDefinitionItem)
END_NODE()

DECL_NODE(StatementClassVisibility)
    DECL_CODEGEN()
    DECL_TOKEN(mTokenVisibility)
END_NODE()

DECL_NODE(StatementConstructorDefinition)
    DECL_CHILD(StatementFunctionBaseDefinition, mStatementFunctionBaseDefinition);
END_NODE()

DECL_NODE(StatementMemberVariableDefinition)
    DECL_CODEGEN()
    DECL_CHILD(StatementVariableDefinition, mStatementVariableDefinition);
END_NODE()

DECL_NODE(StatementMemberFunctionDefinition)
    DECL_CODEGEN()
    DECL_CHILD(StatementFunctionDefinition, mStatementFunctionDefinition);
END_NODE()

// ******* ENUM DEFINITION *******
DECL_NODE(StatementEnumDefinition)
END_NODE()

// ******* VARIABLE DEFINITION *******
DECL_NODE(StatementVariableDefinition)
    DECL_CODEGEN()
    DECL_TOKEN(mTokenIdentifier);
    DECL_CHILD(StatementExpression, mStatementExpression);
    DECL_CHILD(StatementTypeQualifierVariable, mStatementTypeQualifierVariable);
END_NODE()

DECL_NODE(StatementGlobalVariableDefinition)
    DECL_CODEGEN()
    DECL_CHILD(StatementVariableDefinition, mStatementVariableDefinition);
END_NODE()

// ******* FUNCTION DEFINITION *******
DECL_NODE(StatementFunctionBaseDefinition)
    DECL_CODEGEN()
    DECL_TOKEN(mTokenIdentifier);
    DECL_CHILD(StatementParametersList, mStatementParametersList);
    DECL_CHILD(StatementFunctionQualifier, mStatementFunctionQualifier);
    DECL_CHILD(StatementBlock, mStatementBody);
public:
    FunctionSignatureBuilder mFunctionSignature;
END_NODE()

DECL_NODE(StatementFunctionDefinition)
    DECL_CODEGEN()
    DECL_CHILD(StatementTypeQualifierFunctionReturn, mStatementTypeQualifierFunctionReturn);
    DECL_CHILD(StatementFunctionBaseDefinition, mStatementFunctionBaseDefinition);
END_NODE()

DECL_NODE(StatementFunctionQualifier)
    DECL_CODEGEN()
    DECL_TOKEN(mTokenQualifier);
END_NODE()

DECL_NODE(StatementParameterDefinition)
    DECL_CODEGEN()
    DECL_TOKEN(mTokenIdentifier);
    DECL_CHILD(StatementTypeQualifierVariable, mStatementTypeQualifierVariable);
    VariableInfo mParameterInfo;
END_NODE()

DECL_NODE(StatementParametersList)
    DECL_CODEGEN()
END_NODE()

// ******* ASSIGNMENT *******
DECL_NODE(StatementAssignment)
    DECL_CODEGEN()
    DECL_CHILD(StatementExpression, mStatementExpression);
    DECL_CHILD(StatementExpressionInvocation, mStatementExpressionInvocation);
END_NODE()

// ******* EXPRESSION *******
DECL_EXPRESSION_NODE(StatementExpression)
    DECL_CODEGEN()
    DECL_CHILD(NodeExpression, mNodeExpression);
    DECL_CHILD(NodeExpression, mNodeEnclosedExpression);
END_NODE()

DECL_EXPRESSION_NODE(StatementExpressionPrimary)
    DECL_CODEGEN()
    DECL_TOKEN(mTokenExpression);
    DECL_CHILD(StatementExpressionInvocation, mStatementExpressionInvocation);
END_NODE()

DECL_EXPRESSION_NODE(StatementExpressionUnary)
    DECL_CODEGEN()
    DECL_CHILD(StatementUnaryOperator, mStatementUnaryOperatorPre);
    DECL_CHILD(StatementUnaryOperator, mStatementUnaryOperatorPost);
    DECL_CHILD(StatementExpressionPrimary, mStatementExpressionPrimary);
END_NODE()

DECL_EXPRESSION_NODE(StatementExpressionBinary)
    DECL_CODEGEN()
    DECL_CHILD(StatementBinaryOperator, mStatementBinaryOperator);
    DECL_CHILD(StatementExpressionPrimary, mStatementExpressionPrimary);
    DECL_CHILD(StatementExpression, mStatementExpression);
END_NODE()

DECL_EXPRESSION_NODE(StatementExpressionInvocation)
    DECL_CODEGEN()
    DECL_TOKEN(mTokenThis);
    DECL_CHILD(StatementExpressionCompoundInvocation, mStatementExpressionCompoundInvocation);
END_NODE()

DECL_EXPRESSION_NODE(StatementExpressionSimpleInvocation)
    DECL_CODEGEN()
    DECL_CHILD(StatementExpressionVariableInvocation, mStatementExpressionVariableInvocation);
    DECL_CHILD(StatementExpressionFunctionInvocation, mStatementExpressionFunctionInvocation);
END_NODE()

DECL_EXPRESSION_NODE(StatementExpressionCompoundInvocation)
    DECL_CODEGEN()
    DECL_CHILD(StatementExpressionSimpleInvocation, mStatementExpressionSimpleInvocation);
    DECL_CHILD(StatementExpressionCompoundInvocation, mStatementExpressionCompoundInvocation);
    DECL_CHILD(StatementScope, mStatementScope);
END_NODE()

DECL_EXPRESSION_NODE(StatementExpressionVariableInvocation)
    DECL_CODEGEN()
    DECL_TOKEN(mTokenIdentifier);
END_NODE()

DECL_EXPRESSION_NODE(StatementExpressionFunctionInvocation)
    DECL_CODEGEN()
    DECL_TOKEN(mTokenIdentifier);
    DECL_CHILD(StatementExpressionFunctionParametersList, mStatementExpressionFunctionParametersList);
END_NODE()

DECL_EXPRESSION_NODE(StatementExpressionFunctionParametersList)
    DECL_CODEGEN()
END_NODE()

// ******* SCOPE *******
DECL_NODE(StatementScope)
    DECL_CODEGEN()
    DECL_TOKEN(mTokenScope);
    DECL_CHILD(StatementScope, mStatementScope);
public:
    std::vector<std::string> mScopeVector;
END_NODE()

// ******* OPERATORS *******
DECL_NODE(StatementUnaryOperator)
    DECL_CODEGEN()
    DECL_TOKEN(mTokenOperator);
END_NODE()

DECL_NODE(StatementBinaryOperator)
    DECL_CODEGEN()
    DECL_TOKEN(mTokenOperator);
END_NODE()

// ******* TYPE *******
DECL_NODE(StatementType)
    DECL_CODEGEN()
    DECL_TOKEN(mTokenType);
END_NODE()

DECL_NODE(StatementTypeQualifier)
    DECL_CODEGEN()
    DECL_TOKEN(mTokenTypeStaticQualifier);
    DECL_TOKEN(mTokenTypeConstQualifier);
    DECL_CHILD(StatementType, mStatementType);
END_NODE()

DECL_NODE(StatementTypeQualifierVariable)
    DECL_CODEGEN()
    DECL_CHILD(StatementTypeQualifier, mStatementTypeQualifier);
END_NODE()

DECL_NODE(StatementTypeQualifierFunctionReturn)
    DECL_CODEGEN()
    DECL_CHILD(StatementTypeQualifier, mStatementTypeQualifier);
END_NODE()

#endif