#include "Compiler/AST/Nodes/Statements/Statement.hpp"
#include "Compiler/Parser/Parser.hpp"
#include "Compiler/AST/Nodes/CodeBuilder.hpp"
#include "Compiler/AST/Registry.hpp"

IMPL_PARSE(StatementModule)
{
    expectRepeatNode<StatementDefinition>();

    const Token* token = mParser->getCurrentToken();
    if(!token->is(TokensDefinitions::End))
    {
        logError("");
    }

    return true;
}

IMPL_PARSE(Statement)
{
    if(expectNode<StatementBlock>())
    {
        return true;
    }
    else if(expectNode<StatementBranch>())
    {
        return true;
    }
    else if(expectNode<StatementJump>())
    {
        return true;
    }
    else if(expectNode<StatementVariableDefinition>())
    {
        if(expectTokenOrError(TokensDefinitions::Semicolon))
        {
            return true;
        }
    }
    else if(expectNode<StatementAssignment>())
    {
        if(expectTokenOrError(TokensDefinitions::Semicolon))
        {
            return true;
        }
    }
    else if(expectNode<StatementExpressionFunctionInvocation>())
    {
        if(expectTokenOrError(TokensDefinitions::Semicolon))
        {
            return true;
        }
    }

    return false;
}

IMPL_PARSE(StatementBlock)
{
    return expectRepeatNodeEnclosed<Statement>(TokensDefinitions::LeftCurly, TokensDefinitions::RightCurly);
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

IMPL_PARSE(StatementJump)
{
    bool tokenFound = false;
    if(expectToken(TokensDefinitions::Return, &mTokenJump))
    {
        mStatementExpression = expectNode<StatementExpression>();

        tokenFound = true;
    }
    else if(expectToken(TokensDefinitions::Break, &mTokenJump))
    {
        tokenFound = true;
    }
    else if(expectToken(TokensDefinitions::Continue, &mTokenJump))
    {
        tokenFound = true;
    }

    if(tokenFound)
    {
        if(expectTokenOrError(TokensDefinitions::Semicolon))
        {
            return true;
        }
    }

    return false;
}

IMPL_PARSE(StatementDefinition)
{
    if(expectNode<StatementClassDefinition>())
    {
        return true;
    }
    else if(expectNode<StatementEnumDefinition>())
    {
        return true;
    }
    else if(expectNode<StatementFunctionDefinition>())
    {
        return true;
    }
    else if(expectNode<StatementGlobalVariableDefinition>())
    {
        return true;
    }

    return false;
}

IMPL_PARSE(StatementClassDefinition)
{
    if(expectToken(TokensDefinitions::Class))
    {
        if(expectToken(TokensDefinitions::Identifier, &mTokenIdentifier))
        {
            getScopeBuilder().pushScope(mTokenIdentifier.getLexeme());
            if(expectRepeatNodeEnclosed<StatementClassDefinitionItem>(TokensDefinitions::LeftCurly, TokensDefinitions::RightCurly))
            {
                if(expectTokenOrError(TokensDefinitions::Semicolon))
                {
                    getScopeBuilder().popScope();

                    if(!getRegistry().isClass(getScopeBuilder().getScope(), mTokenIdentifier.getLexeme()))
                    {
                        ClassInfo info;
                        info.mIdentifier = mTokenIdentifier;
                        info.mPath = mParser->mPath;
                        info.mScope = getScopeBuilder().getScope();
                        getRegistry().registerClass(info);
                        return true;
                    }
                    else
                    {
                        logError("Duplicated class " + mTokenIdentifier.getLexeme());
                    }
                }
            }
            getScopeBuilder().popScope();
        }
    }

    return false;
}

IMPL_PARSE(StatementClassDefinitionItem)
{
    if(expectNode<StatementClassVisibility>())
    {
        return true;
    }
    else if(expectNode<StatementMemberVariableDefinition>())
    {
        if(expectToken(TokensDefinitions::Semicolon))
        {
            return true;
        }
    }
    else if(expectNode<StatementConstructorDefinition>())
    {
        return true;
    }
    else if(expectNode<StatementMemberFunctionDefinition>())
    {
        return true;
    }

    return false;
}

IMPL_PARSE(StatementClassVisibility)
{
    bool visibilityFound = false;
    if(expectToken(TokensDefinitions::Public, &mTokenVisibility))
    {
        visibilityFound = true;
    }
    else if(expectToken(TokensDefinitions::Protected, &mTokenVisibility))
    {
        visibilityFound = true;
    }
    else if(expectToken(TokensDefinitions::Private, &mTokenVisibility))
    {
        visibilityFound = true;
    }

    if(visibilityFound)
    {
        if(expectToken(TokensDefinitions::Colon))
        {
            return true;
        }
    }

    return false;
}

IMPL_PARSE(StatementConstructorDefinition)
{
    if(expectNode<StatementFunctionBaseDefinition>())
    {
        return true;
    }

    return false;
}

IMPL_PARSE(StatementMemberVariableDefinition)
{
    if(mStatementVariableDefinition = expectNode<StatementVariableDefinition>())
    {
        return true;
    }

    return false;
}

IMPL_PARSE(StatementMemberFunctionDefinition)
{
    if(mStatementFunctionDefinition = expectNode<StatementFunctionDefinition>())
    {
        return true;
    }

    return false;
}

IMPL_PARSE(StatementEnumDefinition)
{

    return false;
}

IMPL_PARSE(StatementVariableDefinition)
{
    if(mStatementType = expectNode<StatementType>())
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

                    return true;
                }
            }

            // if(expectToken(TokensDefinitions::Semicolon))
            // {
            //     return true;
            //     // if(!getRegistry().isVariable(mTokenIdentifier.getLexeme()))
            //     // {
            //     //     VariableInfo info;
            //     //     info.mIdentifier = mTokenIdentifier;
            //     //     info.mType = mStatementType->mTokenType;
            //     //     getRegistry().registerVariable(info);

            //     // }
            //     // else
            //     // {
            //     //     logError("Duplicated " + mTokenIdentifier.getLexeme());
            //     // }
            // }
            // else
            // {
            //     // NOTE: we could be parsing a method.
            //     if(!checkToken(TokensDefinitions::LeftParen))
            //     {
            //         logError("Expected " + TokensDefinitions::Semicolon.getValue());
            //     }
            // }
        }
    }

    return false;
}

IMPL_PARSE(StatementGlobalVariableDefinition)
{
    if(expectNode<StatementVariableDefinition>())
    {
        return true;
    }

    return false;
}

IMPL_PARSE(StatementFunctionDefinition)
{
    if(mStatementType = expectNode<StatementType>())
    {
        if(mStatementFunctionBaseDefinition = expectNode<StatementFunctionBaseDefinition>())
        {
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
    if(expectToken(TokensDefinitions::Const, &mTokenQualifier))
    {
        return true;
    }

    return false;
}

IMPL_PARSE(StatementParameterDefinition)
{
    if(mStatementType = expectNode<StatementType>())
    {
        if(expectToken(TokensDefinitions::Identifier, &mTokenIdentifier))
        {
            if(!getRegistry().isVariable(mTokenIdentifier.getLexeme(), getScopeBuilder().getScope()))
            {
                VariableInfo info;
                info.mIdentifier = mTokenIdentifier;
                info.mType = mStatementType->mTokenType;
                info.mScope = getScopeBuilder().getScope();
                getRegistry().registerVariable(info);
            }
            else
            {
                logError("Duplicated " + mTokenIdentifier.getLexeme());
            }
            return true;
        }
    }

    return false;
}

IMPL_PARSE(StatementParametersList)
{
    if(expectNodeListEnclosed<StatementParameterDefinition>(TokensDefinitions::Comma, TokensDefinitions::LeftParen, TokensDefinitions::RightParen))
    {
        return true;
    }

    return false;
}

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

IMPL_PARSE(StatementExpression)
{
    if(expectNodeEnclosed<StatementExpression>(TokensDefinitions::LeftParen, TokensDefinitions::RightParen))
    {
        return true;
    }
    else if(expectNode<StatementExpressionBinary>())
    {
        return true;
    }
    if(expectNode<StatementExpressionUnary>())
    {
        return true;
    }
    else if(expectNode<StatementExpressionPrimary>())
    {
        return true;
    }

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
    if(expectToken(TokensDefinitions::Number, &mTokenExpression))
    {
        return true;
    }
    else if(expectToken(TokensDefinitions::True, &mTokenExpression))
    {
        return true;
    }
    else if(expectToken(TokensDefinitions::False, &mTokenExpression))
    {
        return true;
    }
    else if(expectNode<StatementExpressionInvocation>())
    {
        return true;
    }

    return false;
}

IMPL_PARSE(StatementExpressionInvocation)
{
    if(expectToken(TokensDefinitions::This, &mTokenThis))
    {
        if(expectToken(TokensDefinitions::Dot))
        {
            if(expectNode<StatementExpressionCompoundInvocation>())
            {
                return true;
            }
        }

        return true;
    }
    else if(expectNode<StatementExpressionCompoundInvocation>())
    {
        return true;
    }

    return false;
}

IMPL_PARSE(StatementExpressionSimpleInvocation)
{
    if(expectNode<StatementExpressionFunctionInvocation>())
    {
        return true;
    }
    else if(expectNode<StatementExpressionVariableInvocation>())
    {
        return true;
    }

    return false;
}

IMPL_PARSE(StatementExpressionCompoundInvocation)
{
    if(mStatementExpressionSimpleInvocation = expectNode<StatementExpressionSimpleInvocation>())
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

    return false;
}

IMPL_PARSE(StatementExpressionVariableInvocation)
{
    if(expectToken(TokensDefinitions::Identifier, &mTokenIdentifier))
    {
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
            return true;
        }
    }

    return false;
}

IMPL_PARSE(StatementExpressionFunctionParametersList)
{
    if(expectNodeListEnclosed<StatementExpression>(TokensDefinitions::Comma, TokensDefinitions::LeftParen, TokensDefinitions::RightParen))
    {
        return true;
    }

    return false;
}

IMPL_PARSE(StatementUnaryOperator)
{
    if(expectToken(TokensDefinitions::Increment, &mTokenOperator))
    {
        return true;
    }
    else if(expectToken(TokensDefinitions::Decrement, &mTokenOperator))
    {
        return true;
    }
    else if(expectToken(TokensDefinitions::Plus, &mTokenOperator))
    {
        return true;
    }
    else if(expectToken(TokensDefinitions::Minus, &mTokenOperator))
    {
        return true;
    }
    else if(expectToken(TokensDefinitions::Exclamation, &mTokenOperator))
    {
        return true;
    }
    else if(expectToken(TokensDefinitions::Tilde, &mTokenOperator))
    {
        return true;
    }

    return false;
}

IMPL_PARSE(StatementBinaryOperator)
{
    // multiplicative
    if(expectToken(TokensDefinitions::Asterisk, &mTokenOperator))
    {
        return true;
    }
    else if(expectToken(TokensDefinitions::Slash, &mTokenOperator))
    {
        return true;
    }
    else if(expectToken(TokensDefinitions::Percent, &mTokenOperator))
    {
        return true;
    }
    // additive
    else if(expectToken(TokensDefinitions::Plus, &mTokenOperator))
    {
        return true;
    }
    else if(expectToken(TokensDefinitions::Minus, &mTokenOperator))
    {
        return true;
    }
    // relational
    else if(expectToken(TokensDefinitions::LessThan, &mTokenOperator))
    {
        return true;
    }
    else if(expectToken(TokensDefinitions::GreaterThan, &mTokenOperator))
    {
        return true;
    }
    else if(expectToken(TokensDefinitions::LessThanEqual, &mTokenOperator))
    {
        return true;
    }
    else if(expectToken(TokensDefinitions::GreaterThanEqual, &mTokenOperator))
    {
        return true;
    }
    // equality
    else if(expectToken(TokensDefinitions::EqualTo, &mTokenOperator))
    {
        return true;
    }
    else if(expectToken(TokensDefinitions::NotEqualTo, &mTokenOperator))
    {
        return true;
    }
    // bitwise
    else if(expectToken(TokensDefinitions::Ampersand, &mTokenOperator))
    {
        return true;
    }
    else if(expectToken(TokensDefinitions::Caret, &mTokenOperator))
    {
        return true;
    }
    else if(expectToken(TokensDefinitions::Pipe, &mTokenOperator))
    {
        return true;
    }
    // logic
    else if(expectToken(TokensDefinitions::And, &mTokenOperator))
    {
        return true;
    }
    else if(expectToken(TokensDefinitions::Or, &mTokenOperator))
    {
        return true;
    }

    return false;
}

IMPL_PARSE(StatementType)
{
    if(expectToken(TokensDefinitions::Int, &mTokenType))
    {
        return true;
    }
    else if(expectToken(TokensDefinitions::Float, &mTokenType))
    {
        return true;
    }
    else if(expectToken(TokensDefinitions::Void, &mTokenType))
    {
        return true;
    }
    else if(expectToken(TokensDefinitions::Identifier, &mTokenType))
    {
        return true;
    }

    return false;
}