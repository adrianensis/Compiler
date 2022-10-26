#include "Compiler/AST/Nodes/Statements/Statement.hpp"
#include "Compiler/Parser/Parser.hpp"
#include "Compiler/AST/Nodes/CodeBuilder.hpp"
#include "Compiler/AST/Registry.hpp"

// ******* MODULE *******
IMPL_PARSE(StatementModule)
{
    expectRepeatNode<StatementDefinition>();

    const Token* token = mParser->getCurrentToken();
    if(!token->is(TokensDefinitions::End))
    {
        logError("Module doesn't compile!");
    }

    return true;
}

// ******* STATEMENT *******
IMPL_PARSE(Statement)
{
    if(expectNode<StatementBlock>()) { return true; }
    if(expectNode<StatementBranch>()) { return true; }
    if(expectNode<StatementJump>()) { return true; }
    if(expectNodeTerminated<StatementVariableDefinition>(TokensDefinitions::Semicolon)) { return true; }
    if(expectNodeTerminated<StatementAssignment>(TokensDefinitions::Semicolon)) { return true; }
    if(expectNodeTerminated<StatementExpression>(TokensDefinitions::Semicolon)) { return true; }
    if(expectNodeTerminated<StatementTypeAlias>(TokensDefinitions::Semicolon)) { return true; }
    return false;
}

IMPL_PARSE(StatementBlock)
{
    return expectRepeatNodeEnclosed<Statement>(TokensDefinitions::LeftCurly, TokensDefinitions::RightCurly);
}

IMPL_PARSE(StatementJump)
{
    if(expectToken(TokensDefinitions::Return, &mTokenJump))
    {
        if(mStatementExpression = expectNodeTerminated<StatementExpression>(TokensDefinitions::Semicolon))
        {
            return true;
        }
    }
    else if(expectToken(TokensDefinitions::Break, &mTokenJump))
    {
        if(expectTokenOrError(TokensDefinitions::Semicolon))
        {
            return true;
        }
    }
    else if(expectToken(TokensDefinitions::Continue, &mTokenJump))
    {
        if(expectTokenOrError(TokensDefinitions::Semicolon))
        {
            return true;
        }
    }

    return false;
}

IMPL_PARSE(StatementBranch)
{
    if(expectToken(TokensDefinitions::If))
    {
        if(mStatementExpression = expectNodeEnclosed<StatementExpression>(TokensDefinitions::LeftParen, TokensDefinitions::RightParen))
        {
            if(mStatementBody = expectNode<StatementBlock>())
            {
                return true;
            }
        }
    }

    return false;
}

IMPL_PARSE(StatementTypeAlias)
{
    if(expectToken(TokensDefinitions::Using))
    {
        if(expectToken(TokensDefinitions::Identifier, &mTokenIdentifier))
        {
            if(expectToken(TokensDefinitions::Equal))
            {
                if(mStatementType = expectNode<StatementType>())
                {
                    TypeInfo infoType;
                    infoType.mIdentifier = mTokenIdentifier;
                    infoType.mScope = getScopeBuilder().getScope();
                    infoType.mPath = mParser->mPath;
                    getRegistry().registerInfo(infoType);

                    return true;
                }
            }
        }
    }

    return false;
}

// ******* DEFINITION *******
IMPL_PARSE(StatementDefinition)
{
    if(expectNodeTerminated<StatementTypeAlias>(TokensDefinitions::Semicolon)) { return true; }
    if(expectNodeTerminated<StatementClassDefinition>(TokensDefinitions::Semicolon)) { return true; }
    if(expectNode<StatementEnumDefinition>()) { return true; }
    if(expectNode<StatementFunctionDefinition>()) { return true; }
    if(expectNodeTerminated<StatementGlobalVariableDefinition>(TokensDefinitions::Semicolon)) { return true; }
    return false;
}

// ******* CLASS DEFINITION *******
IMPL_PARSE(StatementClassDefinition)
{
    if(expectToken(TokensDefinitions::Stack, &mTokenStack))
    {
    }

    if(expectToken(TokensDefinitions::Class))
    {
        if(expectToken(TokensDefinitions::Identifier, &mTokenIdentifier))
        {
            getScopeBuilder().pushScope(mTokenIdentifier.getLexeme());
            if(expectRepeatNodeEnclosed<StatementClassDefinitionItem>(TokensDefinitions::LeftCurly, TokensDefinitions::RightCurly))
            {
                getScopeBuilder().popScope();

                TypeInfo info;
                info.mIdentifier = mTokenIdentifier;
                info.mDataType = DataType::TYPE_CLASS;
                info.mIsStack = !mTokenStack.getIsNull();
                info.mScope = getScopeBuilder().getScope();
                info.mPath = mParser->mPath;
                getRegistry().registerInfo(info);
                return true;
            }
            getScopeBuilder().popScope();
        }
    }

    return false;
}

IMPL_PARSE(StatementClassDefinitionItem)
{
    if(expectNodeTerminated<StatementClassDefinition>(TokensDefinitions::Semicolon)) { return true; }
    if(expectNodeTerminated<StatementClassVisibility>(TokensDefinitions::Colon)) { return true; }
    if(expectNodeTerminated<StatementMemberVariableDefinition>(TokensDefinitions::Semicolon)) { return true; }
    if(expectNode<StatementConstructorDefinition>()) { return true; }
    if(expectNode<StatementMemberFunctionDefinition>()) { return true; }
    return false;
}

IMPL_PARSE(StatementClassVisibility)
{
    if(expectToken(TokensDefinitions::Public, &mTokenVisibility)) { return true; }
    if(expectToken(TokensDefinitions::Protected, &mTokenVisibility)) { return true; }
    if(expectToken(TokensDefinitions::Private, &mTokenVisibility)) { return true; }
    return false;
}

IMPL_PARSE(StatementConstructorDefinition)
{
    if(expectNode<StatementFunctionBaseDefinition>()) { return true; }
    return false;
}

IMPL_PARSE(StatementMemberVariableDefinition)
{
    if(mStatementVariableDefinition = expectNode<StatementVariableDefinition>()) { return true; }
    return false;
}

IMPL_PARSE(StatementMemberFunctionDefinition)
{
    if(mStatementFunctionDefinition = expectNode<StatementFunctionDefinition>()) { return true; }
    return false;
}

// ******* ENUM DEFINITION *******
IMPL_PARSE(StatementEnumDefinition)
{

    return false;
}

// ******* VARIABLE DEFINITION *******
IMPL_PARSE(StatementVariableDefinition)
{
    if(mStatementTypeQualifier = expectNode<StatementTypeQualifier>())
    {
        if(expectToken(TokensDefinitions::Identifier, &mTokenIdentifier))
        {
            if(expectToken(TokensDefinitions::Equal))
            {
                if(mStatementExpression = expectNode<StatementExpression>())
                {
                    // NOTE: we might be parsing a method.
                    if(checkToken(TokensDefinitions::LeftParen))
                    {
                        return false;
                    }

                    VariableInfo infoVariable;
                    infoVariable.mIdentifier = mTokenIdentifier;
                    infoVariable.mType = mStatementTypeQualifier->mStatementType->mTokenType;
                    infoVariable.mScope = getScopeBuilder().getScope();
                    getRegistry().registerInfo(infoVariable);

                    return true;
                }
            }
        }
    }

    return false;
}

IMPL_PARSE(StatementGlobalVariableDefinition)
{
    if(expectNode<StatementVariableDefinition>()) { return true;  }
    return false;
}

// ******* FUNCTION DEFINITION *******
IMPL_PARSE(StatementFunctionDefinition)
{
    if(mStatementTypeQualifierFunctionReturn = expectNode<StatementTypeQualifierFunctionReturn>())
    {
        if(mStatementFunctionBaseDefinition = expectNode<StatementFunctionBaseDefinition>())
        {
            FunctionInfo infoFunction;
            infoFunction.mIdentifier = mStatementFunctionBaseDefinition->mTokenIdentifier;
            infoFunction.mType = mStatementTypeQualifierFunctionReturn->mStatementTypeQualifier->mStatementType->mTokenType;
            infoFunction.mScope = getScopeBuilder().getScope();
            getRegistry().registerInfo(infoFunction);

            return true;
        }
    }

    return false;
}

IMPL_PARSE(StatementFunctionBaseDefinition)
{
    if(expectToken(TokensDefinitions::Identifier, &mTokenIdentifier))
    {
        getScopeBuilder().pushScope(mTokenIdentifier.getLexeme());
        if(mStatementParametersList = expectNode<StatementParametersList>())
        {
            if(mStatementFunctionQualifier = expectNode<StatementFunctionQualifier>())
            {
                // ok
            }

            if(mStatementBody = expectNode<StatementBlock>())
            {
                getScopeBuilder().popScope();
                return true;
            }
        }
        getScopeBuilder().popScope();
    }

    return false;
}

IMPL_PARSE(StatementFunctionQualifier)
{
    if(expectToken(TokensDefinitions::Const, &mTokenQualifier)) { return true;  }
    return false;
}

IMPL_PARSE(StatementParameterDefinition)
{
    if(mStatementTypeQualifierParameter = expectNode<StatementTypeQualifierParameter>())
    {
        if(expectToken(TokensDefinitions::Identifier, &mTokenIdentifier))
        {
            VariableInfo infoVariable;
            infoVariable.mIdentifier = mTokenIdentifier;
            infoVariable.mType = mStatementTypeQualifierParameter->mStatementTypeQualifier->mStatementType->mTokenType;
            infoVariable.mScope = getScopeBuilder().getScope();
            getRegistry().registerInfo(infoVariable);
            return true;
        }
    }

    return false;
}

IMPL_PARSE(StatementParametersList)
{
    if(expectNodeListEnclosed<StatementParameterDefinition>(TokensDefinitions::Comma, TokensDefinitions::LeftParen, TokensDefinitions::RightParen)) { return true;  }
    return false;
}

// ******* ASSIGNMENT *******
IMPL_PARSE(StatementAssignment)
{
    if(mStatementExpressionInvocation = expectNode<StatementExpressionInvocation>())
    {
        if(expectToken(TokensDefinitions::Equal))
        {
            if(mStatementExpression = expectNode<StatementExpression>())
            {
                return true;
            }
        }
    }

    return false;
}

// ******* EXPRESSION *******
IMPL_PARSE(StatementExpression)
{
    if(mNodeExpression = expectNode<StatementExpressionBinary>()) { return true; }
    if(mNodeExpression = expectNode<StatementExpressionUnary>()) { return true; }
    if(mNodeExpression = expectNodeEnclosed<StatementExpression>(TokensDefinitions::LeftParen, TokensDefinitions::RightParen)) { return true; }
    if(mNodeExpression = expectNode<StatementExpressionPrimary>()) { return true; }
    return false;
}

IMPL_PARSE(StatementExpressionUnary)
{
    if(mStatementUnaryOperatorPre = expectNode<StatementUnaryOperator>())
    {
        if(mStatementExpressionPrimary = expectNode<StatementExpressionPrimary>())
        {
            return true;
        }
    }
    else if(mStatementExpressionPrimary = expectNode<StatementExpressionPrimary>())
    {
        if(mStatementUnaryOperatorPost = expectNode<StatementUnaryOperator>())
        {
            return true;
        }
    }

    return false;
}

IMPL_PARSE(StatementExpressionBinary)
{
    if(mStatementExpressionPrimary = expectNode<StatementExpressionPrimary>())
    {
        if(mStatementBinaryOperator = expectNode<StatementBinaryOperator>())
        {
            if(mStatementExpression = expectNode<StatementExpression>())
            {
                return true;
            }
        }
    }

    return false;
}

IMPL_PARSE(StatementExpressionPrimary)
{
    if(expectToken(TokensDefinitions::Number, &mTokenExpression)) { return true; }
    if(expectToken(TokensDefinitions::True, &mTokenExpression)) { return true; }
    if(expectToken(TokensDefinitions::False, &mTokenExpression)) { return true; }
    if(mStatementExpressionInvocation = expectNode<StatementExpressionInvocation>()) { return true; }
    if(mStatementExpression = expectNodeEnclosed<StatementExpression>(TokensDefinitions::LeftParen, TokensDefinitions::RightParen)) { return true; }
    return false;
}

IMPL_PARSE(StatementExpressionInvocation)
{
    if(expectToken(TokensDefinitions::This, &mTokenThis))
    {
        if(expectToken(TokensDefinitions::Dot))
        {
            if(mStatementExpressionCompoundInvocation = expectNode<StatementExpressionCompoundInvocation>())
            {
                return true;
            }
        }

        return true;
    }
    if(mStatementExpressionCompoundInvocation = expectNode<StatementExpressionCompoundInvocation>())
    {
        return true;
    }

    return false;
}

IMPL_PARSE(StatementExpressionSimpleInvocation)
{
    if(mStatementExpressionFunctionInvocation = expectNode<StatementExpressionFunctionInvocation>())
    {
        setRootTypeNode(mStatementExpressionFunctionInvocation);
        return true;
    }
    if(mStatementExpressionVariableInvocation = expectNode<StatementExpressionVariableInvocation>())
    {
        setRootTypeNode(mStatementExpressionVariableInvocation);
        return true;
    }
    return false;
}

IMPL_PARSE(StatementExpressionCompoundInvocation)
{
    if(mStatementExpressionSimpleInvocation = expectNode<StatementExpressionSimpleInvocation>())
    {
        if(expectToken(TokensDefinitions::Dot, &mTokenAccessOperator) || expectToken(TokensDefinitions::Scope, &mTokenAccessOperator))
        {
            if(mStatementExpressionCompoundInvocation = expectNode<StatementExpressionCompoundInvocation>())
            {
                setRootTypeNode(mStatementExpressionCompoundInvocation);
                return true;
            }
        }

        setRootTypeNode(mStatementExpressionSimpleInvocation);
        return true;
    }

    return false;
}

IMPL_PARSE(StatementExpressionVariableInvocation)
{
    if(expectToken(TokensDefinitions::Identifier, &mTokenIdentifier))
    {
        VariableInfo infoVariable;
        infoVariable.mIdentifier = mTokenIdentifier;
        infoVariable.mScope = getScopeBuilder().getScope();
        setRootTypeInfo(infoVariable);
        return true;
    }
    return false;
}

IMPL_PARSE(StatementExpressionFunctionInvocation)
{
    if(expectToken(TokensDefinitions::Identifier, &mTokenIdentifier))
    {
        if(mStatementExpressionFunctionParametersList = expectNode<StatementExpressionFunctionParametersList>())
        {
            //setRootTypeInfo(mTokenIdentifier);
            return true;
        }
    }

    return false;
}

IMPL_PARSE(StatementExpressionFunctionParametersList)
{
    if(expectNodeListEnclosed<StatementExpression>(TokensDefinitions::Comma, TokensDefinitions::LeftParen, TokensDefinitions::RightParen)) { return true; }
    return false;
}

// ******* OPERATORS *******
IMPL_PARSE(StatementUnaryOperator)
{
    if(expectToken(TokensDefinitions::Increment, &mTokenOperator)) { return true; }
    if(expectToken(TokensDefinitions::Decrement, &mTokenOperator)) { return true; }
    if(expectToken(TokensDefinitions::Plus, &mTokenOperator)) { return true; }
    if(expectToken(TokensDefinitions::Minus, &mTokenOperator)) { return true; }
    if(expectToken(TokensDefinitions::Exclamation, &mTokenOperator)) { return true; }
    if(expectToken(TokensDefinitions::Tilde, &mTokenOperator)) { return true; }
    return false;
}

IMPL_PARSE(StatementBinaryOperator)
{
    // multiplicative
    if(expectToken(TokensDefinitions::Asterisk, &mTokenOperator)) { return true; }
    if(expectToken(TokensDefinitions::Slash, &mTokenOperator)) { return true; }
    if(expectToken(TokensDefinitions::Percent, &mTokenOperator)) { return true; }
    // additive
    if(expectToken(TokensDefinitions::Plus, &mTokenOperator)) { return true; }
    if(expectToken(TokensDefinitions::Minus, &mTokenOperator)) { return true; }
    if(expectToken(TokensDefinitions::PlusEqual, &mTokenOperator)) { return true; }
    if(expectToken(TokensDefinitions::MinusEqual, &mTokenOperator)) { return true; }
    // relational
    if(expectToken(TokensDefinitions::LessThan, &mTokenOperator)) { return true; }
    if(expectToken(TokensDefinitions::GreaterThan, &mTokenOperator)) { return true; }
    if(expectToken(TokensDefinitions::LessThanEqual, &mTokenOperator)) { return true; }
    if(expectToken(TokensDefinitions::GreaterThanEqual, &mTokenOperator)) { return true; }
    // equality
    if(expectToken(TokensDefinitions::EqualTo, &mTokenOperator)) { return true; }
    if(expectToken(TokensDefinitions::NotEqualTo, &mTokenOperator)) { return true; }
    // bitwise
    if(expectToken(TokensDefinitions::Ampersand, &mTokenOperator)) { return true; }
    if(expectToken(TokensDefinitions::Caret, &mTokenOperator)) { return true; }
    if(expectToken(TokensDefinitions::Pipe, &mTokenOperator)) { return true; }
    // logic
    if(expectToken(TokensDefinitions::And, &mTokenOperator)) { return true; }
    if(expectToken(TokensDefinitions::Or, &mTokenOperator)) { return true; }
    return false;
}

// ******* TYPE *******
IMPL_PARSE(StatementType)
{
    if(expectToken(TokensDefinitions::Int, &mTokenType)) { return true; }
    if(expectToken(TokensDefinitions::Float, &mTokenType)) { return true; }
    if(expectToken(TokensDefinitions::Void, &mTokenType)) { return true; }
    if(expectToken(TokensDefinitions::Identifier, &mTokenType)) { return true; }
    return false;
}

IMPL_PARSE(StatementTypeQualifier)
{
    if(mStatementType = expectNode<StatementType>()) { return true; }
    return false;
}

IMPL_PARSE(StatementTypeQualifierParameter)
{
    if(mStatementTypeQualifier = expectNode<StatementTypeQualifier>()) { return true; }
    return false;
}

IMPL_PARSE(StatementTypeQualifierFunctionReturn)
{
    if(mStatementTypeQualifier = expectNode<StatementTypeQualifier>()) { return true; }
    return false;
}