#include "../inc/Command.h"

/* The constructor  */
Command::Command()
{
    mCommandIsEntered=FALSE;
    mEntry=NULL;

    mRegisterPack.regB.setValue(0x01);
	mRegisterPack.regC.setValue(0xAA);
	mRegisterPack.regC.setValue(0xAA);
	mRegisterPack.regD.setValue(0x0C);
	mRegisterPack.regE.setValue(0x10);
	mRegisterPack.regL.setValue(0xC8);
	mRegisterPack.regF.setValue(0b01101111);
}

/* The destructor  */
Command::~Command()
{
    
}

/* Byte to binay function	*/
const char *Command::byteToBinary(uint8_t x)
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

/* Verify if the entry is an hexa number    */
bool Command::isACode()
{
    bool code=true;
    bool cont=TRUE;
    uint16_t i=0;

    if (strlen(mEntry)<=8)
    {
        while (cont)
        {
            if (mEntry[i]=='\0')
            {
                cont=FALSE;
            }
            else 
            {
                if (!((mEntry[i]>='A' && mEntry[i]<='F') || (mEntry[i]>='0' && mEntry[i]<='9')))
                {
                    code=FALSE;
                    cont=FALSE;
                }
            }

            i++;
        }
    }

    return code;
}

/* Internal method to find the entry type   */
typeOfEntry Command::findEntryType()
{
    /* The default type is COMMAND  */
    typeOfEntry type=NOTHING;

    if (strlen(mEntry)==1)
    {
        type=COMMAND;
    }
    else 
    {
        /* Convert string into uppercase    */
        for (int i = 0; mEntry[i]!='\0'; i++) 
        {
            if(mEntry[i]>='a' && mEntry[i]<='z') 
            {
                mEntry[i]-=32;
            }
        }

        if (isACode())
        {
            type=CODE;
        }
        else
        {
            type=INSTRUCTION;
        }
    }   

    return type;
}

/* Give a command to be analysed    */
void Command::setEntry(char *pEntry)
{
    mEntry=pEntry;

    //printf("len=%d, p=%s", strlen(mEntry), mEntry);

    if (strlen(mEntry) && mEntry) 
    {
        mCommandIsEntered=TRUE;
    }
    else
    {
        mCommandIsEntered=FALSE;
    }
}

// Analyse the command  */
bool Command::analyse()
{
    typeOfEntry type;
    bool retValue=FALSE;

    if (mCommandIsEntered)
    {
        //printf(">>> analyse [%s] <<<\n", mEntry);

        switch(findEntryType())
        {
            case COMMAND:
                switch (mEntry[0]) {

                /* I have to exit	*/
                case 'x':
                    retValue=TRUE;
                    break;
            
                /* OK, display help	*/
                case 'h':
                    printf("\na <code>\ttranslate <code> to assembly langage.\n");
                    printf("\t\tExample: ld c,b gives 0x41\n");
                    printf("m <cmd>\t\ttranslate <cmd> in machine code.\n");
                    printf("\t\tExample: cb22 gives sla d\n");
                    printf("r\t\tdisplay main registers.\n");
                    printf("R\t\tdisplay all registers.\n");
                    printf("x\t\texit me.\n");
                    printf("\n");
                    printf("<cmd>\t\texecute the command.\n");
                    printf("<code>\t\texecute the code.\n");
                    break;
          
                /* Display registers	*/
                case 'r':
                    //printf("--- Registers ---\n\n");
                    printf("\n");
                    printf("B: [%02X]    C: [%02X]\n", mRegisterPack.regB.getValue(), mRegisterPack.regC.getValue());
                    printf("D: [%02X]    E: [%02X]\n", mRegisterPack.regD.getValue(), mRegisterPack.regE.getValue());
                    printf("H: [%02X]    L: [%02X]\n", mRegisterPack.regH.getValue(), mRegisterPack.regL.getValue());
                    printf("A: [%02X]    F: [%02X] [%s]\n", mRegisterPack.regA.getValue(), mRegisterPack.regF.getValue(), 
                        byteToBinary(mRegisterPack.regF.getValue()));
                break;
            }
            break;

            case CODE:
                if (!strcmp(mEntry, "00"))
                {
                    printf("\n[00] is NOP\n");
                }
                break;
        }
    }
    
    return retValue;
}
