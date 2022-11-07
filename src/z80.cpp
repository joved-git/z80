#include <iostream>
#include <cstring>

#include "../inc/version.hpp"
#include "../inc/Register_8bits.h"

#define EOL '\n'

/* Byte to binay function	*/
const char *byteToBinary(uint8_t x)
{
    static char b[9];
    b[0] = '\0';

    int z;
    for (z = 128; z > 0; z >>= 1)
    {
        strcat(b, ((x & z) == z) ? "1" : "0");
    }

    return b;
}

int main(int argc, char *argv[])
{
	/* Variables definition	*/
    bool exit=false;
    char c;

    /* Define registers	*/
    Register_8bits regB;
    Register_8bits regC;
    Register_8bits regD;
    Register_8bits regE;
    Register_8bits regH;
    Register_8bits regL;
	Register_8bits regA;
    Register_8bits regF;

	regB.setValue(0x01);
	regC.setValue(0xAA);
	regD.setValue(0x0C);
	regE.setValue(0x10);
	regL.setValue(0xC8);
	regF.setValue(0b01101111);

	/* Display the invitational message	*/
    std::cout << "Hello Z80 world !" << std::endl;
    std::cout << "Version: " << VERSION << std::endl << std::endl;
    std::cout << "Type h if your want some help..." << std::endl << std::endl;
    std::cout << "z> ";

	/* Do what you do or quit !	*/
    while (!exit)
    {
        c=getchar();
        /* printf("%d-", c);    */

		/* Handle strings	*/ 
		switch (c) {
			/* I have to exit	*/
            case 'x':
                exit=true;
				break;
        
			/* OK, display help	*/
			case 'h':
            	printf("\na <code>: translate <code> to assembly langage\n");
				printf("  Example: ld c,b gives 0x41\n");
            	printf("m <cmd>: translate <cmd> in machine code\n");
				printf("  Example: cb22 gives sla d\n");
            	printf("r: display main registers\n");
            	printf("rr: display all registers\n");
            	printf("x: exit me\n");
            	printf("\n");
            	printf("<cmd>: execute the command\n");
            	printf("<code>: execute the code\n");
				break;

			/* Display the prompt after a line	*/
			case EOL:
            	std::cout << std::endl << "z> ";
				break;

			/* Display registers	*/
			case 'r':
				printf("\n");
				printf("B: [%02X]    C: [%02X]\n", regB.getValue(), regC.getValue());
				printf("E: [%02X]    E: [%02X]\n", regD.getValue(), regE.getValue());
				printf("H: [%02X]    L: [%02X]\n", regH.getValue(), regL.getValue());
				printf("A: [%02X]    F: [%02X] [%s]\n", regA.getValue(), regF.getValue(), byteToBinary(regF.getValue()));

			break;
        }
    }

    return 0;
}
