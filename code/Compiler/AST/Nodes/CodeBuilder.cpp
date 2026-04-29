#include "Compiler/AST/Nodes/CodeBuilder.hpp"
#include "Compiler/AST/Context.hpp"
#include "Compiler/AST/Nodes/Node.hpp"
#include "Compiler/Lexer/Token.hpp"
#include "Compiler/Utils/CharUtils.hpp"
#include <algorithm>

void CodeBuilder::addString(const std::string& string)
{
    if(mIsNewLine)
    {
        addSpaces(mIndent);
        mIsNewLine = false;
    }
    else
    {
        // if we are joining 2 strings, add 1 space in between
        if(CharUtils::isIdentifierChar(mCode.back()) && CharUtils::isIdentifierChar(string.front()))
        {
            mCode += " ";
        }
    }

    mCode += string;
}

void CodeBuilder::addSpaces(unsigned int spaces)
{
    unsigned int tabSizeInSpaces = 4;
    unsigned int totalSpaces = spaces*tabSizeInSpaces;
    for (int i = 0; i < totalSpaces; ++i)
    {
        mCode += " ";
    }
}

void CodeBuilder::addToken(const Token& token)
{
    if(!token.getIsNull())
    {
        addString(token.getLexeme());
    }
}

void CodeBuilder::addTokenType(const TokenType& tokenType)
{
    addString(tokenType.getValue());
}

void CodeBuilder::newLine()
{
    mIsNewLine = true;
    mCode += "\n";
}

void CodeBuilder::indent()
{
    mIndent++;
}

void CodeBuilder::unindent()
{
    if(mIndent > 0)
    {
        mIndent--;
    }
}

void CodeBuilder::addScope(const Context& context)
{
    if(context.hasScope())
    {
        addString(context.getScope());
        addString("::");
    }
}

void CodeBuilder::includeInHeader(const std::string& string)
{
    if (std::find(mHeaderIncludes.begin(), mHeaderIncludes.end(), string) == mHeaderIncludes.end())
    {
        mHeaderIncludes.push_back(string);
    }
}

void CodeBuilder::includeInSource(const std::string& string)
{
    if (std::find(mSourceIncludes.begin(), mSourceIncludes.end(), string) == mSourceIncludes.end())
    {
        mSourceIncludes.push_back(string);
    }
}

void CodeBuilder::forwardInHeader(const std::string& string)
{
    if (std::find(mHeaderForwards.begin(), mHeaderForwards.end(), string) == mHeaderForwards.end())
    {
        mHeaderForwards.push_back(string);
    }
}

void CodeBuilder::forwardInSource(const std::string& string)
{
    if (std::find(mSourceForwards.begin(), mSourceForwards.end(), string) == mSourceForwards.end())
    {
        mSourceForwards.push_back(string);
    }
}

const std::string& CodeBuilder::generateCode()
{
    std::string finalCode;

    if(mGenerateHeaderCode)
    {
        std::string ifGuardName = mFileName;

        std::replace(ifGuardName.begin(), ifGuardName.end(), '.', '_');

        finalCode += "#ifndef " + ifGuardName + "_HPP\n";
        finalCode += "#define " + ifGuardName + "_HPP\n";

        for(const auto& it: mHeaderIncludes)
        {
            if(!it.empty())
            {
                finalCode += "#include \"" + it + ".hpp\"\n";
            }
        }
        
        for(const auto& it: mHeaderForwards)
        {
            if(!it.empty())
            {
                finalCode += "class " + it + ";\n";
            }
        }
    }
    else
    {
        for(const auto& it: mSourceIncludes)
        {
            if(!it.empty())
            {
                finalCode += "#include \"" + it + ".hpp\"\n";
            }
        }

        for(const auto& it: mSourceForwards)
        {
            if(!it.empty())
            {
                finalCode += "class " + it + ";\n";
            }
        }
    }

    finalCode += mCode;
    
    if(mGenerateHeaderCode)
    {
        finalCode += "#endif // " + mFileName;
    }

    mCode = finalCode;

    return mCode;
}

