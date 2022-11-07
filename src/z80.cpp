#include <iostream>
#include "../inc/version.hpp"

#define EOL '\n'

int main(int argc, char *argv[])
{
    bool exit=false;
    char c;

    std::cout << "Hello Z80 world !" << std::endl;
    std::cout << "Version: " << VERSION << std::endl << std::endl;
    std::cout << "Type h if your want some help..." << std::endl << std::endl;
    std::cout << "z> ";

    while (!exit)
    {
        c=getchar();
        /* printf("%d-", c);    */

        if (c=='x')
        {
            exit=true;
        }

        if (c=='h')
        {
            printf("\na <code>: translate <code> in assembly langage\n");
            printf("m <cmd>: translate <cmd> in machine code\n");
            printf("r: display registries\n");
            printf("x: exit me\n");
            printf("\n");
            printf("<cmd>: execute the command\n");
            printf("<code>: execute the code\n\n");
        }

        if (c==EOL)
        {
            std::cout << "z> ";
        }
    }

    return 0;
}
