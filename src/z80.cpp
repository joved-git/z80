#include <iostream>
#include <cstring>

#include "../inc/version.hpp"

#include "../inc/common_values.h"
//#include "../inc/Register_8bits.h"
#include "../inc/Command.h"

int main(int argc, char *argv[])
{
	/* Variables definition	*/
    bool exit=false;
    char c=0;
	char command[MAX_LEN];
	uint16_t i=0;
	Command newCommand;
	
	/* Init the values	*/
	command[0]='\0';

/* Define Registers	Pack structure	*/
	/*RegisterPack regPack;
	regPack.regB=new(Register_8bits);
	regPack.regC=new(Register_8bits);

	regPack.regB->setValue(0x01);
	regPack.regC->setValue(0xAA);

	regPack.regC.setValue(0xAA);
	regPack.regD.setValue(0x0C);
	regPack.regE.setValue(0x10);
	regPack.regL.setValue(0xC8);
	regPack.regF.setValue(0b01101111);
	*/

	/* Display the invitational message	*/
    std::cout << "Hello Z80 world !" << std::endl;
    std::cout << "Version: " << VERSION << std::endl << std::endl;
    std::cout << "Type h if you want some help..." << std::endl << std::endl;
    std::cout << "z> ";

	/* Set the register pack to the command	*/
	//newCommand.setRegisterPack(&regPack);

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
		//printf("*** 2");
		//newCommand.getResult();
		//
		//cmd=newCommand.getInstruction();
		//attr1=newCommand.getFirstAttribute();
		//attr2=newCommand.getSecondAttribute();

		if (!exit) 
		{
			std::cout << std::endl << "z> ";
		}
    }

	std::cout << std::endl << "Bye, see you next time !!" << std::endl  << std::endl;

    return 0;
}
