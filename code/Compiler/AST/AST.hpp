#ifndef AST_HPP
#define AST_HPP

#include "Core/Module.hpp"
#include "Compiler/Parser/Parser.hpp"
#include "Compiler/AST/Context.hpp"

class StatementModule;

class AST 
{
public:
    AST() = default;
    virtual ~AST();
    void addFile(const std::string& path);
    void generateCode();

private:
    void initModule(const std::string& path, const std::string& content);
    void parseModule(StatementModule* module);
    void generateCode(StatementModule* module);

private:
    std::vector<std::string> mFiles;
    std::map<std::string, StatementModule*> mModules;
    std::vector<Parser*> mParsers;
    Context mContext;
};

#endif