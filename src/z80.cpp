#include <iostream>
#include <cstring>

#include "../inc/version.hpp"

#include "../inc/common_values.h"
#include "../inc/Register_8bits.h"
#include "../inc/Command.h"

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
    char c=0;
	bool firstChar=TRUE;
	char command[MAX_LEN];
	uint16_t i=0;
	Command newCommand;

	/* Init the values	*/
	command[0]='\0';

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
    std::cout << "Type h if you want some help..." << std::endl << std::endl;
    std::cout << "z> ";

	/* Do what you do or quit !	*/
    while (!exit)
    {
	    c=getchar();
		/* printf("%d-", c);    */

		/* Read the command	*/
		while (c!=EOL)
		{
			if (i<MAX_LEN-1)
			{
				command[i++]=c;		
				c=getchar();
			}
			else 
			{
				c=EOL;
			}
		}

		command[i]='\0';
		i=0;
		//printf("command=<%s>\n", command);
		
		/* Handle the command	*/ 
		newCommand.setEntry(command);
		exit=newCommand.analyse();
		//newCommand.getResult();
		//
		//cmd=newCommand.getInstruction();
		//attr1=newCommand.getFirstAttribute();
		//attr2=newCommand.getSecondAttribute();

		std::cout << "z> ";
		firstChar=FALSE;
    }

    return 0;
}
