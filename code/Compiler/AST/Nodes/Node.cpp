#include "Compiler/AST/Nodes/Node.hpp"
#include "Compiler/Parser/Parser.hpp"
#include "Compiler/AST/Nodes/CodeBuilder.hpp"
#include "Compiler/AST/ScopeBuilder.hpp"
#include "Compiler/AST/Context.hpp"

Node::~Node() 
{
    DELETE_CONTENT(mChildren);
}

void Node::init(Parser* parser, Context* context) 
{
    mParser = parser;
    mContext = context;
}

bool Node::parse()
{ 
    return false;
};

void Node::generateCode(CodeBuilder& builder) const
{
    generateCodeChildren(builder);
};

void Node::generateCodeChildren(CodeBuilder& builder) const
{
    FOR_ARRAY(i, mChildren)
    {
        mChildren[i]->generateCode(builder);
    }
}

Context& Node::getContext() const
{
    return *mContext;
}

Node* Node::internalExpectNode(Node* node)
{
    node->init(mParser, mContext);
    bool parseResult = false;
    parseResult = node->parse();
    if(parseResult)
    {
        mChildren.emplace_back(node);
    }
    else
    {
        node->revert();
        delete node;
        return nullptr;
    }

    return node;
}

bool Node::expectToken(const TokenType& type, Token* capture /*= nullptr*/) 
{
    const Token* token = mParser->getCurrentToken();
    bool tokenMatch = token->is(type);

    if(tokenMatch) 
    {
        mTokensParsed++;

        if(capture)
        {
            *capture = *token;
        }

        mParser->advance();
    }

    return tokenMatch;
}

bool Node::expectTokenOrError(const TokenType& type, Token* capture /*= nullptr*/) 
{
    bool result = expectToken(type, capture);

    if(!result)
    {
        logError("Expected " + type.getValue());
    }

    return result;
}

bool Node::checkToken(const TokenType& type)
{
    return mParser->getCurrentToken() ? mParser->getCurrentToken()->is(type) : false;
}

void Node::revert()
{
    FOR_LIST(child, mChildren)
    {
        (*child)->revert();
    }

    FOR_RANGE(i, 0, mTokensParsed)
    {
        mParser->goBack();
    }

    mTokensParsed = 0;
}

void Node::logError(const std::string& error) const
{
    const Token* token = mParser->getCurrentToken();
    const Token* nextToken = mParser->getNextToken();
    BRLINE()
    ECHO("ERROR: " + std::string(typeid(*this).name()));
    ECHO("ERROR: line " + std::to_string((token->getLineNumber() + 1)) + ": " + mParser->getLine(token->getLineNumber()));
    ECHO("ERROR: at token " + token->getLexeme());
    if(nextToken)
    {
        ECHO("ERROR: next token " + nextToken->getLexeme());
    }
    ECHO("ERROR: " + error);
}
