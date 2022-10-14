#ifndef AST_HPP
#define AST_HPP

#include "Core/Module.hpp"
#include "Compiler/Parser/Parser.hpp"
#include "Compiler/AST/Registry.hpp"

class StatementModule;

class AST 
{
public:
    AST() = default;
    virtual ~AST();
    void addFile(const std::string& path);
    void generateCode();

private:
    void parse(const std::string& path, const std::string& content);
    void generateCode(const std::string& path, StatementModule* module);

private:
    std::vector<std::string> mFiles;
    std::vector<StatementModule*> mModules;
    std::vector<Parser*> mParsers;
    Registry mRegistry;
};

#endif