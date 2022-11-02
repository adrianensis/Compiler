#ifndef PREDEFINED_HPP
#define PREDEFINED_HPP

#include "Core/Module.hpp"
#include "Compiler/AST/Context.hpp"

class Predefined 
{
public:
    Predefined(Context* context) : mContext(context) {};
    void predefine();
private:
    Context* mContext = nullptr;
};

#endif