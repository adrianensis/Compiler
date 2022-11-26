#include "Compiler/AST/AST.hpp"
#include "Compiler/AST/Nodes/Statements/Statement.hpp"
#include "Compiler/AST/Nodes/CodeBuilder.hpp"
#include "Compiler/Predefined/Predefined.hpp"

AST::~AST() 
{
    MAP_DELETE_CONTENT(mModules);
    DELETE_CONTENT(mParsers);
}

void AST::addFile(const std::string& path)
{
    mFiles.push_back(path);
}

void AST::generateCode()
{
    // PREDEFINE STUFF
    Predefined predefined(&mContext);
    predefined.predefine();

    FOR_LIST(it, mFiles)
    {
        std::ifstream file;
        file.open(*it);

        if(file.good() && !file.fail())
        {
            std::ostringstream ss;
            ss << file.rdbuf(); // reading data
            std::string fileContent = ss.str();
            
            initModule(*it, fileContent);
        }

        file.close();
    }
    
    FOR_MAP(it, mModules)
    {   
        parseModule(it->second);
    }

    FOR_MAP(it, mModules)
    {
        generateCode(it->second);
    }
}

void AST::initModule(const std::string& path, const std::string& content)
{
    ECHO("INIT " + path);

    Parser* parser = new Parser(content);
    mParsers.push_back(parser);
    parser->mPath = path;
    parser->parse();

    StatementModule* module = new StatementModule();
    module->mPath = path;
    module->init(parser, &mContext);

    module->parseHeader();

    std::string moduleName = module->mStatementDeclareModule->mTokenIdentifier.getLexeme();
    MAP_INSERT(mModules, moduleName, module);
}

void AST::parseModule(StatementModule* module)
{
    if(!module->mParsed && !module->mIsParsing)
    {
        std::string moduleName = module->mStatementDeclareModule->mTokenIdentifier.getLexeme();
        ECHO("PARSING " + moduleName);
        module->mIsParsing = true;

        FOR_LIST(it, module->mDependencies)
        {
            StatementModule* dependency = mModules[*it];
            std::string dependencyName = dependency->mStatementDeclareModule->mTokenIdentifier.getLexeme();
            ECHO("DEPENDENCY DETECTED " + dependencyName);
            parseModule(dependency);
        }

        if(module->mParsed)
        {
            return;
        }

        module->parse();

        ECHO("PARSED " + moduleName);
    }
}

void AST::generateCode(StatementModule* module)
{
    ECHO("CODE BUILDING " + module->mPath);

    CodeBuilder builderHeader;
    builderHeader.setFileName(std::filesystem::path( module->mPath ).filename());
    builderHeader.mGenerateHeaderCode = true;
    module->generateCode(builderHeader);
    std::string headerCode = builderHeader.generateCode();

    CodeBuilder builderSource;
    module->generateCode(builderSource);
    std::string sourceCode = builderSource.generateCode();

    std::ofstream headerFile;
	headerFile.open(module->mPath + ".hpp");

	if(headerFile.good() && !headerFile.fail())
	{
        headerFile << headerCode;
	}

	headerFile.close();

    std::ofstream sourceFile;
	sourceFile.open(module->mPath + ".cpp");

	if(sourceFile.good() && !sourceFile.fail())
	{
        sourceFile << sourceCode;
	}

	sourceFile.close();
};