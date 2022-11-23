#ifndef TOKEN_HPP
#define TOKEN_HPP

#include "Core/Module.hpp"

class TokenType
{
public:
    TokenType() = default;
    TokenType(const std::string& name, const std::string& value, bool isKeyword, bool isPrimitive);

    bool operator== (const TokenType& other) const 
    {
        return mId == other.mId;
    }

    bool operator!= (const TokenType& other) const 
    {
        return !(*this == other);
    }

    TokenType& operator=(const TokenType& other) 
    {
        mName = other.mName;
        mValue = other.mValue;
        mId = other.mId;
        return *this;
    }

    std::string getName() const 
    {
        return mName;
    }

    std::string getValue() const 
    {
        return mValue;
    }

    bool matchString(const std::string& str) const
    {
        return mValue == str;
    }
    
    bool matchString(const char c) const
    {
        return mValue == std::string(1, c);
    }

    static bool isKeyword(const std::string& keyword);
    static const TokenType& getKeywordTokenType(const std::string& keyword);
    static bool isPrimitive(const std::string& primitive);

private:
    inline static u32 smTokenTypeCounter = 0;
    inline static std::map<std::string, TokenType> smKeywordsMap = {};
    inline static std::map<std::string, TokenType> smPrimitivesMap = {};
    u32 mId = 0;
    std::string mName{};
    std::string mValue{};
};

#define DECLARE_TOKEN(tokenName, value) inline static TokenType tokenName = TokenType(#tokenName, value, false, false);
#define DECLARE_TOKEN_SIMPLE(tokenName, singleChar) DECLARE_TOKEN(tokenName, std::string(1, singleChar));
#define DECLARE_TOKEN_KEYWORD(tokenName, value) inline static TokenType tokenName = TokenType(#tokenName, value, true, false);
#define DECLARE_TOKEN_PRIMITIVE(tokenName, value) inline static TokenType tokenName = TokenType(#tokenName, value, true, true);

class TokensDefinitions 
{
public:
    // literals
    DECLARE_TOKEN(Number, std::string());
    DECLARE_TOKEN(Identifier, std::string());
    DECLARE_TOKEN_KEYWORD(True, "true");
    DECLARE_TOKEN_KEYWORD(False, "false");


    // symbols
    DECLARE_TOKEN_SIMPLE(LeftParen, '(');
    DECLARE_TOKEN_SIMPLE(RightParen, ')');
    DECLARE_TOKEN_SIMPLE(LeftSquare, '[');
    DECLARE_TOKEN_SIMPLE(RightSquare, ']');
    DECLARE_TOKEN_SIMPLE(LeftCurly, '{');
    DECLARE_TOKEN_SIMPLE(RightCurly, '}');
    DECLARE_TOKEN_SIMPLE(LessThan, '<');
    DECLARE_TOKEN_SIMPLE(GreaterThan, '>');
    DECLARE_TOKEN_SIMPLE(Equal, '=');
    DECLARE_TOKEN_SIMPLE(Plus, '+');
    DECLARE_TOKEN_SIMPLE(Minus, '-');
    DECLARE_TOKEN_SIMPLE(Asterisk, '*');
    DECLARE_TOKEN_SIMPLE(Ampersand, '&');
    DECLARE_TOKEN_SIMPLE(Exclamation, '!');
    DECLARE_TOKEN_SIMPLE(Tilde, '~');
    DECLARE_TOKEN_SIMPLE(Caret, '^');
    DECLARE_TOKEN_SIMPLE(Slash, '/');
    DECLARE_TOKEN_SIMPLE(Percent, '%');
    DECLARE_TOKEN_SIMPLE(Hash, '#');
    DECLARE_TOKEN_SIMPLE(Dot, '.');
    DECLARE_TOKEN_SIMPLE(Comma, ',');
    DECLARE_TOKEN_SIMPLE(Colon, ':');
    DECLARE_TOKEN_SIMPLE(Semicolon, ';');
    DECLARE_TOKEN_SIMPLE(SingleQuote, '\'');
    DECLARE_TOKEN_SIMPLE(DoubleQuote, '"');
    DECLARE_TOKEN_SIMPLE(Pipe, '|');
    
    DECLARE_TOKEN(Arrow, "->");
    DECLARE_TOKEN(Increment, "++");
    DECLARE_TOKEN(Decrement, "--");
    DECLARE_TOKEN(PlusEqual, "+=");
    DECLARE_TOKEN(MinusEqual, "-=");
    DECLARE_TOKEN(MulEqual, "*=");
    DECLARE_TOKEN(DivEqual, "/=");
    DECLARE_TOKEN(EqualTo, "==");
    DECLARE_TOKEN(NotEqualTo, "!=");
    DECLARE_TOKEN(LessThanEqual, "<=");
    DECLARE_TOKEN(GreaterThanEqual, ">=");
    DECLARE_TOKEN(And, "&&");
    DECLARE_TOKEN(Or, "||");
    DECLARE_TOKEN(Scope, "::");

    // primitives
    DECLARE_TOKEN_PRIMITIVE(Int, "int");
    DECLARE_TOKEN_PRIMITIVE(Float, "float");
    DECLARE_TOKEN_PRIMITIVE(Void, "void");
    DECLARE_TOKEN_PRIMITIVE(Long, "long");
    DECLARE_TOKEN_PRIMITIVE(Short, "short");
    DECLARE_TOKEN_PRIMITIVE(Bool, "bool");

    // keywords
    DECLARE_TOKEN_KEYWORD(Using, "using");
    DECLARE_TOKEN_KEYWORD(Return, "return");
    DECLARE_TOKEN_KEYWORD(Break, "break");
    DECLARE_TOKEN_KEYWORD(Case, "case");
    DECLARE_TOKEN_KEYWORD(Char, "char");
    DECLARE_TOKEN_KEYWORD(Const, "const");
    DECLARE_TOKEN_KEYWORD(Continue, "continue");
    DECLARE_TOKEN_KEYWORD(Default, "default");
    DECLARE_TOKEN_KEYWORD(Do, "do");
    DECLARE_TOKEN_KEYWORD(Double, "double");
    DECLARE_TOKEN_KEYWORD(Else, "else");
    DECLARE_TOKEN_KEYWORD(Enum, "enum");
    DECLARE_TOKEN_KEYWORD(For, "for");
    DECLARE_TOKEN_KEYWORD(If, "if");
    DECLARE_TOKEN_KEYWORD(Signed, "signed");
    DECLARE_TOKEN_KEYWORD(Sizeof, "sizeof");
    DECLARE_TOKEN_KEYWORD(Static, "static");
    DECLARE_TOKEN_KEYWORD(Inline, "inline");
    DECLARE_TOKEN_KEYWORD(Switch, "switch");
    DECLARE_TOKEN_KEYWORD(Unsigned, "unsigned");
    DECLARE_TOKEN_KEYWORD(While, "while");
    DECLARE_TOKEN_KEYWORD(Class, "class");
    DECLARE_TOKEN_KEYWORD(Stack, "stack");
    DECLARE_TOKEN_KEYWORD(Public, "public");
    DECLARE_TOKEN_KEYWORD(Protected, "protected");
    DECLARE_TOKEN_KEYWORD(Private, "private");
    DECLARE_TOKEN_KEYWORD(This, "this");
    DECLARE_TOKEN_KEYWORD(DeclareModule, "decl_module");
    DECLARE_TOKEN_KEYWORD(ImportModule, "import_module");

    // special
    DECLARE_TOKEN(Comment, std::string());
    DECLARE_TOKEN_SIMPLE(End, '\0');
    DECLARE_TOKEN(Unexpected, std::string());
};

#undef DECLARE_TOKEN

class Token 
{
public:
    Token() = default;

    Token(const TokenType& type, const char* inputStream, std::size_t len, u32 lineNumber)
    : mTokenType{type}, mLexeme(inputStream, len), mLineNumber(lineNumber) { mIsNull=false; }

    Token(const TokenType& type, const char* inputStream, const char* end, u32 lineNumber)
    : mTokenType{type}, mLexeme(inputStream, std::distance(inputStream, end)), mLineNumber(lineNumber) { mIsNull=false; }

    bool is(const TokenType& type) const { return mTokenType == type; }
    bool isOneOf(const TokenType& tokenType1, const TokenType& tokenType2) const { return is(tokenType1) || is(tokenType2); }

    std::string getLexeme() const 
    {
        return std::string(mLexeme);
    }
    std::string toString() const 
    {
        return mTokenType.getName() + std::string(" : ") + getLexeme();
    }

    bool isKeyword() const;
    bool isPrimitive() const;

private:
    TokenType mTokenType{};
    std::string_view mLexeme{};
    u32 mLineNumber = 0;

    bool mIsNull = true;

public:
    GET(LineNumber)
    GET(IsNull)
};

#endif