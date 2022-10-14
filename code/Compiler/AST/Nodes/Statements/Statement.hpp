#ifndef STATEMENT_HPP
#define STATEMENT_HPP

#include "Core/Module.hpp"
#include "Compiler/AST/Nodes/Node.hpp"
#include "Compiler/Lexer/Token.hpp"

// ******* MODULE *******
DECL_NODE(StatementModule)
    public: std::string mPath;
END_NODE()

// ******* STATEMENT *******
DECL_NODE(Statement)
    DECL_CODEGEN();
END_NODE()

DECL_NODE(StatementBlock)
    DECL_CODEGEN();
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

// ******* DEFINITION *******
DECL_NODE(StatementDefinition)
END_NODE()

// ******* CLASS DEFINITION *******
DECL_NODE(StatementClassDefinition)
    DECL_CODEGEN()
    DECL_TOKEN(mTokenIdentifier)
END_NODE()

DECL_NODE(StatementClassDefinitionItem)
END_NODE()

DECL_NODE(StatementConstructorDefinition)
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
    DECL_CHILD(StatementType, mStatementType);
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
    DECL_CHILD(StatementBlock, mStatementBody);
END_NODE()

DECL_NODE(StatementFunctionDefinition)
    DECL_CODEGEN()
    DECL_CHILD(StatementType, mStatementType);
    DECL_CHILD(StatementFunctionBaseDefinition, mStatementFunctionBaseDefinition);
END_NODE()

DECL_NODE(StatementParameterDefinition)
    DECL_CODEGEN()
    DECL_TOKEN(mTokenIdentifier);
    DECL_CHILD(StatementType, mStatementType);
END_NODE()

DECL_NODE(StatementParametersList)
    DECL_CODEGEN()
END_NODE()

// ******* ASSIGNMENT *******
DECL_NODE(StatementAssignment)
    DECL_CODEGEN()
    DECL_CHILD(StatementExpression, mStatementExpression);
    DECL_TOKEN(mTokenIdentifier);
END_NODE()

// ******* EXPRESSION *******
DECL_NODE(StatementExpression)
END_NODE()

DECL_NODE(StatementExpressionPrimary)
    DECL_CODEGEN()
    DECL_TOKEN(mTokenExpression);
END_NODE()

// ******* TYPE *******
DECL_NODE(StatementType)
    DECL_CODEGEN()
    DECL_TOKEN(mTokenType);
END_NODE()

#endif