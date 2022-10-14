#include "Compiler/AST/AST.hpp"
#include "Compiler/AST/Nodes/Statements/Statement.hpp"
#include "Compiler/AST/Nodes/CodeBuilder.hpp"

AST::~AST() 
{
}

void AST::addFile(const std::string& path)
{
    mFiles.push_back(path);
}

void AST::generateCode()
{
    FOR_LIST(it, mFiles)
    {
        std::ifstream file;
        file.open(*it);

        if(file.good() && !file.fail())
        {
            std::ostringstream ss;
            ss << file.rdbuf(); // reading data
            std::string str = ss.str();
            
            parse(*it, str);
        }

        file.close();
    }

    FOR_LIST(it, mModules)
    {
        generateCode((*it)->mPath, (*it));
    }
}

void AST::parse(const std::string& path, const std::string& content)
{
    Parser* parser = new Parser(content, mRegistry);
    mParsers.push_back(parser);
    parser->mPath = path;
    parser->parse();

    StatementModule* module = new StatementModule();
    mModules.push_back(module);
    module->mPath = path;
    module->init(parser);
    module->parse();
}

void AST::generateCode(const std::string& path, StatementModule* module)
{
    CodeBuilder builderHeader;
    builderHeader.setFileName(std::filesystem::path( path ).filename());
    builderHeader.mGenerateHeaderCode = true;
    module->generateCode(builderHeader);
    std::string headerCode = builderHeader.generateCode();

    CodeBuilder builderSource;
    module->generateCode(builderSource);
    std::string sourceCode = builderSource.generateCode();

    // std::cout << headerCode << std::endl;
    // std::cout << sourceCode << std::endl;

    std::ofstream headerFile;
	headerFile.open(path + ".hpp");

	if(headerFile.good() && !headerFile.fail())
	{
        headerFile << headerCode;
	}

	headerFile.close();

    std::ofstream sourceFile;
	sourceFile.open(path + ".cpp");

	if(sourceFile.good() && !sourceFile.fail())
	{
        sourceFile << sourceCode;
	}

	sourceFile.close();
};