#include <iostream>
#include "BFMachine.hpp"

int main(int argc, char**argv)
{
    for (;--argc;)
    {
        BFMachine bfm;
        bfm.exec(*++argv, false);
        std::cout<<bfm.get_output();
    }
}
