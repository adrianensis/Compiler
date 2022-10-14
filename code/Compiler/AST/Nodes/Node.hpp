#ifndef NODE_HPP
#define NODE_HPP

#include "Core/Module.hpp"
#include "Compiler/Lexer/Token.hpp"

#define DECL_NODE(nodeName) class nodeName: public Node { DECL_PARSE();
#define DECL_PARSE() public: bool parse() override;
#define DECL_CODEGEN() public: void generateCode(CodeBuilder& builder) const override;
#define DECL_TOKEN(tokenName) public: Token tokenName;
#define DECL_CHILD(nodeClass, childName) public: class nodeClass* childName = nullptr;
#define END_NODE() };

#define IMPL_PARSE(nodeName) bool nodeName::parse()
#define IMPL_CODEGEN(nodeName) void nodeName::generateCode(CodeBuilder& builder) const

class Parser;
class CodeBuilder;
class Registry;

class Node 
{
public:
    Node() = default;
    virtual ~Node();
    void init (Parser* parser);
    virtual bool parse();
    virtual void generateCode(CodeBuilder& builder) const;
    void generateCodeChildren(CodeBuilder& builder) const;
    Registry& getRegistry() const;

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
    NodeType* expectNodeEnclosed(const TokenType& leftToken, const TokenType& rightToken) 
    {
        if(expectToken(leftToken))
        {
            NodeType* node = expectNode<NodeType>();
            if(node)
            {
                if(expectToken(rightToken))
                {
                    return node;
                }
                else
                {
                    logError("Expected: " + rightToken.getValue());
                }
            }
        }

        return nullptr;
    }
    template <class NodeType>
    bool expectRepeatNodeEnclosed(const TokenType& leftToken, const TokenType& rightToken) 
    {
        if(expectToken(leftToken))
        {
            expectRepeatNode<NodeType>();

            if(expectToken(rightToken))
            {
                return true;
            }
            else
            {
                logError("Expected: " + rightToken.getValue());
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
protected:
    Parser* mParser = nullptr;
private:
    std::vector<Node*> mChildren;
    u32 mTokensParsed = 0;
public:
    CRGET(Children)
};

#endif