#ifndef FUNCTIONSIGNATUREBUILDER_HPP
#define FUNCTIONSIGNATUREBUILDER_HPP

#include "Core/Module.hpp"

class FunctionSignatureBuilder
{
public:
    FunctionSignatureBuilder() = default;
    
    void push(const std::string& string);

private:
    void regenerateSignature();

public:
    std::string mSignature;
    std::vector<std::string> mSignatureVector;

public:
    CRGET(Signature)
    CRGET(SignatureVector)
};

#endif