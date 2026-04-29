#ifndef FUNCTIONSIGNATUREBUILDER_HPP
#define FUNCTIONSIGNATUREBUILDER_HPP

#include <string>
#include <vector>

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
    const std::string& getSignature() const { return mSignature; };
    const std::vector<std::string>& getSignatureVector() const { return mSignatureVector; };
};

#endif