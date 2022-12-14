#include "Compiler/AST/Nodes/CodeBuilder.hpp"
#include "Compiler/AST/Context.hpp"
#include "Compiler/AST/Nodes/Node.hpp"
#include "Compiler/Lexer/Token.hpp"
#include "Compiler/Utils/CharUtils.hpp"

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

void CodeBuilder::addSpaces(u32 spaces)
{
    u32 tabSizeInSpaces = 4;
    u32 totalSpaces = spaces*tabSizeInSpaces;
    FOR_RANGE(i,0,totalSpaces)
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

        FOR_LIST(it, mHeaderIncludes)
        {
            if(!(*it).empty())
            {
                finalCode += "#include \"" + (*it) + ".hpp\"\n";
            }
        }
        
        FOR_LIST(it, mHeaderForwards)
        {
            if(!(*it).empty())
            {
                finalCode += "class " + (*it) + ";\n";
            }
        }
    }
    else
    {
        FOR_LIST(it, mSourceIncludes)
        {
            if(!(*it).empty())
            {
                finalCode += "#include \"" + (*it) + ".hpp\"\n";
            }
        }

        FOR_LIST(it, mSourceForwards)
        {
            if(!(*it).empty())
            {
                finalCode += "class " + (*it) + ";\n";
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

