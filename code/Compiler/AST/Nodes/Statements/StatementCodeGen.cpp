#include "Compiler/AST/Nodes/Statements/Statement.hpp"
#include "Compiler/AST/Nodes/CodeBuilder.hpp"
#include "Compiler/AST/Registry.hpp"

IMPL_CODEGEN(Statement)
{
    generateCodeChildren(builder);
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

IMPL_CODEGEN(StatementClassDefinition)
{
    if(builder.mGenerateHeaderCode)
    {
        builder.addTokenType(TokensDefinitions::Class);
        builder.addToken(mTokenIdentifier);
        builder.addTokenType(TokensDefinitions::LeftCurly);
        builder.newLine();
        builder.indent();
        builder.getScopeBuilder().pushScope(mTokenIdentifier.getLexeme());
        generateCodeChildren(builder);
        builder.getScopeBuilder().popScope();
        builder.unindent();
        builder.newLine();
        builder.addTokenType(TokensDefinitions::RightCurly);
        builder.addTokenType(TokensDefinitions::Semicolon);
        builder.newLine();
    }
    else
    {
        if(getRegistry().isClass(builder.getScopeBuilder().getScope(), mTokenIdentifier.getLexeme()))
        {
            builder.includeInSource(getRegistry().getClass(builder.getScopeBuilder().getScope(), mTokenIdentifier.getLexeme()).mPath);
        }

        builder.getScopeBuilder().pushScope(mTokenIdentifier.getLexeme());
        generateCodeChildren(builder);
        builder.getScopeBuilder().popScope();
    }
}

IMPL_CODEGEN(StatementClassVisibility)
{
    if(builder.mGenerateHeaderCode)
    {
        builder.addToken(mTokenVisibility);
        builder.addTokenType(TokensDefinitions::Colon);
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

IMPL_CODEGEN(StatementVariableDefinition)
{
    mStatementType->generateCode(builder);
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
        builder.newLine();
    }
}

IMPL_CODEGEN(StatementFunctionBaseDefinition)
{

    if(!builder.mGenerateHeaderCode)
    {
        builder.addScope();
    }

    builder.addToken(mTokenIdentifier);
    
    builder.getScopeBuilder().pushScope(mTokenIdentifier.getLexeme());
    builder.addTokenType(TokensDefinitions::LeftParen);
    mStatementParametersList->generateCode(builder);
    builder.addTokenType(TokensDefinitions::RightParen);

    if(builder.mGenerateHeaderCode)
    {
        builder.addTokenType(TokensDefinitions::Semicolon);
    }
    else
    {
        mStatementBody->generateCode(builder);
    }

    builder.getScopeBuilder().popScope();

    builder.newLine();
}

IMPL_CODEGEN(StatementFunctionDefinition)
{
    mStatementType->generateCode(builder);
    mStatementFunctionBaseDefinition->generateCode(builder);
}

IMPL_CODEGEN(StatementParameterDefinition)
{
    mStatementType->generateCode(builder);
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

IMPL_CODEGEN(StatementAssignment)
{
    mStatementVariableInvocation->generateCode(builder);
    if(mStatementExpression)
    {
        builder.addTokenType(TokensDefinitions::Equal);
        mStatementExpression->generateCode(builder);
    }

    builder.addTokenType(TokensDefinitions::Semicolon);
}

IMPL_CODEGEN(StatementVariableInvocation)
{
    builder.addToken(mTokenIdentifier);
    if(mStatementVariableInvocation)
    {
        bool isPointer = false; 
        if(getRegistry().isVariable(builder.getScopeBuilder().getScope(), mTokenIdentifier.getLexeme()))
        {
            if(getRegistry().getVariable(builder.getScopeBuilder().getScope(), mTokenIdentifier.getLexeme()).mType.is(TokensDefinitions::Identifier))
            {
                isPointer = true;
            }
        }
        else
        {
            if(mTokenIdentifier.is(TokensDefinitions::This))
            {
                isPointer = true;
            }
        }
        
        if(isPointer)
        {
            builder.addTokenType(TokensDefinitions::Arrow);
        }
        else
        {
            builder.addTokenType(TokensDefinitions::Dot);
        }

        mStatementVariableInvocation->generateCode(builder);
    }
}

IMPL_CODEGEN(StatementExpressionPrimary)
{
    if(mTokenExpression.getIsNull())
    {
        generateCodeChildren(builder);
    }
    else
    {
        builder.addToken(mTokenExpression);
    }
}

IMPL_CODEGEN(StatementType)
{
    builder.addToken(mTokenType);
    
    if(mTokenType.is(TokensDefinitions::Identifier))
    {
        builder.addTokenType(TokensDefinitions::Asterisk);
    }
    
    if(getRegistry().isClass("", mTokenType.getLexeme()))
    {
        builder.includeInHeader(getRegistry().getClass("", mTokenType.getLexeme()).mPath);
    }
}
