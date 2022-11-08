#include "../inc/Command.h"

/* The constructor  */
Command::Command()
{
    mCommandIsEntered=FALSE;
    mEntry=NULL;
}

/* The destructor  */
Command::~Command()
{

}

/* Internal method to find the entry type   */
typeOfEntry Command::findEntryType(char *pEntry)
{
    /* The default type is COMMAND  */
    typeOfEntry type=NOTHING;

    if (strlen(pEntry)==1)
    {
        type=COMMAND;
    }

    return type;
}

/* Give a command to be analysed    */
void Command::setEntry(char *pEntry)
{
    mEntry=pEntry;

    printf("len=%d, p=%d", strlen(mEntry), mEntry);

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
        printf(">>> analyse [%s] <<<\n", mEntry);

        switch(findEntryType(mEntry))
        {
            case COMMAND:
                switch (mEntry[0]) {
                /* I have to exit	*/
                case 'x':
                    retValue=TRUE;
                    break;
            
                /* OK, display help	*/
                case 'h':
                    printf("\na <code>: translate <code> to assembly langage\n");
                    printf("  Example: ld c,b gives 0x41\n");
                    printf("m <cmd>: translate <cmd> in machine code\n");
                    printf("  Example: cb22 gives sla d\n");
                    printf("r: display main registers\n");
                    printf("R: display all registers\n");
                    printf("x: exit me\n");
                    printf("\n");
                    printf("<cmd>: execute the command\n");
                    printf("<code>: execute the code\n");
                    break;

                /* Display the prompt after a line	*/
               
                /* Display registers	*/
                case 'r':
                    printf("--- Registers\n");
                    /*
                    printf("B: [%02X]    C: [%02X]\n", regB.getValue(), regC.getValue());
                    printf("E: [%02X]    E: [%02X]\n", regD.getValue(), regE.getValue());
                    printf("H: [%02X]    L: [%02X]\n", regH.getValue(), regL.getValue());
                    printf("A: [%02X]    F: [%02X] [%s]\n", regA.getValue(), regF.getValue(), byteToBinary(regF.getValue()));
                    */

                break;
            }
            break;
        }
    }

    return retValue;
}
