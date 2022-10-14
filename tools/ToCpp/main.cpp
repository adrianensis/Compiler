#include "Compiler/Compiler.hpp"

int main(int argc, char* argv[])
{
    if (argc > 1)
    {
        Compiler compiler;
        compiler.init();

        FOR_RANGE(i, 1, argc)
        {
            compiler.addFile(argv[i]);
        }

        compiler.compile();
    }

    return 0;
}