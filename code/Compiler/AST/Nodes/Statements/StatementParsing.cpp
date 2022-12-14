#include "Compiler/AST/Nodes/Statements/Statement.hpp"
#include "Compiler/Parser/Parser.hpp"
#include "Compiler/AST/Nodes/CodeBuilder.hpp"
#include "Compiler/AST/Context.hpp"

// ******* MODULE *******
IMPL_PARSE(StatementModule)
{    
    expectRepeatNode<StatementDefinition>();

    const Token* token = getParser()->getCurrentToken();
    if(!token->is(TokensDefinitions::End))
    {
        logError("Module doesn't compile!");
    }

    mParsed = true;
    mParsed = false;
    return true;
}

void StatementModule::parseHeader()
{
    if(mStatementDeclareModule = expectNode<StatementDeclareModule>())
    {
        while(StatementImportModule* importModuleNode = expectNode<StatementImportModule>())
        {
            mDependencies.push_back(importModuleNode->mTokenIdentifier.getLexeme());
        }
    }
    else
    {
        logError("declare module missing!");
    }
}

IMPL_PARSE(StatementDeclareModule)
{
    if(expectToken(TokensDefinitions::DeclareModule))
    {
        if(expectTokenOrError(TokensDefinitions::LeftParen))
        {
            if(expectTokenOrError(TokensDefinitions::Identifier, &mTokenIdentifier))
            {
                if(expectTokenOrError(TokensDefinitions::RightParen))
                {
                    if(expectTokenOrError(TokensDefinitions::Semicolon))
                    {
                        return true;
                    }
                }
            }
        }
    }

    return false;
}

IMPL_PARSE(StatementImportModule)
{
    if(expectToken(TokensDefinitions::ImportModule))
    {
        if(expectTokenOrError(TokensDefinitions::LeftParen))
        {
            if(expectTokenOrError(TokensDefinitions::Identifier, &mTokenIdentifier))
            {
                if(expectTokenOrError(TokensDefinitions::RightParen))
                {
                    if(expectTokenOrError(TokensDefinitions::Semicolon))
                    {
                        return true;
                    }
                }
            }
        }
    }
    
    return false;
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
            if(mStatementIfBody = expectNode<StatementBlock>())
            {
                if(expectToken(TokensDefinitions::Else))
                {
                    if(mStatementElseBody = expectNode<StatementBlock>())
                    {
                        return true;
                    }
                }
                else
                {
                    return true;
                }
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
                    infoType.mIdentifier = mTokenIdentifier.getLexeme();
                    infoType.mPath = getParser()->mPath;
                    infoType.mAliasedType = mStatementType->mTokenType.getLexeme();
                    getContext().getRegistry().registerInfo(infoType);

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
            bool extendClassOk = true;
            if(expectToken(TokensDefinitions::Colon))
            {
                extendClassOk = false;
                if(expectNode<StatementClassVisibility>())
                {
                    extendClassOk = false;
                    // Parent class
                    if(expectToken(TokensDefinitions::Identifier, &mTokenParentClassIdentifier))
                    {
                        extendClassOk = true;
                    }
                }
            }

            if(!extendClassOk)
            {
                return false;
            }

            TypeInfo typeInfo;
            typeInfo.mIdentifier = mTokenIdentifier.getLexeme();
            typeInfo.mDataType = DataType::TYPE_CLASS;
            typeInfo.mIsStack = !mTokenStack.getIsNull();
            typeInfo.mPath = getParser()->mPath;
            getContext().getRegistry().registerInfo(typeInfo);

            getContext().pushScope(mTokenIdentifier.getLexeme());
            getContext().pushClassScope(mTokenIdentifier.getLexeme());

            // register "this" keyword
            VariableInfo thisInfo;
            thisInfo.mIdentifier = "this";
            thisInfo.mType = typeInfo.mIdentifier;
            getContext().getRegistry().registerInfo(thisInfo);

            if(expectRepeatNodeEnclosed<StatementClassDefinitionItem>(TokensDefinitions::LeftCurly, TokensDefinitions::RightCurly))
            {        
                getContext().popScope();
                getContext().popClassScope();
                return true;
            }
            getContext().popScope();
            getContext().popClassScope();
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
    if(mStatementFunctionBaseDefinition = expectNode<StatementFunctionBaseDefinition>())
    {
        FunctionInfo infoFunction;
        infoFunction.mIdentifier = mStatementFunctionBaseDefinition->mFunctionSignature.getSignature();
        infoFunction.mType = mStatementFunctionBaseDefinition->mTokenIdentifier.getLexeme();
        infoFunction.mIsConstructor = true;
        getContext().getRegistry().registerInfo(infoFunction);
        return true;
    }
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
    if(mStatementTypeQualifierVariable = expectNode<StatementTypeQualifierVariable>())
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
                    infoVariable.mIdentifier = mTokenIdentifier.getLexeme();
                    infoVariable.mType = mStatementTypeQualifierVariable->mStatementTypeQualifier->mStatementType->mTokenType.getLexeme();
                    infoVariable.mIsConst = ! mStatementTypeQualifierVariable->mStatementTypeQualifier->mTokenTypeConstQualifier.getIsNull();
                    infoVariable.mIsStatic = ! mStatementTypeQualifierVariable->mTokenTypeStaticQualifier.getIsNull();
                    getContext().getRegistry().registerInfo(infoVariable);

                    return true;
                }
            }
        }
    }

    return false;
}

IMPL_PARSE(StatementGlobalVariableDefinition)
{
    if(expectNode<StatementVariableDefinition>()) { return true; }
    return false;
}

// ******* FUNCTION DEFINITION *******
IMPL_PARSE(StatementFunctionDefinition)
{
    if(mStatementTypeQualifierFunctionReturn = expectNode<StatementTypeQualifierFunctionReturn>())
    {
        FunctionInfo infoFunction;
        infoFunction.mType = mStatementTypeQualifierFunctionReturn->mStatementTypeQualifier->mStatementType->mTokenType.getLexeme();

        if(mStatementFunctionBaseDefinition = expectNode<StatementFunctionBaseDefinition>())
        {
            infoFunction.mIdentifier = mStatementFunctionBaseDefinition->mFunctionSignature.getSignature();
            getContext().getRegistry().registerInfo(infoFunction);

            return true;
        }
    }

    return false;
}

IMPL_PARSE(StatementFunctionBaseDefinition)
{
    if(expectToken(TokensDefinitions::Identifier, &mTokenIdentifier))
    {
        // Operator overload
        if(StatementBinaryOperator* statementBinaryOperator = expectNode<StatementBinaryOperator>())
        {
            mTokenOperatorOverload = statementBinaryOperator->mTokenOperator;
        }
        else
        {
            if(StatementUnaryOperator* statementUnaryOperator = expectNode<StatementUnaryOperator>())
            {
                mTokenOperatorOverload = statementUnaryOperator->mTokenOperator;
            }
        }
        
        if(mStatementParametersList = expectNode<StatementParametersList>())
        {
            if(mStatementFunctionQualifier = expectNode<StatementFunctionQualifier>())
            {
                // ok
            }

            // Generate function signature

            if(mTokenOperatorOverload.getIsNull())
            {
                mFunctionSignature.push(mTokenIdentifier.getLexeme());
            }
            else
            {
                mFunctionSignature.push(mTokenIdentifier.getLexeme() + mTokenOperatorOverload.getLexeme());
            }

            FOR_LIST(it, mStatementParametersList->getChildren())
            {
                const StatementParameterDefinition* paramDefNode = dynamic_cast<StatementParameterDefinition*>(*it);
                mFunctionSignature.push(paramDefNode->mStatementTypeQualifierParameter->mStatementTypeQualifier->mStatementType->mTokenType.getLexeme());
            }

            // set scope now
            getContext().pushScope(mFunctionSignature.getSignature());

            // Register parameter infos
            FOR_LIST(it, mStatementParametersList->getChildren())
            {
                StatementParameterDefinition* paramDefNode = dynamic_cast<StatementParameterDefinition*>(*it);
                getContext().getRegistry().registerInfo(paramDefNode->mParameterInfo);
            }

            if(mStatementBody = expectNode<StatementBlock>())
            {
                getContext().popScope();
                return true;
            }
            else
            {
                if(mStatementFunctionQualifier->mIsPure)
                {
                    if(expectToken(TokensDefinitions::Semicolon))
                    {
                        getContext().popScope();
                        return true;
                    }
                }
            }

            getContext().popScope();
        }
    }

    return false;
}

IMPL_PARSE(StatementFunctionQualifier)
{
    expectToken(TokensDefinitions::Const, &mTokenQualifierConst);

    if(expectToken(TokensDefinitions::Override, &mTokenQualifierOverride))
    {
        // ok
    }
    else
    {
        if(expectToken(TokensDefinitions::Equal))
        {
            Token tokenNumber;
            if(expectToken(TokensDefinitions::Number, &tokenNumber))
            {
                if(tokenNumber.getLexeme() == "0")
                {
                    mIsPure = true;
                    return true;
                }
            }

            return false;
        }
    }

    return true;
}

IMPL_PARSE(StatementParameterDefinition)
{
    if(mStatementTypeQualifierParameter = expectNode<StatementTypeQualifierParameter>())
    {
        if(expectToken(TokensDefinitions::Identifier, &mTokenIdentifier))
        {
            //VariableInfo infoVariable;
            mParameterInfo.mIdentifier = mTokenIdentifier.getLexeme();
            mParameterInfo.mType = mStatementTypeQualifierParameter->mStatementTypeQualifier->mStatementType->mTokenType.getLexeme();
            //mParameterInfo.mScope = getContext().getScope();
            mParameterInfo.mIsConst = ! mStatementTypeQualifierParameter->mStatementTypeQualifier->mTokenTypeConstQualifier.getIsNull();
            //getContext().getRegistry().registerInfo(infoVariable);
            return true;
        }
    }

    return false;
}

IMPL_PARSE(StatementParametersList)
{
    if(expectNodeListEnclosed<StatementParameterDefinition>(TokensDefinitions::Comma, TokensDefinitions::LeftParen, TokensDefinitions::RightParen)) { return true; }
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
    if(mNodeExpression = expectNode<StatementExpressionBinary>())
    {
        setRootTypeNode(mNodeExpression);
        return true;
    }
    if(mNodeExpression = expectNode<StatementExpressionUnary>())
    {
        setRootTypeNode(mNodeExpression);
        return true;
    }
    if(mNodeExpression = expectNode<StatementExpressionPrimary>())
    {
        setRootTypeNode(mNodeExpression);
        return true;
    }
    if(mNodeEnclosedExpression = expectNodeEnclosed<StatementExpression>(TokensDefinitions::LeftParen, TokensDefinitions::RightParen))
    {
        setRootTypeNode(mNodeEnclosedExpression);
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
            setRootTypeNode(mStatementExpressionPrimary);
            return true;
        }
    }
    else if(mStatementExpressionPrimary = expectNode<StatementExpressionPrimary>())
    {
        if(mStatementUnaryOperatorPost = expectNode<StatementUnaryOperator>())
        {
            setRootTypeNode(mStatementExpressionPrimary);
            return true;
        }
    }

    return false;
}

IMPL_PARSE(StatementExpressionBinary)
{
    if(mStatementUnaryOperatorPre = expectNode<StatementUnaryOperator>())
    {
        // ok
    }

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
        setRootTypeIdentifier(mTokenExpression.getLexeme());
        return true;
    }
    if(expectToken(TokensDefinitions::True, &mTokenExpression))
    {
        setRootTypeIdentifier(mTokenExpression.getLexeme());
        return true;
    }
    if(expectToken(TokensDefinitions::False, &mTokenExpression))
    {
        setRootTypeIdentifier(mTokenExpression.getLexeme());
        return true;
    }
    if(mStatementExpressionInvocation = expectNode<StatementExpressionInvocation>())
    {
        setRootTypeNode(mStatementExpressionInvocation);
        return true;
    }
    if(mStatementEnclosedExpression = expectNodeEnclosed<StatementExpression>(TokensDefinitions::LeftParen, TokensDefinitions::RightParen))
    {
        setRootTypeNode(mStatementEnclosedExpression);
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
            if(mStatementExpressionCompoundInvocation = expectNode<StatementExpressionCompoundInvocation>())
            {
                setRootTypeNode(mStatementExpressionCompoundInvocation);
                //setRootTypeIdentifier(mTokenThis.getLexeme());
                return true;
            }
        }

        setRootTypeIdentifier(mTokenThis.getLexeme());
        return true;
    }
    if(mStatementExpressionCompoundInvocation = expectNode<StatementExpressionCompoundInvocation>())
    {
        setRootTypeNode(mStatementExpressionCompoundInvocation);
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
    if(mStatementScope = expectNode<StatementScope>())
    {
        // ok
    }

    if(mStatementExpressionSimpleInvocation = expectNode<StatementExpressionSimpleInvocation>())
    {
        if(expectToken(TokensDefinitions::Dot))
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
        setRootTypeIdentifier(mTokenIdentifier.getLexeme());
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
            // Generate function signature
            FunctionSignatureBuilder functionSignature;
            functionSignature.push(mTokenIdentifier.getLexeme());
            FOR_LIST(it, mStatementExpressionFunctionParametersList->getChildren())
            {
                const StatementExpression* paramExpression = dynamic_cast<StatementExpression*>(*it);
                const std::string typeIdentifier = getContext().findTypedDataTypeInfoIdentifier(paramExpression->getRootTypeIdentifier());
                functionSignature.push(typeIdentifier);
            }
            setRootTypeIdentifier(functionSignature.getSignature());
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

// ******* SCOPE *******
IMPL_PARSE(StatementScope)
{
    if(expectToken(TokensDefinitions::Identifier, &mTokenScope))
    {
        if(expectToken(TokensDefinitions::Scope))
        {
            mScopeVector.push_back(mTokenScope.getLexeme());
            if(mStatementScope = expectNode<StatementScope>())
            {
                mScopeVector.insert(mScopeVector.end(), mStatementScope->mScopeVector.begin(), mStatementScope->mScopeVector.end());
                return true;
            }

            return true;
        }
    }

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
    if(expectToken(TokensDefinitions::Equal, &mTokenOperator)) { return true; }
    return false;
}

IMPL_PARSE(StatementBinaryOperator)
{
    // multiplicative
    if(expectToken(TokensDefinitions::Asterisk, &mTokenOperator)) { return true; }
    if(expectToken(TokensDefinitions::Slash, &mTokenOperator)) { return true; }
    if(expectToken(TokensDefinitions::Percent, &mTokenOperator)) { return true; }
    if(expectToken(TokensDefinitions::MulEqual, &mTokenOperator)) { return true; }
    if(expectToken(TokensDefinitions::DivEqual, &mTokenOperator)) { return true; }
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
    mStatementScope = expectNode<StatementScope>();
    
    if(expectToken(TokensDefinitions::Int, &mTokenType)) { return true; }
    if(expectToken(TokensDefinitions::Float, &mTokenType)) { return true; }
    if(expectToken(TokensDefinitions::Double, &mTokenType)) { return true; }
    if(expectToken(TokensDefinitions::Bool, &mTokenType)) { return true; }
    if(expectToken(TokensDefinitions::Void, &mTokenType)) { return true; }
    if(expectToken(TokensDefinitions::Identifier, &mTokenType)) { return true; }
    return false;
}

IMPL_PARSE(StatementTypeQualifier)
{
    expectToken(TokensDefinitions::Const, &mTokenTypeConstQualifier);

    if(mStatementType = expectNode<StatementType>()) { return true; }
    return false;
}

IMPL_PARSE(StatementTypeQualifierVariable)
{
    expectToken(TokensDefinitions::Static, &mTokenTypeStaticQualifier);

    if(mStatementTypeQualifier = expectNode<StatementTypeQualifier>()) { return true; }
    return false;
}

IMPL_PARSE(StatementTypeQualifierParameter)
{
    if(mStatementTypeQualifier = expectNode<StatementTypeQualifier>()) { return true; }
    return false;
}

IMPL_PARSE(StatementTypeQualifierFunctionReturn)
{
    expectToken(TokensDefinitions::Static, &mTokenTypeStaticQualifier);

    expectToken(TokensDefinitions::Virtual, &mTokenTypeVirtualQualifier);

    if(mStatementTypeQualifier = expectNode<StatementTypeQualifier>()) { return true; }
    return false;
}