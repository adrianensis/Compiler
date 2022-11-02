#include "Compiler/AST/Nodes/Statements/Statement.hpp"
#include "Compiler/AST/Nodes/CodeBuilder.hpp"
#include "Compiler/AST/Context.hpp"

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
    if(builder.mGenerateHeaderCode)
    {
        builder.addTokenType(TokensDefinitions::Using);
        builder.addToken(mTokenIdentifier);
        builder.addTokenType(TokensDefinitions::Equal);
        mStatementType->generateCode(builder);
        builder.addTokenType(TokensDefinitions::Semicolon); 
        builder.newLine();
    }
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
        getContext().pushScope(mTokenIdentifier.getLexeme());
        getContext().pushClassScope(mTokenIdentifier.getLexeme());
        generateCodeChildren(builder);
        getContext().popClassScope();
        getContext().popScope();
        builder.unindent();
        builder.newLine();
        builder.addTokenType(TokensDefinitions::RightCurly);
        builder.addTokenType(TokensDefinitions::Semicolon);
        builder.newLine();
    }
    else
    {
        const TypeInfo* info = getContext().findTypeInfo(mTokenIdentifier.getLexeme());
        if(info && !info->mPath.empty())
        {
            builder.includeInSource(info->mPath);
        }

        getContext().pushScope(mTokenIdentifier.getLexeme());
        getContext().pushClassScope(mTokenIdentifier.getLexeme());
        generateCodeChildren(builder);
        getContext().popClassScope();
        getContext().popScope();
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
    mStatementTypeQualifierVariable->generateCode(builder);
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
        builder.addScope(getContext());
    }

    builder.addToken(mTokenIdentifier);
    
    getContext().pushScope(mFunctionSignature);
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

    getContext().popScope();

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
    mStatementTypeQualifierVariable->generateCode(builder);
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
IMPL_CODEGEN(StatementExpression)
{
    if(mNodeEnclosedExpression)
    {
        builder.addTokenType(TokensDefinitions::LeftParen);
        mNodeEnclosedExpression->generateCode(builder);
        builder.addTokenType(TokensDefinitions::RightParen);
    }
    else
    {
        mNodeExpression->generateCode(builder);
    }
}

IMPL_CODEGEN(StatementExpressionInvocation)
{
    if(!mTokenThis.getIsNull())
    {
        if(mStatementExpressionCompoundInvocation)
        {
            builder.addTokenType(TokensDefinitions::This);
            builder.addTokenType(TokensDefinitions::Arrow);
        }
        else
        {
            const TypeInfo* info = getContext().findTypedDataTypeInfo(getRootTypeIdentifier());
            if(info)
            {
                if(info->mIsStack)
                {
                    builder.addTokenType(TokensDefinitions::Asterisk);
                }
                builder.addTokenType(TokensDefinitions::This);
            }
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
    bool isPointer = false;
    const TypedDataInfo* typedDataInfo = nullptr;
    typedDataInfo = getContext().findTypedData(mStatementExpressionSimpleInvocation->getRootTypeIdentifier());

    if(mStatementScope)
    {
        mStatementScope->generateCode(builder);

        ScopeBuilder scopeBuilder;
        FOR_LIST(it, mStatementScope->mScopeVector)
        {
            scopeBuilder.pushScope(*it);
        }

        typedDataInfo = getContext().getRegistryFromScope(scopeBuilder.getScope()).getInfo<TypedDataInfo>(mStatementExpressionSimpleInvocation->getRootTypeIdentifier());
    }

    if(typedDataInfo)
    {
        const TypeInfo* typeInfo = getContext().findTypeInfo(typedDataInfo->mType);
        if(typeInfo)
        {
            if(!typeInfo->mIsStack)
            {
                isPointer = true;
            }
        }
    }

    mStatementExpressionSimpleInvocation->generateCode(builder);
    if(mStatementExpressionCompoundInvocation)
    {
        if(isPointer)
        {
            builder.addTokenType(TokensDefinitions::Arrow);
        }
        else
        {
            builder.addTokenType(TokensDefinitions::Dot);
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
        mStatementExpressionInvocation->generateCode(builder);
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

// ******* SCOPE *******
IMPL_CODEGEN(StatementScope)
{
    builder.addToken(mTokenScope);
    builder.addTokenType(TokensDefinitions::Scope);

    if(mStatementScope)
    {
        mStatementScope->generateCode(builder);
    }
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

    const TypeInfo* info = getContext().findTypeInfo(mTokenType.getLexeme());
    bool includeHeader = true;
    if(builder.mGenerateHeaderCode)
    {
        if(info && info->mDataType == DataType::TYPE_CLASS)
        {
            includeHeader = false;
        }
    }

    if(info && !info->mPath.empty())
    {
        if(includeHeader)
        {
            builder.includeInHeader(info->mPath);
        }
        else
        {
            builder.forwardInHeader(info->mIdentifier);
        }
    }
    
}

IMPL_CODEGEN(StatementTypeQualifier)
{
    builder.addToken(mTokenTypeQualifier);
    mStatementType->generateCode(builder);
}

IMPL_CODEGEN(StatementTypeQualifierVariable)
{
    mStatementTypeQualifier->generateCode(builder);
    
    const TypeInfo* info = getContext().findTypeInfo(mStatementTypeQualifier->mStatementType->mTokenType.getLexeme());
    if(info)
    {
        if(info->mDataType == DataType::TYPE_CLASS)
        {
            if(info->mIsStack)
            {
                builder.addTokenType(TokensDefinitions::Ampersand);
            }
            else
            {
                builder.addTokenType(TokensDefinitions::Asterisk);
            }
        }
    }
}

IMPL_CODEGEN(StatementTypeQualifierFunctionReturn)
{
    mStatementTypeQualifier->generateCode(builder);
    
    const TypeInfo* info = getContext().findTypeInfo(mStatementTypeQualifier->mStatementType->mTokenType.getLexeme());

    if(info)
    {
        if(info->mDataType == DataType::TYPE_CLASS)
        {
            if(info->mIsStack)
            {
                builder.addTokenType(TokensDefinitions::Ampersand);
            }
            else
            {
                builder.addTokenType(TokensDefinitions::Asterisk);
            }
        }
    }
}
