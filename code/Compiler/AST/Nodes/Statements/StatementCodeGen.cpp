#include "Compiler/AST/Nodes/Statements/Statement.hpp"
#include "Compiler/AST/Nodes/CodeBuilder.hpp"
#include "Compiler/AST/Registry.hpp"
#include "Compiler/AST/Nodes/ScopeBuilder.hpp"

// ******* STATEMENT *******
IMPL_CODEGEN(Statement)
{
    generateCodeChildren(builder);
    builder.addTokenType(TokensDefinitions::Semicolon);
    builder.newLine();
}

IMPL_CODEGEN(StatementBlock)
{
    builder.addTokenType(TokensDefinitions::LeftCurly);
    builder.newLine();
    builder.indent();
    generateCodeChildren(builder);
    builder.unindent();
    builder.newLine();
    builder.addTokenType(TokensDefinitions::RightCurly);
    builder.newLine();
}

IMPL_CODEGEN(StatementJump)
{
    builder.addToken(mTokenJump);
    if(mStatementExpression)
    {
        mStatementExpression->generateCode(builder);
    }
    builder.addTokenType(TokensDefinitions::Semicolon);
}

IMPL_CODEGEN(StatementBranch)
{
    builder.addTokenType(TokensDefinitions::If);
    builder.addTokenType(TokensDefinitions::LeftParen);
    mStatementExpression->generateCode(builder);
    builder.addTokenType(TokensDefinitions::RightParen);
    mStatementBody->generateCode(builder);
}

IMPL_CODEGEN(StatementTypeAlias)
{
    builder.addTokenType(TokensDefinitions::Using);
    builder.addToken(mTokenIdentifier);
    builder.addTokenType(TokensDefinitions::Equal);
    mStatementType->generateCode(builder);
    builder.addTokenType(TokensDefinitions::Semicolon); 
    builder.newLine();
}

// ******* CLASS DEFINITION *******
IMPL_CODEGEN(StatementClassDefinition)
{
    if(builder.mGenerateHeaderCode)
    {
        builder.addTokenType(TokensDefinitions::Class);
        builder.addToken(mTokenIdentifier);
        builder.addTokenType(TokensDefinitions::LeftCurly);
        builder.newLine();
        builder.indent();
        getScopeBuilder().pushScope(mTokenIdentifier.getLexeme());
        generateCodeChildren(builder);
        getScopeBuilder().popScope();
        builder.unindent();
        builder.newLine();
        builder.addTokenType(TokensDefinitions::RightCurly);
        builder.addTokenType(TokensDefinitions::Semicolon);
        builder.newLine();
    }
    else
    {
        TypeInfo info;
        info.mIdentifier = mTokenIdentifier;
        info.mScope = getScopeBuilder().getScope();
        if(getRegistry().getInfo(info))
        {
            builder.includeInSource(getRegistry().getInfo(info)->mPath);
        }

        getScopeBuilder().pushScope(mTokenIdentifier.getLexeme());
        generateCodeChildren(builder);
        getScopeBuilder().popScope();
    }
}

IMPL_CODEGEN(StatementClassVisibility)
{
    if(builder.mGenerateHeaderCode)
    {
        builder.addToken(mTokenVisibility);
        builder.addTokenType(TokensDefinitions::Colon);
        builder.newLine();
    }
}

IMPL_CODEGEN(StatementMemberVariableDefinition)
{
    if(builder.mGenerateHeaderCode)
    {
        mStatementVariableDefinition->generateCode(builder);
    }
}

IMPL_CODEGEN(StatementMemberFunctionDefinition)
{
    mStatementFunctionDefinition->generateCode(builder);
}

// ******* VARIABLE DEFINITION *******
IMPL_CODEGEN(StatementVariableDefinition)
{
    mStatementTypeQualifier->generateCode(builder);
    builder.addToken(mTokenIdentifier);
    if(mStatementExpression)
    {
        builder.addTokenType(TokensDefinitions::Equal);
        mStatementExpression->generateCode(builder);
    }

    builder.addTokenType(TokensDefinitions::Semicolon);
    builder.newLine();
}

IMPL_CODEGEN(StatementGlobalVariableDefinition)
{
    if(builder.mGenerateHeaderCode)
    {
        builder.addTokenType(TokensDefinitions::Static);
        generateCodeChildren(builder);
    }
}

// ******* FUNCTION DEFINITION *******
IMPL_CODEGEN(StatementFunctionBaseDefinition)
{
    if(!builder.mGenerateHeaderCode)
    {
        builder.addScope(getScopeBuilder());
    }

    builder.addToken(mTokenIdentifier);
    
    getScopeBuilder().pushScope(mTokenIdentifier.getLexeme());
    builder.addTokenType(TokensDefinitions::LeftParen);
    mStatementParametersList->generateCode(builder);
    builder.addTokenType(TokensDefinitions::RightParen);

    if(mStatementFunctionQualifier)
    {
        mStatementFunctionQualifier->generateCode(builder);
    }

    if(builder.mGenerateHeaderCode)
    {
        builder.addTokenType(TokensDefinitions::Semicolon);
    }
    else
    {
        builder.newLine();
        mStatementBody->generateCode(builder);
    }

    getScopeBuilder().popScope();

    builder.newLine();
}

IMPL_CODEGEN(StatementFunctionDefinition)
{
    mStatementTypeQualifierFunctionReturn->generateCode(builder);
    mStatementFunctionBaseDefinition->generateCode(builder);
}

IMPL_CODEGEN(StatementFunctionQualifier)
{
    builder.addToken(mTokenQualifier);
}

IMPL_CODEGEN(StatementParameterDefinition)
{
    mStatementTypeQualifierParameter->generateCode(builder);
    builder.addToken(mTokenIdentifier);
}

IMPL_CODEGEN(StatementParametersList)
{
    FOR_ARRAY(i, getChildren())
    {
        getChildren().at(i)->generateCode(builder);

        if(i < (getChildren().size() - 1))
        {
            builder.addTokenType(TokensDefinitions::Comma);
        }
    }
}

// ******* ASSIGNMENT *******
IMPL_CODEGEN(StatementAssignment)
{
    mStatementExpressionInvocation->generateCode(builder);
    if(mStatementExpression)
    {
        builder.addTokenType(TokensDefinitions::Equal);
        mStatementExpression->generateCode(builder);
    }

    builder.addTokenType(TokensDefinitions::Semicolon);
}

// ******* EXPRESSION *******
IMPL_CODEGEN(StatementExpressionInvocation)
{
    if(!mTokenThis.getIsNull())
    {
        builder.addTokenType(TokensDefinitions::This);
        if(getChildren().size() > 0)
        {
            builder.addTokenType(TokensDefinitions::Arrow);
        }
    }

    generateCodeChildren(builder);
}

IMPL_CODEGEN(StatementExpressionSimpleInvocation)
{
    if(mStatementExpressionFunctionInvocation)
    {
        mStatementExpressionFunctionInvocation->generateCode(builder);
    }
    else if(mStatementExpressionVariableInvocation)
    {
        mStatementExpressionVariableInvocation->generateCode(builder);
    }
}

IMPL_CODEGEN(StatementExpressionCompoundInvocation)
{
    mStatementExpressionSimpleInvocation->generateCode(builder);
    if(mStatementExpressionCompoundInvocation)
    {
        if(mTokenAccessOperator.is(TokensDefinitions::Dot))
        {
            bool isPointer = mStatementExpressionSimpleInvocation->isPointer();

            if(isPointer)
            {
               builder.addTokenType(TokensDefinitions::Arrow);
            }
            else
            {
               builder.addTokenType(TokensDefinitions::Dot);
            }
        }
        else
        {
            builder.addToken(mTokenAccessOperator);
        }

        mStatementExpressionCompoundInvocation->generateCode(builder);
    }
}

IMPL_CODEGEN(StatementExpressionVariableInvocation)
{
    builder.addToken(mTokenIdentifier);
}

IMPL_CODEGEN(StatementExpressionFunctionInvocation)
{
    builder.addToken(mTokenIdentifier);
    builder.addTokenType(TokensDefinitions::LeftParen);
    mStatementExpressionFunctionParametersList->generateCode(builder);
    builder.addTokenType(TokensDefinitions::RightParen);
}

IMPL_CODEGEN(StatementExpressionFunctionParametersList)
{
    FOR_ARRAY(i, getChildren())
    {
        getChildren().at(i)->generateCode(builder);

        if(i < (getChildren().size() - 1))
        {
            builder.addTokenType(TokensDefinitions::Comma);
        }
    }
}

IMPL_CODEGEN(StatementExpressionPrimary)
{
    if(mTokenExpression.getIsNull())
    {
        if(mStatementExpression)
        {
            builder.addTokenType(TokensDefinitions::LeftParen);
            mStatementExpression->generateCode(builder);
            builder.addTokenType(TokensDefinitions::RightParen);
        }
        else if(mStatementExpressionInvocation)
        {
            if(!mStatementExpressionInvocation->mTokenThis.getIsNull())
            {
                if(mStatementExpressionInvocation->getChildren().size() == 0)
                {
                    builder.addTokenType(TokensDefinitions::Asterisk);       
                }
            }

            mStatementExpressionInvocation->generateCode(builder);
        }
    }
    else
    {
        builder.addToken(mTokenExpression);
    }
}

IMPL_CODEGEN(StatementExpressionUnary)
{
    if(mStatementUnaryOperatorPre)
    {
        mStatementUnaryOperatorPre->generateCode(builder);
        mStatementExpressionPrimary->generateCode(builder);
    }
    else
    {
        mStatementExpressionPrimary->generateCode(builder);
        mStatementUnaryOperatorPost->generateCode(builder);
    }
}

IMPL_CODEGEN(StatementExpressionBinary)
{
    mStatementExpressionPrimary->generateCode(builder);
    mStatementBinaryOperator->generateCode(builder);
    mStatementExpression->generateCode(builder);
}

// ******* OPERATORS *******
IMPL_CODEGEN(StatementUnaryOperator)
{
    builder.addToken(mTokenOperator);
}

IMPL_CODEGEN(StatementBinaryOperator)
{
    builder.addToken(mTokenOperator);
}

// ******* TYPE *******
IMPL_CODEGEN(StatementType)
{
    builder.addToken(mTokenType);
    
    TypeInfo infoType;
    infoType.mIdentifier = mTokenType;
    if(getRegistry().getInfo(infoType))
    {
        builder.includeInHeader(getRegistry().getInfo(infoType)->mPath);
    }
}

IMPL_CODEGEN(StatementTypeQualifier)
{
    mStatementType->generateCode(builder);
}

IMPL_CODEGEN(StatementTypeQualifierParameter)
{
    mStatementTypeQualifier->generateCode(builder);
    
    TypeInfo info;
    info.mIdentifier = mStatementTypeQualifier->mStatementType->mTokenType;
    if(getRegistry().getInfo(info))
    {
        if(getRegistry().getInfo(info)->mIsStack)
        {
            builder.addTokenType(TokensDefinitions::Ampersand);
        }
        else
        {
            builder.addTokenType(TokensDefinitions::Asterisk);
        }
    }
}

IMPL_CODEGEN(StatementTypeQualifierFunctionReturn)
{
    mStatementTypeQualifier->generateCode(builder);
    
    TypeInfo info;
    info.mIdentifier = mStatementTypeQualifier->mStatementType->mTokenType;
    if(getRegistry().getInfo(info))
    {
        if(getRegistry().getInfo(info)->mIsStack)
        {
            builder.addTokenType(TokensDefinitions::Ampersand);
        }
        else
        {
            builder.addTokenType(TokensDefinitions::Asterisk);
        }
    }
}
