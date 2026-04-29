#include "Compiler/Compiler.hpp"

int main(int argc, char* argv[])
{
    if (argc > 1)
    {
        Compiler compiler;
        compiler.init();

        for(int i = 1; i < argc; ++i)
        {
            compiler.addFile(argv[i]);
        }

        compiler.compile();
    }

    return 0;
}