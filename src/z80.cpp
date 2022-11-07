#include <iostream>
#include "../inc/version.hpp"

int main(int argc, char *argv[])
{
    bool exit=false;
    char c;

    std::cout << "Hello Z80 world !" << std::endl;
    std::cout << "Version: " << VERSION << std::endl << std::endl;
    std::cout << "Type h if your want some help..." << std::endl << std::endl;

    while (!exit)
    {
        c=getchar();
        //printf("%c", c);

        if (c=='x')
        {
            exit=true;
        }

        if (c=='h')
        {
            printf("a <code>: translate <code> in command\n");
            printf("m <cmd>: translate <cmd> in machine code\n");
            printf("r: display registries\n");
            printf("\n");
            printf("<cmd>: execute the command\n");
        }
    }

    return 0;
}
