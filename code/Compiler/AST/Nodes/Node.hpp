#ifndef NODE_HPP
#define NODE_HPP

#include "Core/Module.hpp"
#include "Compiler/Lexer/Token.hpp"
#include "Compiler/AST/Registry.hpp"

#define DECL_NODE(nodeName) class nodeName: public Node { DECL_PARSE();
#define DECL_EXPRESSION_NODE(nodeName) class nodeName: public NodeExpression { DECL_PARSE();
#define DECL_PARSE() public: bool parse() override;
#define DECL_CODEGEN() public: void generateCode(CodeBuilder& builder) const override;
#define DECL_TOKEN(tokenName) public: Token tokenName;
#define DECL_CHILD(nodeClass, childName) public: class nodeClass* childName = nullptr;
#define END_NODE() };

#define IMPL_PARSE(nodeName) bool nodeName::parse()
#define IMPL_CODEGEN(nodeName) void nodeName::generateCode(CodeBuilder& builder) const
#define IMPL_GET_TYPE(nodeName) const Token& nodeName::getTokenType() const

class Parser;
class Context;
class CodeBuilder;
class ScopeBuilder;

class Node 
{
public:
    Node() = default;
    virtual ~Node();
    void init (Parser* parser, Context* context);
    virtual bool parse();
    virtual void generateCode(CodeBuilder& builder) const;
    void generateCodeChildren(CodeBuilder& builder) const;
    Context& getContext() const;

protected:
    template <class NodeType>
    NodeType* expectNode() 
    {
        NodeType* node = new NodeType();
        return static_cast<NodeType*>(internalExpectNode(node));
    }
    template <class NodeType>
    void expectRepeatNode() 
    {
        while(expectNode<NodeType>());
    }
    template <class NodeType>
    NodeType* expectNodeTerminated(const TokenType& tailToken) 
    {
        NodeType* node = expectNode<NodeType>();
        if(node)
        {
            if(expectTokenOrError(tailToken))
            {
                return node;
            }
        }   

        return nullptr;
    }
    template <class NodeType>
    NodeType* expectNodeEnclosed(const TokenType& leftToken, const TokenType& rightToken) 
    {
        if(expectToken(leftToken))
        {
            NodeType* node = expectNodeTerminated<NodeType>(rightToken);
            return node;
        }

        return nullptr;
    }
    template <class NodeType>
    bool expectRepeatNodeEnclosed(const TokenType& leftToken, const TokenType& rightToken) 
    {
        if(expectToken(leftToken))
        {
            expectRepeatNode<NodeType>();

            if(expectTokenOrError(rightToken))
            {
                return true;
            }
        }

        return false;
    }

    template <class NodeType>
    bool expectNodeListEnclosed(const TokenType& separatorToken, const TokenType& leftToken, const TokenType& rightToken)
    {
        if(expectToken(leftToken))
        {
            bool endDetected = false;
            bool wrongEnd = false;
            do
            {
                if(expectNode<NodeType>())
                {
                    // ok
                }

                if(!expectToken(separatorToken))
                {
                    if(expectTokenOrError(rightToken))
                    {
                        endDetected = true;
                    }
                    else
                    {
                        wrongEnd = true;
                        break;
                    }
                }
                
            } while(!endDetected);

            if(!wrongEnd)
            {
                return true;
            }
        }

        return false;
    }

    bool expectToken(const TokenType& type, Token* capture = nullptr);
    bool expectTokenOrError(const TokenType& type, Token* capture = nullptr);
    bool checkToken(const TokenType& type);

    void revert();

    void logError(const std::string& error) const;

private:
    Node* internalExpectNode(Node* node);
    Parser* mParser = nullptr;
    Context* mContext = nullptr;
private:
    std::vector<Node*> mChildren;
    u32 mTokensParsed = 0;
public:
    CRGET(Children)
    CGET(Parser)
};

class NodeExpression : public Node 
{
public: 
    virtual const std::string& getRootTypeIdentifier() const { return mRootTypeNode ? mRootTypeNode->getRootTypeIdentifier() : mRootTypeIdentifier; }
    void setRootTypeIdentifier(const std::string& identifier) { mRootTypeIdentifier = identifier; }
    void setRootTypeNode(const NodeExpression* nodeExpression) { mRootTypeNode = nodeExpression; }
private:
    std::string mRootTypeIdentifier;
    const NodeExpression* mRootTypeNode = nullptr;
};

#endif