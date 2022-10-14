#include "Compiler/AST/Nodes/Node.hpp"
#include "Compiler/Parser/Parser.hpp"
#include "Compiler/AST/Nodes/CodeBuilder.hpp"

Node::~Node() 
{
    DELETE_CONTENT(mChildren);
}

void Node::init(Parser* parser) 
{
    mParser = parser;
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

Registry& Node::getRegistry() const
{
    return mParser->getRegistry();
}

Node* Node::internalExpectNode(Node* node)
{
    node->init(mParser);
    bool parseResult = false;
    parseResult = node->parse();
    //std::cout << std::boolalpha << typeid(*this).name() << " parse: " << parseResult << std::endl;
    if(parseResult)
    {
        mChildren.emplace_back(node);
    } else {
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
        //std::cout << typeid(*this).name() << ": " << token->getLexeme() << std::endl;
        mParser->advance();
    }

    return tokenMatch;
}

bool Node::expectTokenOrError(const TokenType& type, Token* capture /*= nullptr*/) 
{
    bool result = expectToken(TokensDefinitions::Semicolon);

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
    std::cout << "ERROR: line " << (token->getLineNumber() + 1) << ": " << mParser->getLine(token->getLineNumber()) << std::endl;
    std::cout << "ERROR: at token " << token->getLexeme() << std::endl;
    std::cout << "ERROR: " << error << std::endl;
}
