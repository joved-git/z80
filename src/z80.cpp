#include <iostream>
#include "../inc/version.hpp"

#include "../inc/Register_8bits.h"

#define EOL '\n'

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

	regB.setValue(0x01);
	regC.setValue(0xAA);
	regD.setValue(0x0C);
	regE.setValue(0x10);
	regL.setValue(0xC8);

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
            	printf("\na <code>: translate <code> in assembly langage\n");
            	printf("m <cmd>: translate <cmd> in machine code\n");
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
				printf("[%02X] B | C [%02X]\n", regB.getValue(), regC.getValue());
				printf("[%02X] D | E [%02X]\n", regD.getValue(), regE.getValue());
				printf("[%02X] H | L [%02X]\n", regH.getValue(), regL.getValue());
			break;
        }
    }

    return 0;
}
