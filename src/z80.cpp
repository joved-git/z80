#include <iostream>

int main(int argc, char *argv[])
{
    bool exit=false;
    char c;

    std::cout << "Hello Z80 world !" << std::endl;

    while (!exit)
    {
        c=getchar();
        //printf("%c", c);

        if (c=='x')
        {
            exit=true;
        }
    }

    return 0;
}