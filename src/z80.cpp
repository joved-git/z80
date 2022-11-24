#include <iostream>
#include <cstring>

#include "../inc/version.hpp"

#include "../inc/common_values.h"
#include "../inc/Z80Machine.h"

int main(int argc, char *argv[])
{
	/* Variables definition	*/
    bool exit=false;
    char c=0;
	char command[MAX_LEN];
	uint16_t i=0;
	char instruction[MAX_OP_LENGTH*3];
	uint32_t machineCode=0;
	uint8_t len=0;
	Z80Machine machine;
	
	/* Init the values	*/
	command[0]='\0';

	/* Display the invitational message	*/
    std::cout << std::endl << "Hello Z80 world !" << std::endl;
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
		machine.setEntry(command);
		exit=machine.analyse();
		//printf("*** 2");
		//newCommand.getResult();
		//
		//cmd=newCommand.getInstruction();
		//attr1=newCommand.getFirstAttribute();
		//attr2=newCommand.getSecondAttribute();

		if (!exit) 
		{
			if (machine.getExecutionMode())
			{
				machine.displayExecRegisters();
				machine.displayMemory("(PC)");
				machineCode=machine.getNextInstruction(instruction, &len);

				std::cout << std::endl;

				switch (len)
				{
					case ONE_BYTE:
						printf("z [%02X] [%s]> ", machineCode, instruction);
						break;

					case TWO_BYTES:
						printf("z [%04X] [%s]> ", machineCode, instruction);
						break;

					case THREE_BYTES:
						printf("z [%06X] [%s]> ", machineCode, instruction);
						break;

					case FOUR_BYTES:
						printf("z [%08X] [%s]> ", machineCode, instruction);
						break;
				}
			}
			else
			{
				std::cout << std::endl << "z> ";
			}
				
		}
    }

	std::cout << std::endl << "Bye, see you next time !!" << std::endl  << std::endl;

    return 0;
}
