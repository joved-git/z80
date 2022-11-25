#include "../inc/Z80Machine.h"
#include "../inc/instruction_length.h"

/* The constructor  */
Z80Machine::Z80Machine()
{
    mCommandIsEntered=false;
    mEntry=NULL;

    /* Define the type of the 16-bit registers  */
    mRegisterPack.regBC.set16bitsRegisterType(HALF);
    mRegisterPack.regBC.setHightLowRegister(&mRegisterPack.regB, &mRegisterPack.regC);
    mRegisterPack.regDE.set16bitsRegisterType(HALF);
    mRegisterPack.regDE.setHightLowRegister(&mRegisterPack.regD, &mRegisterPack.regE);
    mRegisterPack.regHL.set16bitsRegisterType(HALF);
    mRegisterPack.regHL.setHightLowRegister(&mRegisterPack.regH, &mRegisterPack.regL);
    mRegisterPack.regAF.set16bitsRegisterType(HALF);
    mRegisterPack.regAF.setHightLowRegister(&mRegisterPack.regA, &mRegisterPack.regF);

    mRegisterPack.regSP.set16bitsRegisterType(FULL);
    mRegisterPack.regPC.set16bitsRegisterType(FULL);
    mRegisterPack.regIX.set16bitsRegisterType(FULL);
    mRegisterPack.regIY.set16bitsRegisterType(FULL);

    mRegisterPack.regAFp.set16bitsRegisterType(FULL);
    mRegisterPack.regBCp.set16bitsRegisterType(FULL);
    mRegisterPack.regDEp.set16bitsRegisterType(FULL);
    mRegisterPack.regHLp.set16bitsRegisterType(FULL);

    /* Initialize registers to 0 exept SP  */
    mRegisterPack.regA.setValue(0x00);
    mRegisterPack.regB.setValue(0x00);
	mRegisterPack.regC.setValue(0x00);
	mRegisterPack.regD.setValue(0x00);
	mRegisterPack.regE.setValue(0x00);
	mRegisterPack.regL.setValue(0x00);
	mRegisterPack.regF.setValue(0b00000000);

    mRegisterPack.regPC.setValue(0x0000);
    mRegisterPack.regIX.setValue(0x0000);
    mRegisterPack.regIY.setValue(0x0000);
    mRegisterPack.regSP.setValue(INITIAL_STACK_POINTER);

    mRegisterPack.regAFp.setValue(0x0000);
    mRegisterPack.regBCp.setValue(0x0000);
    mRegisterPack.regDEp.setValue(0x0000);
    mRegisterPack.regHLp.setValue(0x0000);

     /* Initialize some registers (for test)   */
    mRegisterPack.regB.setValue(0x01);
	mRegisterPack.regC.setValue(0xAA);
	mRegisterPack.regD.setValue(0x0C);
	mRegisterPack.regE.setValue(0x10);
	mRegisterPack.regL.setValue(0xC8);
	//mRegisterPack.regF.setValue(0b01101111);
    mRegisterPack.regPC.setValue(0x1234);
    mRegisterPack.regHL.setValue(0xFE14);
    mRegisterPack.regIX.setValue(0x1200);
    mRegisterPack.regIY.setValue(0x1400);

    /* Set the default mode     */
    mExecMode=false;

    /* Creating the memory      */
    mMemory=new(Memory);
}

/* The destructor  */
Z80Machine::~Z80Machine()
{
    /* Destroying the memory    */
    delete(mMemory);
}

/* Byte to binary function	*/
const char *Z80Machine::byteToBinary(uint8_t x)
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

/* Register to bit converter  */
uint8_t Z80Machine::registerToBit(char *pRegister)
{
    char reg='0';
    uint8_t retBit=0;

    if (strlen(pRegister) == 1)         /* This is a 8-bit register     */
    {
        reg=pRegister[0];               /* The register as a character  */
        
        switch (reg)
        {
            case 'A':
                retBit=REGA;
                break;
            
            case 'B':
                retBit=REGB;
                break;

            case 'C':
                retBit=REGC;
                break;

            case 'D':
                retBit=REGD;
                break;

            case 'E':
                retBit=REGE;
                break;

            case 'H':
                retBit=REGH;
                break;

            case 'L':
                retBit=REGL;
                break;
        }
    }

    if (strlen(pRegister) == 2)     /* This is a 16-bit register     */
    {
        if (!strcmp(pRegister, "BC"))
        {
            retBit=(uint8_t) (REGBC) & 0b00000011;
        }

        if (!strcmp(pRegister, "DE"))
        {
            retBit=(uint8_t) (REGDE) & 0b00000011;
        }

        if (!strcmp(pRegister, "HL"))
        {
            retBit=(uint8_t) (REGHL) & 0b00000011;
        }

        if (!strcmp(pRegister, "SP"))
        {
            retBit=(uint8_t) (REGSP) & 0b00000011;
        }
    }

    return retBit;

}

/* Bit to register converter     */
uint8_t Z80Machine::bitToRegister(uint8_t pBit, char *pRetChar)
{
    uint8_t ret=0;

    switch (pBit)
    {
        case REGA:
            strcpy(pRetChar, STRING_REGA);
            break;

        case REGB:
            strcpy(pRetChar, STRING_REGB);
            break;
        
        case REGC:
            strcpy(pRetChar, STRING_REGC);
            break;
        
        case REGD:
            strcpy(pRetChar, STRING_REGD);
            break;

        case REGE:
            strcpy(pRetChar, STRING_REGE);
            break;

        case REGH:
            strcpy(pRetChar, STRING_REGH);
            break;

        case REGL:
            strcpy(pRetChar, STRING_REGL);
            break;

        case REGBC:
            strcpy(pRetChar, STRING_REGBC);
            break;

        case REGIBC:
            strcpy(pRetChar, STRING_REGIBC);
            break;

        case REGDE:
            strcpy(pRetChar, STRING_REGDE);
            break;

        case REGIDE:
            strcpy(pRetChar, STRING_REGIDE);
            break;

        case REGHL:
            strcpy(pRetChar, STRING_REGHL);
            break;

        case REGIHL:
            strcpy(pRetChar, STRING_REGIHL);
            break;

        case REGSP:
            strcpy(pRetChar, STRING_REGSP);
            break;

        case REGAF:
            strcpy(pRetChar, STRING_REGAF);
            break;

        case REGISP:
            strcpy(pRetChar, STRING_REGISP);
            break;

        default:
            strcpy(pRetChar, STRING_REG_UNDEFINED);
            ret=ERR_NO_REGISTER;
    }

    return ret;
}


/* Change lowercase to uppercase into the entry */
void Z80Machine::toUpper(char *pEntry)
{
        for (int i = 0; pEntry[i]!='\0'; i++) 
        {
            if(pEntry[i]>='a' && pEntry[i]<='z') 
            {
                pEntry[i]-=32;
            }
        }
}

/* Verify if the entry is an hexa number    */
bool Z80Machine::isACode()
{
    bool code=true;
    bool cont=true;
    uint16_t i=0;

    if (strlen(mEntry)<=8)
    {
        while (cont)
        {
            if (mEntry[i]=='\0')
            {
                cont=false;
            }
            else 
            {
                if (!((mEntry[i]>='A' && mEntry[i]<='F') || (mEntry[i]>='0' && mEntry[i]<='9')))
                {
                    code=false;
                    cont=false;
                }
            }

            i++;
        }
    }
    else
    {
        code=false;
    }

    return code;
}


/* This method dumps 16 bytes of memory from the given address. */
void Z80Machine::dumpMemory(uint16_t pAddress)
{
    uint16_t dumpToView=16;

    printf("\n[%04X]: ", pAddress);

    for (int i=0; i<dumpToView; i++)
    {
        printf("%02X", mMemory->get8bitsValue(pAddress+i));

        if (i<dumpToView-1)
        {
            printf(".");
        }
    }

    printf("\n");
}

/* Internal method to find the entry type   */
typeOfEntry Z80Machine::findEntryType()
{
    /* The default type is COMMAND  */
    typeOfEntry type=NOTHING;

    if (strlen(mEntry)==1 || (strlen(mEntry)>1 && mEntry[1]==' '))
    {
        type=COMMAND;

        /* Convert string into uppercase but not the first car of the command   */
        toUpper(mEntry+1);
    }
    else 
    {
        /* Convert string into uppercase    */
        toUpper(mEntry);
        
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
void Z80Machine::setEntry(char *pEntry)
{
    mEntry=pEntry;

    //printf("len=%d, p=%s", strlen(mEntry), mEntry);

    if (strlen(mEntry) && mEntry) 
    {
        mCommandIsEntered=true;
    }
    else
    {
        mCommandIsEntered=false;
    }
}


/* Transform the instruction into real number  */
uint32_t Z80Machine::toValue(char *pCode, uint8_t *pLen, uint8_t *pLenEffective)
{
    uint32_t hexaValue=0;
    uint8_t i=0;

    *pLen=strlen(pCode);
    *pLenEffective=*pLen;

    //printf("c0=%1X val=%1X\n", pCode[0], (pCode[0]>'9'?pCode[0]-55:pCode[0]-'0'));
    //printf("c1=%1X val=%1X\n", pCode[1], (pCode[1]>'9'?pCode[1]-55:pCode[1]-'0'));

    while (i<*pLen)
    {
        if (pCode[i]<'0' || pCode[i]>'F' || (pCode[i]>'9' && pCode[i]<'A'))
        {
            *pLenEffective=i;
            i=*pLen;
        }
        else
        {
            hexaValue=hexaValue*0x10+(pCode[i]>'9'?pCode[i]-55:pCode[i]-'0');
        }

        i++;
    }

    /*
    for (int i=0; i<*pLen; i++)
    {
        hexaValue=hexaValue*0x10+(pCode[i]>'9'?pCode[i]-55:pCode[i]-'0');
        //printf("c=%c\n", pCode[i]);
        //printf("th=<%08X>\n", hexaValue);
    }
    */

    return hexaValue;
}      


/* Transform the instruction into hex number  */
int32_t Z80Machine::toDec(char *pCode)
{
    int64_t decValue=0;
    uint8_t i=0;

    while (i<strlen(pCode))
    {
        if (pCode[i]>'9' || pCode[i]<'0')
        {
            decValue=-1;
            i=10;
        }
        else
        {
            decValue=decValue*10+(pCode[i]-'0');
        }

        i++;
    }

    return decValue;
}      

/* Give the address of a register defined by its binary code    */
Register_8bits *Z80Machine::get8bitsRegisterAddress(uint8_t pReg)
{
    Register_8bits *regReturn;

    switch (pReg)
    {
        case REGA:
            regReturn=&(mRegisterPack.regA);
            break;

        case REGB:
            regReturn=&(mRegisterPack.regB);
            break;
        
        case REGC:
            regReturn=&(mRegisterPack.regC);
            break;

        case REGD:
            regReturn=&(mRegisterPack.regD);
            break;
        
        case REGE:
            regReturn=&(mRegisterPack.regE);
            break;
        
        case REGH:
            regReturn=&(mRegisterPack.regH);
            break;

        case REGL:
            regReturn=&(mRegisterPack.regL);
            break;
    }

    return(regReturn);
}

/* Give the address of a register defined by its binary code    */
Register_16bits *Z80Machine::get16bitsRegisterAddress(uint8_t pReg)
{
    Register_16bits *regReturn;

    switch (pReg)
    {
        case REGAF:
            regReturn=&(mRegisterPack.regAF);
            break;

        case REGBC:
            regReturn=&(mRegisterPack.regBC);
            break;
        
        case REGDE:
            regReturn=&(mRegisterPack.regDE);
            break;
        
        case REGHL:
            regReturn=&(mRegisterPack.regHL);
            break;

        case REGSP:
            regReturn=&(mRegisterPack.regSP);
            break;

        case REGIX:
            regReturn=&(mRegisterPack.regIX);
            break;

        case REGIY:
            regReturn=&(mRegisterPack.regIY);
            break;
    }

    return(regReturn);
}


/* Clean the n operand    */
int8_t Z80Machine::clean_n(char *pOp)
{
    uint8_t retCode=ERR_NO_ERROR;
    char *posChar;

    //printf("op_in =<%s>\n", pOp);

    if (posChar=strchr(pOp, '#'))               /* Is there a '#' ? */
    {
        if (strlen(pOp)>3)                     /* Remove characters if needed  */
        {
            pOp[3]='\0';
        }

        if (posChar!=pOp)
        {
            printf("The # is not at the first position...\n");
            retCode=ERR_BAD_OPERAND;
        }
        else
        {
            if (!strcmp(pOp, "#"))
            {
                printf("The operand is just '#'...\n");
                retCode=ERR_BAD_OPERAND;
            }
            else
            {
                if (strlen(pOp)==2)
                {
                    pOp[3]='\0';            /* Add a zero before */
                    pOp[2]=pOp[1];
                    pOp[1]='0';
                }
            }
        }
    }

    //printf("op_out=<%s>\n", pOp);

    return retCode;
}


/* Clean the n operand    */
int8_t Z80Machine::clean_nn(char *pOp)
{
    uint8_t retCode=ERR_NO_ERROR;
    char *posChar;

    //printf("op_in =<%s>\n", pOp);

    if (posChar=strchr(pOp, '#'))               /* Is there a '#' ? */
    {
        if (strlen(pOp)>5)                     /* Remove characters if needed  */
        {
            pOp[5]='\0';
        }

        if (posChar!=pOp)
        {
            printf("The # is not at the first position...\n");
            retCode=ERR_BAD_OPERAND;
        }
        else
        {
            if (!strcmp(pOp, "#"))
            {
                printf("The operand is just '#'...\n");
                retCode=ERR_BAD_OPERAND;
            }
            else
            {
                if (strlen(pOp)==2)
                {
                    pOp[5]='\0';            /* Add a zero before */
                    pOp[4]=pOp[1];
                    pOp[3]='0';
                    pOp[2]='0';
                    pOp[1]='0';
                }

                if (strlen(pOp)==3)
                {
                    pOp[5]='\0';            /* Add a zero before */
                    pOp[4]=pOp[2];
                    pOp[3]=pOp[1];
                    pOp[2]='0';
                    pOp[1]='0';
                }

                if (strlen(pOp)==4)
                {
                    pOp[5]='\0';            /* Add a zero before */
                    pOp[4]=pOp[3];
                    pOp[3]=pOp[2];
                    pOp[2]=pOp[1];
                    pOp[1]='0';
                }

            }
        }
    }

    //printf("op_out=<%s>\n", pOp);

    return retCode;
}


/* Clean the (IX+#nn) and the (IY+#nn) operand */
int8_t Z80Machine::clean_ixn(char *pOp)
{
    uint8_t retCode=ERR_NO_ERROR;
    char *posChar;

    //printf("op_i=<%s>\n", pOp);

    if (posChar=strchr(pOp, '#'))               /* Is there a '#' ? */ 
    {
        strcpy(pOp, posChar);

        pOp[strlen(pOp)-1]='\0';
        
        if (strlen(pOp)>=3)                     /* Remove characters if needed  */
        {
            pOp[3]='\0';
        }

        if (pOp[0]!='#')
        {
            printf("The # is not at the first position...\n");
            retCode=ERR_BAD_OPERAND;
        }
        else
        {
            if (!strcmp(pOp, "#"))
            {
                printf("The operand is just '#'...\n");
                retCode=ERR_BAD_OPERAND;
            }
            else
            {
                if (strlen(pOp)==2)
                {
                    pOp[3]='\0';            /* Add a zero before */
                    pOp[2]=pOp[1];
                    pOp[1]='0';
                }
            }
        }              
    }
    else
    {
        retCode=ERR_BAD_OPERAND;
    }

    //printf("op_o=<%s>\n", pOp);
    //printf("rc=%d", retCode);

    return retCode;
}


/* Clean the r operand  (IX+#00)  */
int8_t Z80Machine::clean_r(char *)
{
    return 0;
}


/* Give the execution mode.     */
bool Z80Machine::getExecutionMode()
{
    return mExecMode;
}

/* Display registers            */
void Z80Machine::displaySimpleRegisters()
{
    printf("\n");
    //printf("[\033[31m31\033[0m][\033[32m32\033[0m][\033[33m33\033[0m][\033[34m34\033[0m][\033[35m35\033[0m]\n"); 
    printf("B  [%02X]      C  [%02X]\n", mRegisterPack.regB.getValue(), mRegisterPack.regC.getValue());
    printf("D  [%02X]      E  [%02X]\n", mRegisterPack.regD.getValue(), mRegisterPack.regE.getValue());
    printf("H  [%02X]      L  [%02X]\n", mRegisterPack.regH.getValue(), mRegisterPack.regL.getValue());
    printf("A  [%02X]      F  [%02X] [%s] [S:%d Z:%d H:%d PV:%d N:%d C:%d]\n", mRegisterPack.regA.getValue(), mRegisterPack.regF.getValue(), 
        byteToBinary(mRegisterPack.regF.getValue()), 
        mRegisterPack.regF.getSignFlag(), mRegisterPack.regF.getZeroFlag(), 
        mRegisterPack.regF.getHalfCarryFlag(), mRegisterPack.regF.getParityOverflowFlag(),
        mRegisterPack.regF.getAddSubtractFlag(), mRegisterPack.regF.getCarryFlag());

    printf("\n");
    printf("BC [%04X]    DE [%04X]\n", mRegisterPack.regBC.getValue(), mRegisterPack.regDE.getValue());
    printf("HL [%04X]    AF [%04X]\n", mRegisterPack.regHL.getValue(), mRegisterPack.regAF.getValue());
    printf("IX [%04X]    IY [%04X]\n", mRegisterPack.regIX.getValue(), mRegisterPack.regIY.getValue());

    printf("\n");
    printf("PC [%04X]    SP [%04X]\n", mRegisterPack.regPC.getValue(), mRegisterPack.regSP.getValue());
}

/* Display registers            */
void Z80Machine::displayAllRegisters()
{
    printf("\n");
    //printf("[\033[31m31\033[0m][\033[32m32\033[0m][\033[33m33\033[0m][\033[34m34\033[0m][\033[35m35\033[0m]\n"); 
    printf("B  [%02X]      C  [%02X]\n", mRegisterPack.regB.getValue(), mRegisterPack.regC.getValue());
    printf("D  [%02X]      E  [%02X]\n", mRegisterPack.regD.getValue(), mRegisterPack.regE.getValue());
    printf("H  [%02X]      L  [%02X]\n", mRegisterPack.regH.getValue(), mRegisterPack.regL.getValue());
    printf("A  [%02X]      F  [%02X] [%s] [S:%d Z:%d H:%d PV:%d N:%d C:%d]\n", mRegisterPack.regA.getValue(), mRegisterPack.regF.getValue(), 
        byteToBinary(mRegisterPack.regF.getValue()), 
        mRegisterPack.regF.getSignFlag(), mRegisterPack.regF.getZeroFlag(), 
        mRegisterPack.regF.getHalfCarryFlag(), mRegisterPack.regF.getParityOverflowFlag(),
        mRegisterPack.regF.getAddSubtractFlag(), mRegisterPack.regF.getCarryFlag());

    printf("\n");
    printf("BC [%04X]    DE [%04X]    BC' [%04X]    DE' [%04X]\n", 
        mRegisterPack.regBC.getValue(), mRegisterPack.regDE.getValue(), 
        mRegisterPack.regBCp.getValue(), mRegisterPack.regDEp.getValue());
    printf("HL [%04X]    AF [%04X]    HL' [%04X]    AF' [%04X]\n", 
        mRegisterPack.regHL.getValue(), mRegisterPack.regAF.getValue(),
        mRegisterPack.regHLp.getValue(), mRegisterPack.regAFp.getValue());
    printf("IX [%04X]    IY [%04X]\n", mRegisterPack.regIX.getValue(), mRegisterPack.regIY.getValue());

    printf("\n");
    printf("PC [%04X]    SP [%04X]\n", mRegisterPack.regPC.getValue(), mRegisterPack.regSP.getValue());
}


/* Display registers in exec mode           */
void Z80Machine::displayExecRegisters()
{
    printf("\n");
    printf("B:  [%02X]      C: [%02X]      D:  [%02X]      E: [%02X]      H:  [%02X]      L: [%02X]      A:  [%02X]      F: [%02X] [%s] [S:%d Z:%d H:%d PV:%d N:%d C:%d]\n", 
           mRegisterPack.regB.getValue(), mRegisterPack.regC.getValue(),
           mRegisterPack.regD.getValue(), mRegisterPack.regE.getValue(),
           mRegisterPack.regH.getValue(), mRegisterPack.regL.getValue(),
           mRegisterPack.regA.getValue(), mRegisterPack.regF.getValue(), 
            byteToBinary(mRegisterPack.regF.getValue()), mRegisterPack.regF.getSignFlag(), 
            mRegisterPack.regF.getZeroFlag(), mRegisterPack.regF.getHalfCarryFlag(), 
            mRegisterPack.regF.getParityOverflowFlag(), mRegisterPack.regF.getAddSubtractFlag(), 
            mRegisterPack.regF.getCarryFlag());

    printf("BC: [%04X]    DE [%04X]    HL: [%04X]    AF [%04X]    IX: [%04X]    IY [%04X]\n", 
        mRegisterPack.regBC.getValue(), mRegisterPack.regDE.getValue(),
        mRegisterPack.regHL.getValue(), mRegisterPack.regAF.getValue(),
        mRegisterPack.regIX.getValue(), mRegisterPack.regIY.getValue());

    printf("PC: [%04X]    SP [%04X]\n", mRegisterPack.regPC.getValue(), mRegisterPack.regSP.getValue());
}


/* Display memory from (PC)     */
void Z80Machine::displayMemory(const char *pAddress)
{
    uint8_t lenValue=0;
    uint8_t lenEff=0;
    uint32_t value=0;
    //mEntry+=2;
    lenValue=lenEff=0;
    char address[MAX_LEN];

    strcpy(address, pAddress);

    if (!strcmp(pAddress, "(PC)"))
    {
        value=mRegisterPack.regPC.getValue();
    }
    else
    {
        if (!strcmp(pAddress, "(SP)"))
        {
            value=mRegisterPack.regSP.getValue();
        }
        else
        {
            value=toValue(address, &lenValue, &lenEff);
        }
    }    

    if (lenEff<lenValue)
    {
        printf("\nNot a valid address\n"); 
    }
    else
    {
        dumpMemory(value);
    }
}


/* Give the next instruction as a string    */
char *Z80Machine::getInstruction()
{
    return mInstruction;
}


/* Interpret the machine code   */
uint8_t Z80Machine::interpretCode(uint32_t codeInHexa, uint8_t len, uint8_t pMode)
{
    //uint32_t codeInHexa;
    //uint8_t len=0;
    uint8_t lenEff=0;
    uint8_t op1=0;
    uint8_t op2=0;
    uint16_t op16=0;
    uint16_t instruction=CODE_NO_INSTRUCTION;
    Register_8bits *reg8_1=NULL;
    Register_8bits *reg8_2=NULL;
    Register_16bits *reg16_1=NULL;
    uint16_t address=0x0000;
    uint8_t newVal=0;
    uint8_t val=0;
    char strInstr[MAX_OP_LENGTH*3];

    uint8_t ret;
    char sop1[MAX_OP_LENGTH], sop2[MAX_OP_LENGTH];

    //printf("code=<%s>\n", pCode);
    //codeInHexa=toValue(pCode, &len, &lenEff);                     /* Transform the instruction into real number  */
    
    sprintf(mInstruction, " not yet decoded ");

    /* This is a NOP    */
    if ((codeInHexa & MASK_NOP)==CODE_NOP && len == ONE_BYTE)                  
    {
        instruction=CODE_NOP;
    }

    //printf("%02x %02x\n", (codeInHexa & MASK_LDRR), CODE_LDRR);     
    
    /* This is a LD r,r' */
    if ((codeInHexa & MASK_LDRR)==CODE_LDRR && len == ONE_BYTE)
    {
        instruction=CODE_LDRR;
               
        //printf("%s\n", byteToBinary(codeInHexa));

        /* Extract the value of the register (in bits)    */
        op1=EXTRACT(codeInHexa, 3, 3);
        op2=EXTRACT(codeInHexa, 0, 3);

        //ret=bitToRegister(op1, sop1);
        //ret=bitToRegister(op2, sop2);
        //printf("\n[%02X] is LD %s,%s\n", codeInHexa, sop1, sop2);
    }

    /* This is a LD (HL),r  */
    if ((codeInHexa & MASK_LDHLR)==CODE_LDHLR && len == ONE_BYTE)
    {
        instruction=CODE_LDHLR;
        
        /* Extract the value of the register (in bits)    */
        op1=REGIHL;
        op2=EXTRACT(codeInHexa, 0, 3);
    }

    /* This is a LD r,(HL)  */
    if ((codeInHexa & MASK_LDRHL)==CODE_LDRHL && len == ONE_BYTE)
    {
        instruction=CODE_LDRHL;
        
        /* Extract the value of the register (in bits)    */
        op1=EXTRACT(codeInHexa, 3, 3);
        op2=REGIHL;
    }

    /* This is a LD r,n  */
    if ((codeInHexa>>8 & MASK_LDRN)==CODE_LDRN && len == TWO_BYTES)
    {
        instruction=CODE_LDRN;
        
        /* Extract the value of the register (in bits)    */
        op1=EXTRACT(codeInHexa, 11, 3);
        op2=codeInHexa & 0xFF;

        //printf("cih=%02X (%d)", codeInHexa, codeInHexa);
        //printf("op1=%s\n", byteToBinary(op1));
        //printf("n=%d\n", op2);
    }

    /* This is a LD (HL),n  */
    if ((codeInHexa>>SIZE_1_BYTE & MASK_LDHLN)==CODE_LDHLN && len == TWO_BYTES)
    {
        instruction=CODE_LDHLN;
        
        /* Extract the value of the operand #2 (n)    */
        op2=codeInHexa & FIRST_LOWEST_BYTE;
    }

    /* This is a HALT    */
    if ((codeInHexa & MASK_HALT)==CODE_HALT && len == ONE_BYTE)                  
    {
        instruction=CODE_HALT;
    }

    /* This is a LD A,(BC) */
    if ((codeInHexa & MASK_LDABC)==CODE_LDABC && len == ONE_BYTE)
    {
        instruction=CODE_LDABC;
    }

    /* This is a LD A,(DE) */
    if ((codeInHexa & MASK_LDADE)==CODE_LDADE && len == ONE_BYTE)
    {
        instruction=CODE_LDADE;
    }

    /* This is a LD A,(nn)  */
    if ((codeInHexa>>SIZE_2_BYTES & MASK_LDANN)==CODE_LDANN && len == THREE_BYTES)
    {
        instruction=CODE_LDANN;
        
        /* Extract the value of the operand #2 (nn)    */
        op1=codeInHexa & FIRST_LOWEST_BYTE;
        op2=(codeInHexa & SECOND_LOWEST_BYTE) >> SIZE_1_BYTE;
    }

    /* This is a LD (nn),A  */
    if ((codeInHexa>>SIZE_2_BYTES & MASK_LDNNA)==CODE_LDNNA && len == THREE_BYTES)
    {
        instruction=CODE_LDNNA;
        
        /* Extract the value of the operand #2 (nn)    */
        op1=codeInHexa & FIRST_LOWEST_BYTE;
        op2=(codeInHexa & SECOND_LOWEST_BYTE) >> SIZE_1_BYTE;
    }

    /* This is a LD (BC),A  */
    if ((codeInHexa & MASK_LDBCA)==CODE_LDBCA && len == ONE_BYTE)
    {
        instruction=CODE_LDBCA;
        
    }

    /* This is a LD (DE),A  */
    if ((codeInHexa & MASK_LDDEA)==CODE_LDDEA && len == ONE_BYTE)
    {
        instruction=CODE_LDDEA;
        
    }

    /* This is a LD r,(IX+d)    */
    if ((codeInHexa>>SIZE_2_BYTES==ALT_CODE_DD) && ((codeInHexa & SECOND_LOWEST_BYTE)>>SIZE_1_BYTE & MASK_LDRIXD)==CODE_LDRIXD && len == THREE_BYTES)
    {
        instruction=CODE_DD_LDRIXD; 
        op1=EXTRACT(codeInHexa, 11, 3);
        op2=codeInHexa & FIRST_LOWEST_BYTE;
    }

    /* This is a LD rr,nn    */
    if ((codeInHexa>>SIZE_2_BYTES & MASK_LDRRNN)==CODE_LDRRNN && len == THREE_BYTES)
    {
        instruction=CODE_LDRRNN;
        
        /* Extract the value of the operand #1 (rr) and #2 (nn)    */
        op1=EXTRACT(codeInHexa, 20,2) | 0b1000;

        op16=((codeInHexa & FIRST_LOWEST_BYTE)<<SIZE_1_BYTE)+((codeInHexa & SECOND_LOWEST_BYTE)>>SIZE_1_BYTE);
    }

    /* This is a LD r,(IY+d)    */
    if ((codeInHexa>>SIZE_2_BYTES==ALT_CODE_FD) && ((codeInHexa & SECOND_LOWEST_BYTE)>>SIZE_1_BYTE & MASK_LDRIYD)==CODE_LDRIYD && len == THREE_BYTES)
    {
        instruction=CODE_FD_LDRIYD; 
        op1=EXTRACT(codeInHexa, 11, 3);
        op2=codeInHexa & FIRST_LOWEST_BYTE;
    }

    /* This is a LD (IX+d),r    */
    if ((codeInHexa>>SIZE_2_BYTES==ALT_CODE_DD) && ((codeInHexa & SECOND_LOWEST_BYTE)>>SIZE_1_BYTE & MASK_LDIXDR)==CODE_LDIXDR && len == THREE_BYTES)
    {
        instruction=CODE_DD_LDIXDR;
        op1=codeInHexa & FIRST_LOWEST_BYTE;
        op2=EXTRACT(codeInHexa, 8, 3);
    }

    /* This is a LD (IY+d),r    */
    if ((codeInHexa>>SIZE_2_BYTES==ALT_CODE_FD) && ((codeInHexa & SECOND_LOWEST_BYTE)>>SIZE_1_BYTE & MASK_LDIYDR)==CODE_LDIYDR && len == THREE_BYTES)
    {
        instruction=CODE_FD_LDIYDR; 
        op1=codeInHexa & FIRST_LOWEST_BYTE;
        op2=EXTRACT(codeInHexa, 8, 3);
    }

    /* This is a LD IX,nn    */
    if ((codeInHexa>>SIZE_3_BYTES==ALT_CODE_DD) && ((codeInHexa & THIRD_LOWEST_BYTE)>>SIZE_2_BYTES & MASK_LDIXNN)==CODE_LDIXNN && len == FOUR_BYTES)
    {
        instruction=CODE_DD_LDIXNN; 
        op16=((codeInHexa & FIRST_LOWEST_BYTE)<<SIZE_1_BYTE)+((codeInHexa & SECOND_LOWEST_BYTE)>>SIZE_1_BYTE);
    }

    /* This is a LD IY,nn    */
    if ((codeInHexa>>SIZE_3_BYTES==ALT_CODE_FD) && ((codeInHexa & THIRD_LOWEST_BYTE)>>SIZE_2_BYTES & MASK_LDIYNN)==CODE_LDIYNN && len == FOUR_BYTES)
    {
        instruction=CODE_FD_LDIYNN; 
        op16=((codeInHexa & FIRST_LOWEST_BYTE)<<SIZE_1_BYTE)+((codeInHexa & SECOND_LOWEST_BYTE)>>SIZE_1_BYTE);
    }

    /* This is a LD dd,(nn)    */
    if ((codeInHexa>>SIZE_3_BYTES==ALT_CODE_ED) && ((codeInHexa & THIRD_LOWEST_BYTE)>>SIZE_2_BYTES & MASK_LDDDNN)==CODE_LDDDNN && len == FOUR_BYTES)
    {
        instruction=CODE_ED_LDDDNN; 
        op1=EXTRACT(codeInHexa, 20, 2) | 0b1000;
        op16=((codeInHexa & FIRST_LOWEST_BYTE)<<SIZE_1_BYTE)+((codeInHexa & SECOND_LOWEST_BYTE)>>SIZE_1_BYTE);
    }

    /* This is a LD HL,(nn)    */
    if ((codeInHexa>>SIZE_2_BYTES & MASK_LDHLNN)==CODE_LDHLNN && len == THREE_BYTES)
    {
        instruction=CODE_LDHLNN; 
        op16=((codeInHexa & FIRST_LOWEST_BYTE)<<SIZE_1_BYTE)+((codeInHexa & SECOND_LOWEST_BYTE)>>SIZE_1_BYTE);
    }

    /* xxxjoexxx revoir les len== ci-dessus ^ */

    /* This is a LD IX,(nn)    */
    if ((codeInHexa>>SIZE_3_BYTES==ALT_CODE_DD) && ((codeInHexa & THIRD_LOWEST_BYTE)>>SIZE_2_BYTES & MASK_LDIXANN)==CODE_LDIXANN && len == DD_CODE_LENGTH(CODE_LDIXANN))
    {
        instruction=CODE_DD_LDIXANN; 
        op16=((codeInHexa & FIRST_LOWEST_BYTE)<<SIZE_1_BYTE)+((codeInHexa & SECOND_LOWEST_BYTE)>>SIZE_1_BYTE);
    }

    /* This is a LD IY,(nn)    */
    if ((codeInHexa>>SIZE_3_BYTES==ALT_CODE_FD) && ((codeInHexa & THIRD_LOWEST_BYTE)>>SIZE_2_BYTES & MASK_LDIYANN)==CODE_LDIYANN && len == FD_CODE_LENGTH(CODE_LDIYANN))
    {
        instruction=CODE_FD_LDIYANN; 
        op16=((codeInHexa & FIRST_LOWEST_BYTE)<<SIZE_1_BYTE)+((codeInHexa & SECOND_LOWEST_BYTE)>>SIZE_1_BYTE);
    }

    /* This is a (nn),HL    */
    if ((codeInHexa>>SIZE_2_BYTES & MASK_LDNNHL)==CODE_LDNNHL && len == NATURAL_CODE_LENGTH(CODE_LDNNHL))
    {
        instruction=CODE_LDNNHL; 
        op16=((codeInHexa & FIRST_LOWEST_BYTE)<<SIZE_1_BYTE)+((codeInHexa & SECOND_LOWEST_BYTE)>>SIZE_1_BYTE);
    }

    /* This is a INC r */
    if ((codeInHexa & MASK_INCR)==CODE_INCR && len == NATURAL_CODE_LENGTH(CODE_INCR))
    {
        instruction=CODE_INCR;
               
        op1=EXTRACT(codeInHexa, 3, 3);
    }

    /* This is a INC rr */
    if ((codeInHexa & MASK_INCRR)==CODE_INCRR && len == NATURAL_CODE_LENGTH(CODE_INCRR))
    {
        instruction=CODE_INCRR;
               
        op1=EXTRACT(codeInHexa, 4, 2) | 0b1000;;
    }

    /* This is a DEC r */
    if ((codeInHexa & MASK_DECR)==CODE_DECR && len == NATURAL_CODE_LENGTH(CODE_DECR))
    {
        instruction=CODE_DECR;
               
        op1=EXTRACT(codeInHexa, 3, 3);
    }

    /* This is a PUSH rr */
    if ((codeInHexa & MASK_PUSHQQ)==CODE_PUSHQQ && len == NATURAL_CODE_LENGTH(CODE_PUSHQQ))
    {
        instruction=CODE_PUSHQQ;
               
        op1=EXTRACT(codeInHexa, 4, 2) | 0b1000;

        if (op1==REGSP)                          /* This is the case of qq operand       */
        {                                           
            op1=REGAF;
        }
    }

    /* This is a (nn),rr    */
    if (((codeInHexa>>SIZE_2_BYTES) & MASK_LDNNRR)==CODE_LDNNRR && len==ED_CODE_LENGTH(CODE_LDNNRR))
    {
        instruction=CODE_ED_LDNNRR; 

        op1=EXTRACT(codeInHexa, 20, 2) | 0b1000;
        op16=((codeInHexa & FIRST_LOWEST_BYTE)<<SIZE_1_BYTE)+((codeInHexa & SECOND_LOWEST_BYTE)>>SIZE_1_BYTE);
    }


    /* This is a RLCA  */
    if ((codeInHexa & MASK_RLCA)==CODE_RLCA && len==NATURAL_CODE_LENGTH(CODE_RLCA))
    {
        instruction=CODE_RLCA;
    }

    /* This is a RLC r  */
    if (((codeInHexa & FIRST_LOWEST_BYTE) & MASK_RLCR)==CODE_RLCR && len == CB_CODE_LENGTH(CODE_RLCR))
    {
        instruction=CODE_CB_RLCR;
        
        /* Extract the value of the register    */
        op1=EXTRACT(codeInHexa, 0,3);
    }

    /* This is a RLC (HL)  */
    if (((codeInHexa & FIRST_LOWEST_BYTE) & MASK_RLCHL)==CODE_RLCHL && len == CB_CODE_LENGTH(CODE_RLCHL))
    {
        instruction=CODE_CB_RLCHL;

    }


    /*************************************************************************************************************************/

    switch (instruction)
    {
        case CODE_NOP:                              /* This is a NOP    */
            sprintf(mInstruction, "NOP");

            if (pMode==INTP_EXECUTE)
            {
                printf("\n%s was executed\n", mInstruction);
            }
            
            if (pMode==INTP_DISPLAY)
            {
                printf("\n[%02X] is %s\n", codeInHexa, mInstruction);
            }

            break;

        case CODE_HALT:                              /* This is a HALT    */
            sprintf(mInstruction, "HALT");

            if (pMode==INTP_EXECUTE || pMode==INTP_EXECUTE_BLIND)
            {
                printf("\n%s was executed\n", mInstruction);
                /* Don't forget to program the HALT command */
            }
            
            if (pMode==INTP_DISPLAY)
            {
                printf("\n[%02X] is %s\n", codeInHexa, mInstruction);

            }

            break;

        case CODE_LDRR:                             /* This is a LD r,r'    */
            ret=bitToRegister(op1, sop1);
            ret=bitToRegister(op2, sop2);

            sprintf(mInstruction, "LD %s,%s", sop1, sop2);

            if (pMode==INTP_EXECUTE || pMode==INTP_EXECUTE_BLIND)
            {
                reg8_1=get8bitsRegisterAddress(op1);
                reg8_2=get8bitsRegisterAddress(op2);

                reg8_1->setValue(reg8_2->getValue());

                if (pMode==INTP_EXECUTE)
                {
                    printf("\n%s was executed\n", mInstruction);
                }
            }
            
            if (pMode==INTP_DISPLAY)
            {
                printf("\n[%02X] is %s\n", codeInHexa, mInstruction);
            }

            break;

        case CODE_LDRN:                             /* This is a LD r,n    */
            ret=bitToRegister(op1, sop1);

            sprintf(mInstruction, "LD %s,#%02X", sop1, op2);
            
            if (pMode==INTP_EXECUTE || pMode==INTP_EXECUTE_BLIND)
            {
                reg8_1=get8bitsRegisterAddress(op1);
                reg8_1->setValue(op2);

                if (pMode==INTP_EXECUTE)
                {
                    printf("\n%s was executed\n", mInstruction);
                }
            }
            
            if (pMode==INTP_DISPLAY)
            {
                printf("\n[%04X] is %s\n", codeInHexa, mInstruction);
            }
            break;
        
        case CODE_LDRHL:                            /* This is a LD r,(HL)    */
            ret=bitToRegister(op1, sop1);
            ret=bitToRegister(op2, sop2);

            sprintf(mInstruction, "LD %s,(HL)", sop1);

            if (pMode==INTP_EXECUTE || pMode==INTP_EXECUTE_BLIND)
            {
                reg8_1=get8bitsRegisterAddress(op1);
                reg16_1=get16bitsRegisterAddress(REGHL);
                
                reg8_1->setValue(mMemory->get8bitsValue(reg16_1->getValue()));

                if (pMode==INTP_EXECUTE)
                {
                    printf("\n%s was executed\n", mInstruction);
                }
            }
            
            if (pMode==INTP_DISPLAY)
            {
                printf("\n[%02X] is %s\n", codeInHexa, mInstruction);
            }
            break;

        case CODE_LDHLR:                            /* This is a LD (HL),r    */   
            ret=bitToRegister(op1, sop1);
            ret=bitToRegister(op2, sop2);

            sprintf(mInstruction, "LD (HL),%s", sop2);

            if (pMode==INTP_EXECUTE || pMode==INTP_EXECUTE_BLIND)
            {
                reg8_1=get8bitsRegisterAddress(op2);
                reg16_1=get16bitsRegisterAddress(REGHL);

                mMemory->set8bitsValue(reg16_1->getValue(), reg8_1->getValue());

                if (pMode==INTP_EXECUTE)
                {
                    printf("\n%s was executed\n", mInstruction);
                }
            }
            
            if (pMode==INTP_DISPLAY)
            {
                printf("\n[%02X] is %s\n", codeInHexa, mInstruction);
            }
            break;

        case CODE_LDHLN:                            /* This is a LD (HL),n   */   
            sprintf(mInstruction, "LD (HL),#%02X", op2);

            if (pMode==INTP_EXECUTE || pMode==INTP_EXECUTE_BLIND)
            {
                ret=bitToRegister(REGIHL, sop1);

                reg16_1=get16bitsRegisterAddress(REGHL);
                mMemory->set8bitsValue(reg16_1->getValue(), op2);

                if (pMode==INTP_EXECUTE)
                {
                    printf("\n%s was executed\n", mInstruction);
                }
            }
            
            if (pMode==INTP_DISPLAY)
            {
                ret=bitToRegister(REGIHL, sop1);
                
                printf("\n[%02X] is %s\n", codeInHexa, mInstruction);
            }
            break;

        case CODE_LDABC:                            /* This is a LD A,(BC)    */   
            sprintf(mInstruction, "LD A,(BC)");

            if (pMode==INTP_EXECUTE || pMode==INTP_EXECUTE_BLIND)
            {
                reg8_1=get8bitsRegisterAddress(REGA);
                reg16_1=get16bitsRegisterAddress(REGBC);

                reg8_1->setValue(mMemory->get8bitsValue(reg16_1->getValue()));

                if (pMode==INTP_EXECUTE)
                {
                    printf("\n%s was executed\n", mInstruction);
                }
            }
            
            if (pMode==INTP_DISPLAY)
            {
                ret=bitToRegister(op1, sop1);
                ret=bitToRegister(op2, sop2);

                printf("\n[%02X] is %s\n", codeInHexa, mInstruction);
            }
            break;

        case CODE_LDADE:                            /* This is a LD A,(DE)    */  
            sprintf(mInstruction, "LD A,(DE)");

            if (pMode==INTP_EXECUTE || pMode==INTP_EXECUTE_BLIND)
            {
                reg8_1=get8bitsRegisterAddress(REGA);
                reg16_1=get16bitsRegisterAddress(REGDE);

                reg8_1->setValue(mMemory->get8bitsValue(reg16_1->getValue()));

                if (pMode==INTP_EXECUTE)
                {
                    printf("\n%s was executed\n", mInstruction);
                }
            }
            
            if (pMode==INTP_DISPLAY)
            {
                ret=bitToRegister(op1, sop1);
                ret=bitToRegister(op2, sop2);

                printf("\n[%02X] is %s\n", codeInHexa, mInstruction);
            }
            break;
        
        case CODE_LDANN:                                    /* This is a LD A,(nn)    */   
            address=op1*0x100+op2;
            sprintf(mInstruction, "LD A,(#%04X)", address);
            
            if (pMode==INTP_EXECUTE || pMode==INTP_EXECUTE_BLIND)
            {
                reg8_1=get8bitsRegisterAddress(REGA);
                reg8_1->setValue(mMemory->get8bitsValue(address));

                if (pMode==INTP_EXECUTE)
                {
                    printf("\n%s was executed\n", mInstruction);
                }
            }
            
            if (pMode==INTP_DISPLAY)
            {
                printf("\n[%06X] is %s\n", codeInHexa, mInstruction);
            }
            break;

        case CODE_LDBCA:                            /* This is a LD (BC),A    */  
            sprintf(mInstruction, "LD (BC),A");
 
            if (pMode==INTP_EXECUTE || pMode==INTP_EXECUTE_BLIND)
            {
                reg16_1=get16bitsRegisterAddress(REGBC);
                reg8_2=get8bitsRegisterAddress(REGA);
                mMemory->set8bitsValue(reg16_1->getValue(), reg8_2->getValue());

                if (pMode==INTP_EXECUTE)
                {
                    printf("\n%s was executed\n", mInstruction);
                }
            }
            
            if (pMode==INTP_DISPLAY)
            {
                printf("\n[%02X] is %s\n", codeInHexa, mInstruction);
            }
            break;

        case CODE_LDDEA:                            /* This is a LD (DE),A    */   
            sprintf(mInstruction, "LD (DE),A");

            if (pMode==INTP_EXECUTE || pMode==INTP_EXECUTE_BLIND)
            {
                reg16_1=get16bitsRegisterAddress(REGDE);
                reg8_2=get8bitsRegisterAddress(REGA);
                mMemory->set8bitsValue(reg16_1->getValue(), reg8_2->getValue());

                if (pMode==INTP_EXECUTE)
                {
                    printf("\n%s was executed\n", mInstruction);
                }
            }
            
            if (pMode==INTP_DISPLAY)
            {
                printf("\n[%02X] is %s\n", codeInHexa, mInstruction);
            }
            break;

        case CODE_LDNNA:                                    /* This is a LD (nn),A    */   
            address=op1*0x100+op2;
            sprintf(mInstruction, "LD (#%04X),A", address);
            
            if (pMode==INTP_EXECUTE || pMode==INTP_EXECUTE_BLIND)
            {
                reg8_1=get8bitsRegisterAddress(REGA);
                mMemory->set8bitsValue(address, reg8_1->getValue());

                if (pMode==INTP_EXECUTE)
                {
                    printf("\n%s was executed\n", mInstruction);
                }
            }
            
            if (pMode==INTP_DISPLAY)
            {
                printf("\n[%06X] is %s\n", codeInHexa, mInstruction);
            }
            break;

        case CODE_DD_LDRIXD:                                    /* This is a LD r,(IX+d)   */
            ret=bitToRegister(op1, sop1);

            if (SIGN(op2))                                      /* Check if op2 is negative */
            {
                op2=~op2+1;
                address=mRegisterPack.regIX.getValue()-op2;
                sprintf(mInstruction,"LD %s,(IX-#%02X)", sop1, op2);
            }
            else
            {
                address=mRegisterPack.regIX.getValue()+op2;
                sprintf(mInstruction,"LD %s,(IX+#%02X)", sop1, op2);
            }
                        
            if (pMode==INTP_EXECUTE || pMode==INTP_EXECUTE_BLIND)
            {
                reg8_1=get8bitsRegisterAddress(op1);
                reg8_1->setValue(mMemory->get8bitsValue(address));

                if (pMode==INTP_EXECUTE)
                {
                    printf("\n%s was executed\n", mInstruction);
                }
            }
            
            if (pMode==INTP_DISPLAY)
            {
                printf("\n[%06X] is %s\n", codeInHexa, mInstruction);
            }
            break;

        case CODE_LDRRNN:                                    /* This is a LD rr,nn    */   
            ret=bitToRegister(op1, sop1);
            sprintf(mInstruction, "LD %s,#%04X", sop1, op16);
            
            if (pMode==INTP_EXECUTE || pMode==INTP_EXECUTE_BLIND)
            {
                reg16_1=get16bitsRegisterAddress(op1);
                reg16_1->setValue(op16);

                if (pMode==INTP_EXECUTE)
                {
                    printf("\n%s was executed\n", mInstruction);
                }
            }
            
            if (pMode==INTP_DISPLAY)
            {
                printf("\n[%06X] is %s\n", codeInHexa, mInstruction);
            }
            break;

        case CODE_FD_LDRIYD:                                    /* This is a LD r,(IY+d)   */
            ret=bitToRegister(op1, sop1);

            if (SIGN(op2))                                      /* Check if op2 is negative */
            {
                op2=~op2+1;
                address=mRegisterPack.regIX.getValue()-op2;
                sprintf(mInstruction,"LD %s,(IY-#%02X)", sop1, op2);
            }
            else
            {
                address=mRegisterPack.regIX.getValue()+op2;
                sprintf(mInstruction,"LD %s,(IY+#%02X)", sop1, op2);
            }
            
            if (pMode==INTP_EXECUTE || pMode==INTP_EXECUTE_BLIND)                                /* Execute LD r,(IY+d)*/
            {
                reg8_1=get8bitsRegisterAddress(op1);
                reg8_1->setValue(mMemory->get8bitsValue(address));

                if (pMode==INTP_EXECUTE)
                {
                    printf("\n%s was executed\n", mInstruction);
                }   
            }
            
            if (pMode==INTP_DISPLAY)
            {
                printf("\n[%06X] is %s\n", codeInHexa, mInstruction);
            }
            break;

        case CODE_DD_LDIXDR:                                        /* This is a LD (IX+d),r  */
            ret=bitToRegister(op2, sop2);
            
            if (SIGN(op1))                                          /* Check if op2 is negative */
            {
                op1=~op1+1;
                address=mRegisterPack.regIX.getValue()-op1;
                sprintf(mInstruction, "LD (IX-#%02X),%s", op1, sop2);
            }
            else
            {
                address=mRegisterPack.regIX.getValue()+op1;
                sprintf(mInstruction, "LD (IX+#%02X),%s", op1, sop2);
            }

            if (pMode==INTP_EXECUTE || pMode==INTP_EXECUTE_BLIND)                                /* Execute LD (IX+d),r  */
            {
                reg8_1=get8bitsRegisterAddress(op2);
                mMemory->set8bitsValue(address, reg8_1->getValue());
            
                if (pMode==INTP_EXECUTE)
                {
                    printf("\n%s was executed\n", mInstruction);
                }
            }
            
            if (pMode==INTP_DISPLAY)
            {
                printf("\n[%06X] is %s\n", codeInHexa, mInstruction);
            }
            break;

        case CODE_FD_LDIYDR:                                        /* This is a LD (IY+d),r  */
            ret=bitToRegister(op2, sop2);
            
            if (SIGN(op1))                                          /* Check if op2 is negative */
            {
                op1=~op1+1;
                address=mRegisterPack.regIX.getValue()-op1;
                sprintf(mInstruction, "LD (IY-#%02X),%s", op1, sop2);
            }
            else
            {
                address=mRegisterPack.regIX.getValue()+op1;
                sprintf(mInstruction, "LD (IY+#%02X),%s", op1, sop2);
            }
            if (pMode==INTP_EXECUTE || pMode==INTP_EXECUTE_BLIND)                                /* Execute LD (IY+d),r  */
            {
                reg8_1=get8bitsRegisterAddress(op2);
                mMemory->set8bitsValue(address, reg8_1->getValue());

                if (pMode==INTP_EXECUTE)
                {
                    printf("\n%s was executed\n", mInstruction);
                }
            }
            
            if (pMode==INTP_DISPLAY)
            {
                printf("\n[%06X] is %s\n", codeInHexa, mInstruction);
            }
            break;

        case CODE_DD_LDIXNN:                                    /* This is a LD IX,nn    */ 
            sprintf(mInstruction, "LD IX,#%04X", op16);

            if (pMode==INTP_EXECUTE || pMode==INTP_EXECUTE_BLIND)                               
            {
                reg16_1=get16bitsRegisterAddress(REGIX);
                reg16_1->setValue(op16);

                if (pMode==INTP_EXECUTE)
                {
                    printf("\n%s was executed\n", mInstruction);
                }
            }
            
            if (pMode==INTP_DISPLAY)
            {
                printf("\n[%08X] is %s\n", codeInHexa, mInstruction);
            }
            break;
    
        case CODE_FD_LDIYNN:                                    /* This is a LD IY,nn    */ 
            sprintf(mInstruction, "LD IY,#%04X", op16);

            if (pMode==INTP_EXECUTE || pMode==INTP_EXECUTE_BLIND)
            {
                reg16_1=get16bitsRegisterAddress(REGIY);
                reg16_1->setValue(op16);

                if (pMode==INTP_EXECUTE)
                {
                    printf("\n%s was executed\n", mInstruction);
                }
            }
            
            if (pMode==INTP_DISPLAY)
            {
                printf("\n[%08X] is %s\n", codeInHexa, mInstruction);
            }
            break;

        case CODE_ED_LDDDNN:                                    /* This is a LD rr,(nn)    */   
            ret=bitToRegister(op1, sop1);                      
            sprintf(mInstruction, "LD %s,(#%04X)", sop1, op16);

            if (pMode==INTP_EXECUTE || pMode==INTP_EXECUTE_BLIND)
            {
                reg16_1=get16bitsRegisterAddress(op1);
                reg16_1->setValue(mMemory->getAddress(op16));

                if (pMode==INTP_EXECUTE)
                {
                    printf("\n%s was executed\n", mInstruction);
                }
            }
            
            if (pMode==INTP_DISPLAY)
            {
                printf("\n[%08X] is %s\n", codeInHexa, mInstruction);
            }
            break;

        case CODE_LDHLNN:                                    /* This is a LD HL,(nn)    */   
            sprintf(mInstruction, "LD HL,(#%04X)", op16);
            if (pMode==INTP_EXECUTE || pMode==INTP_EXECUTE_BLIND)                         
            {
                reg16_1=get16bitsRegisterAddress(REGHL);
                reg16_1->setValue(mMemory->getAddress(op16));

                if (pMode==INTP_EXECUTE)
                {
                    printf("\n%s was executed\n", mInstruction);
                }
            }
            
            if (pMode==INTP_DISPLAY)
            {
                printf("\n[%06X] is %s\n", codeInHexa, mInstruction);
            }
            break;

        case CODE_DD_LDIXANN:                                   /* This is a LD IX,(nn)    */   
            sprintf(mInstruction, "LD IX,(#%04X)", op16);

            if (pMode==INTP_EXECUTE || pMode==INTP_EXECUTE_BLIND)                            
            {
                reg16_1=get16bitsRegisterAddress(REGIX);
                reg16_1->setValue(mMemory->getAddress(op16));

                if (pMode==INTP_EXECUTE)
                {
                    printf("\n%s was executed\n", mInstruction);
                }
            }
            
            if (pMode==INTP_DISPLAY)
            {
                printf("\n[%08X] is %s\n", codeInHexa, mInstruction);
            }
            break;

        case CODE_FD_LDIYANN:                                   /* This is a LD IY,(nn)    */
            sprintf(mInstruction, "LD IY,(#%04X)", op16);

            if (pMode==INTP_EXECUTE || pMode==INTP_EXECUTE_BLIND)                            
            {
                reg16_1=get16bitsRegisterAddress(REGIY);
                reg16_1->setValue(mMemory->getAddress(op16));

                if (pMode==INTP_EXECUTE)
                {
                    printf("\n%s was executed\n", mInstruction);
                }
            }
            
            if (pMode==INTP_DISPLAY)
            {
                printf("\n[%08X] is %s\n", codeInHexa, mInstruction);
            }
            break;

        case CODE_LDNNHL:                                       /* This is a LD (nn),HL    */   
            sprintf(mInstruction, "LD (#%04X),HL", op16);

            if (pMode==INTP_EXECUTE || pMode==INTP_EXECUTE_BLIND)                            
            {
                reg16_1=get16bitsRegisterAddress(REGHL);
                mMemory->setAddress(op16, reg16_1->getValue());

                if (pMode==INTP_EXECUTE)
                {
                    printf("\n%s was executed\n", mInstruction);
                }
            }
            
            if (pMode==INTP_DISPLAY)
            {
                printf("\n[%06X] is %s\n", codeInHexa, mInstruction);
            }
            break;

        case CODE_INCR:                                         /* This is a INC R  */
            ret=bitToRegister(op1, sop1);

            sprintf(mInstruction, "INC %s", sop1);

            if (pMode==INTP_EXECUTE || pMode==INTP_EXECUTE_BLIND)                            
            {
                reg8_1=get8bitsRegisterAddress(op1);
                reg8_1->setValue(reg8_1->getValue()+1);

                /* Modify flags here    */
                mRegisterPack.regF.setSignFlag(reg8_1->getSignFlag());
                mRegisterPack.regF.setZeroFlag(reg8_1->isZero());
                mRegisterPack.regF.setAddSubtractFlag(false);

                if (reg8_1->getValue()==0x10)
                {
                    mRegisterPack.regF.setHalfCarryFlag(true);
                }
                else
                {
                    mRegisterPack.regF.setHalfCarryFlag(false);
                }

                if (reg8_1->getValue()==0x80)
                {
                    mRegisterPack.regF.setParityOverflowFlag(true);
                }
                else
                {
                    mRegisterPack.regF.setParityOverflowFlag(false);
                }

                if (pMode==INTP_EXECUTE)
                {
                    printf("\n%s was executed\n", mInstruction);
                }
            }
            
            if (pMode==INTP_DISPLAY)
            {
                printf("\n[%02X] is %s\n", codeInHexa, mInstruction);
            }
            break;

            case CODE_DECR:                                         /* This is a DEC R  */
            ret=bitToRegister(op1, sop1);

            sprintf(mInstruction, "DEC %s", sop1);

            if (pMode==INTP_EXECUTE || pMode==INTP_EXECUTE_BLIND)                            
            {
                reg8_1=get8bitsRegisterAddress(op1);
                reg8_1->setValue(reg8_1->getValue()-1);

                /* Modify flags here    */
                mRegisterPack.regF.setSignFlag(reg8_1->getSignFlag());
                mRegisterPack.regF.setZeroFlag(reg8_1->isZero());
                mRegisterPack.regF.setAddSubtractFlag(true);

                if (reg8_1->getValue()==0x0F)
                {
                    mRegisterPack.regF.setHalfCarryFlag(true);
                }
                else
                {
                    mRegisterPack.regF.setHalfCarryFlag(false);
                }

                if (reg8_1->getValue()==0x7F)
                {
                    mRegisterPack.regF.setParityOverflowFlag(true);
                }
                else
                {
                    mRegisterPack.regF.setParityOverflowFlag(false);
                }

                if (pMode==INTP_EXECUTE)
                {
                    printf("\n%s was executed\n", mInstruction);
                }
            }
            
            if (pMode==INTP_DISPLAY)
            {
                printf("\n[%02X] is %s\n", codeInHexa, mInstruction);
            }
            break;


        case CODE_INCRR:                                            /* This is a INC rr    */
            ret=bitToRegister(op1, sop1);

            sprintf(mInstruction, "INC %s", sop1);

            if (pMode==INTP_EXECUTE || pMode==INTP_EXECUTE_BLIND)                                
            {
                reg16_1=get16bitsRegisterAddress(op1);
                reg16_1->setValue(reg16_1->getValue()+1);

                if (pMode==INTP_EXECUTE)
                {
                    printf("\n%s was executed\n", mInstruction);
                }
            }
            
            if (pMode==INTP_DISPLAY)
            {
                printf("\n[%02X] is %s\n", codeInHexa, mInstruction);
            }
            break;

        case CODE_PUSHQQ:                                       /* This is a PUSH qq                    */
            ret=bitToRegister(op1, sop1);

            sprintf(mInstruction, "PUSH %s", sop1);

            if (pMode==INTP_EXECUTE || pMode==INTP_EXECUTE_BLIND)                            
            {
                mRegisterPack.regSP.setValue(mRegisterPack.regSP.getValue()-2);
                reg16_1=get16bitsRegisterAddress(op1);
                mMemory->setAddress(mRegisterPack.regSP.getValue(), reg16_1->getValue());

                if (pMode==INTP_EXECUTE)
                {
                    printf("\n%s was executed\n", mInstruction);
                }
            }
            
            if (pMode==INTP_DISPLAY)
            {
                printf("\n[%02X] is %s\n", codeInHexa, mInstruction);
            }
            break;

        case CODE_ED_LDNNRR:                                        /* This is a LD (nn),rr    */   
            ret=bitToRegister(op1, sop1);                           

            sprintf(mInstruction, "LD (#%04X),%s", op16, sop1);

            if (pMode==INTP_EXECUTE || pMode==INTP_EXECUTE_BLIND)
            {
                reg16_1=get16bitsRegisterAddress(op1);
                mMemory->setAddress(op16, reg16_1->getValue());

                if (pMode==INTP_EXECUTE)
                {
                    printf("\n%s was executed\n", mInstruction);
                }
            }
            
            if (pMode==INTP_DISPLAY)
            {
                printf("\n[%06X] is %s\n", codeInHexa, mInstruction);
            }
            break;

        case CODE_RLCA:                                         /* This is a RLCA  */
            sprintf(mInstruction, "RLCA");

            if (pMode==INTP_EXECUTE || pMode==INTP_EXECUTE_BLIND)                            
            {
                reg8_1=get8bitsRegisterAddress(REGA);
                newVal=(reg8_1->getValue()<<1) | BIT(reg8_1->getValue(), 7);
                reg8_1->setValue(newVal);

                /* Modify flags here    */
                H_RESET;
                N_RESET;
                C_IS(BIT(reg8_1->getValue(), 0));

                if (pMode==INTP_EXECUTE)
                {
                    printf("\n%s was executed\n", mInstruction);
                }
            }
            
            if (pMode==INTP_DISPLAY)
            {
                printf("\n[%02X] is %s\n", codeInHexa, mInstruction);
            }
            break;

        case CODE_CB_RLCR:                                         /* This is a RLC r  */
            ret=bitToRegister(op1, sop1);

            sprintf(mInstruction, "RLC %s", sop1);

            if (pMode==INTP_EXECUTE || pMode==INTP_EXECUTE_BLIND)                            
            {
                reg8_1=get8bitsRegisterAddress(op1);
                newVal=(reg8_1->getValue()<<1) | BIT(reg8_1->getValue(), 7);
                reg8_1->setValue(newVal);

                /* Modify flags here    */
                S_IS(SIGN(newVal));
                Z_IS(ZERO(newVal));
                H_RESET;
                PV_IS(EVEN(newVal));
                N_RESET;
                C_IS(BIT(reg8_1->getValue(), 0));

                if (pMode==INTP_EXECUTE)
                {
                    printf("\n%s was executed\n", mInstruction);
                }
            }
            
            if (pMode==INTP_DISPLAY)
            {
                printf("\n[%02X] is %s\n", codeInHexa, mInstruction);
            }
            break;

        case CODE_CB_RLCHL:                                         /* This is a RLC (HL)  */
            sprintf(mInstruction, "RLC (HL)");

            if (pMode==INTP_EXECUTE || pMode==INTP_EXECUTE_BLIND)                            
            {
                /* Retrive the byte and rotate it  */
                val=mMemory->get8bitsValue(mRegisterPack.regHL.getValue());
                newVal=(val<<1) | BIT(val, 7);
                mMemory->set8bitsValue(mRegisterPack.regHL.getValue(), newVal);

                /* Modify flags here    */
                S_IS(SIGN(newVal));
                Z_IS(ZERO(newVal));
                H_RESET;
                PV_IS(EVEN(newVal));
                N_RESET;
                C_IS(BIT(val, 7));

                if (pMode==INTP_EXECUTE)
                {
                    printf("\n%s was executed\n", mInstruction);
                }
            }
            
            if (pMode==INTP_DISPLAY)
            {
                printf("\n[%02X] is %s\n", codeInHexa, mInstruction);
            }
            break;
    }

    /*************************************************************************************************************************/

    
    return 0;
}

/* Used to cut the instruction  */
int8_t Z80Machine::cutInstruction(char *pInstruction, char *pInst, char *pOp1, char *pOp2)
{
    int8_t nbOfComp=0;
    int8_t retCheck=0;
    char *charPos;

    pOp1[0]='\0';
    pOp2[0]='\0';

    strcpy(pInst, pInstruction);                        /* Init the return intruction string    */
    
    /* Is there a space into the instruction ?  */
    if (!(charPos=strchr(pInst, ' ')))
    {
        pOp1[0]='\0';
        pOp2[0]='\0';

        nbOfComp=1;
    }
    else 
    {
        strcpy(pOp1, charPos+1);
        pInst[charPos-pInst]='\0';
        
        /* Is there a ',' into the instruction ?  */
        if (!(charPos=strchr(pOp1, ',')))
        {
            nbOfComp=2;
        }
        else
        {
            pOp1[charPos-pOp1]='\0';
            strcpy(pOp2, charPos+1);
            nbOfComp=3;
        }
    }

    //printf("inst=%s\n", pInst);
    //printf("op1 =%s\n", pOp1);
    //printf("op2 =%s\n", pOp2);
    
    return nbOfComp;
}

/* Find machine code    */
uint32_t Z80Machine::findMachineCode(char *pInstruction, uint8_t *pLen)
{
    uint32_t retCode=0xFFFFFFFF;
    char str_inst[MAX_LEN];
    char str_op1[MAX_OP_LENGTH];
    char str_op2[MAX_OP_LENGTH];
    char *str_ptr=NULL;
    uint8_t nbOfComponents=0;
    uint8_t lenEff=0;
    int8_t retCheck=0;
    uint16_t word=0;
    bool isIX=false;
    bool isIY=false;

    nbOfComponents=cutInstruction(pInstruction, str_inst, str_op1, str_op2);

    //printf(">>> %s %s,%s\n", str_inst, str_op1, str_op2);
    //printf("comp=%d\n", nbOfComponents);

    switch (nbOfComponents)
    {
        case 1:                                                     /* Only one component in the instruction    */
            if (!strcmp(str_inst, "NOP"))                           /* A NOP is present     */
            {
                retCode=CODE_NOP;
                *pLen=ONE_BYTE;
            }

            if (!strcmp(str_inst, "HALT"))                          /* A HALT is present     */
            {
                retCode=CODE_HALT;
                *pLen=ONE_BYTE;
            }

            if (!strcmp(str_inst, "RLCA"))                          /* A RLCA is present     */
            {
                retCode=CODE_RLCA;
                *pLen=ONE_BYTE;
            }
            break;

        case 2:
            if (!strcmp(str_inst, "RLC"))                           /* A RLC instruction is present         */
            {
                if (strlen(str_op1)==1)                             /* Check if it is a RLC r instruction   */
                {
                    retCode=CODE_CB_RLCR;                           /* Prepare the RLC r                    */
                    *pLen=TWO_BYTES;

                    retCheck=clean_r(str_op1);
                    
                    PUSHBIT(retCode, registerToBit(str_op1), 0);    /* Add the register as bits             */
                }

                if (!strcmp(str_op1, "(HL)"))                       /* Check if it is a RLC (HL) instruction*/
                {
                    retCode=CODE_CB_RLCHL;                           /* Prepare the RLC r                    */
                    *pLen=TWO_BYTES;
                }
            }

            if (!strcmp(str_inst, "INC"))                           /* A INC instruction is present         */
            {
                if (strlen(str_op1)==1)                             /* Check if it is a INC r instruction   */
                {
                    retCode=CODE_INCR;                              /* Prepare the INC r                    */
                    *pLen=ONE_BYTE;

                    retCheck=clean_r(str_op1);
                    
                    PUSHBIT(retCode, registerToBit(str_op1), 3);    /* Add the register as bits             */
                }

                if (strlen(str_op1)==2)                             /* Check if it is a INC rr instruction   */
                {
                    retCode=CODE_INCRR;                              /* Prepare the INC rr                   */
                    *pLen=ONE_BYTE;

                    retCheck=clean_r(str_op1);
                    
                    PUSHBIT(retCode, registerToBit(str_op1), 4);    /* Add the register as bits             */
                }
            }

            if (!strcmp(str_inst, "DEC"))                          /* A DEC instruction is present         */
            {
                if (strlen(str_op1)==1)                             /* Check if it is a INC r instruction   */
                {
                    retCode=CODE_DECR;                              /* Prepare the INC r                    */
                    *pLen=ONE_BYTE;

                    retCheck=clean_r(str_op1);
                    
                    PUSHBIT(retCode, registerToBit(str_op1), 3);    /* Add the register as bits             */
                }

                if (strlen(str_op1)==2)                             /* Check if it is a INC rr instruction   */
                {
                    retCode=CODE_DECRR;                              /* Prepare the INC rr                   */
                    *pLen=ONE_BYTE;

                    retCheck=clean_r(str_op1);
                    
                    PUSHBIT(retCode, registerToBit(str_op1), 4);    /* Add the register as bits             */
                }
            }

            if (!strcmp(str_inst, "PUSH"))                          /* A PUSH instruction is present         */
            {
                /* Check if it is a PUSH qq instruction   */
                if (strlen(str_op1)==2 && (!strcmp(str_op1, "BC") || !strcmp(str_op1, "DE") || !strcmp(str_op1, "HL") || !strcmp(str_op1, "AF")))                             
                {
                    retCode=CODE_PUSHQQ;                              /* Prepare the PUSH qq                    */
                    *pLen=ONE_BYTE;

                    retCheck=clean_r(str_op1);

                    if (!strcmp(str_op1, "AF"))
                    {
                        strcpy(str_op1, "SP");
                    }

                    PUSHBIT(retCode, registerToBit(str_op1), 4);    /* Add the register as bits             */
                }
            }
            break;    
    
        case 3:
            if (!strcmp(str_inst, "LD"))                            /* A LD instruction is present  */
            {
                /* Check if it is a LD, r,r' instruction    */
                if (strlen(str_op1)==1 && strlen(str_op2)==1)       
                {
                    retCode=CODE_LDRR;                              /* Prepare the LD r,r'  */
                    *pLen=ONE_BYTE;

                    retCheck=clean_r(str_op1);
                    retCheck=clean_r(str_op2);
                    
                    PUSHBIT(retCode, registerToBit(str_op1), 3);    /* Add the first register as bits   */
                    PUSHBIT(retCode, registerToBit(str_op2), 0);    /* Add the first register as bits   */
                }

                /* Check if it is a LD r,n instruction    */
                if (strlen(str_op1)==1 && ((strlen(str_op2)==2) || (strlen(str_op2)==3)))       
                {
                    /* Clean the n for Op2 and r for Op1 */
                    retCheck=clean_r(str_op1);
                    retCheck=clean_n(str_op2);

                    retCode=CODE_LDRN;
                    
                    PUSHBIT(retCode, registerToBit(str_op1), 3);                          /* Add the first register as bits   */

                    retCode=(retCode<<SIZE_1_BYTE)+toValue(str_op2+1, pLen, &lenEff);     /* Prepare the LD r,n   */
                    *pLen=TWO_BYTES;
                }

                /* Check if it is a LD rr,nn instruction    */
                if (strlen(str_op1)==2 && ((strlen(str_op2)==2) || (strlen(str_op2)==3)|| (strlen(str_op2)==4)|| (strlen(str_op2)==5)))       
                {
                    /* Clean the n for Op2 and r for Op1 */
                    retCheck=clean_r(str_op1);
                    retCheck=clean_nn(str_op2);

                    word=toValue(str_op2+1, pLen, &lenEff);

                    retCode=CODE_LDRRNN;
                    
                    PUSHBIT(retCode, registerToBit(str_op1), 4);                /* Add the first register as bits   */

                    retCode=(retCode<<SIZE_2_BYTES) + ((word & FIRST_LOWEST_BYTE) << SIZE_1_BYTE) + ((word & SECOND_LOWEST_BYTE)>>SIZE_1_BYTE);     /* Prepare the LD rr,nn   */
                    *pLen=THREE_BYTES;
                }

                /* Check if it is a LD r,(IX+n) instruction    */
                if (strlen(str_op1)==1 && ((strlen(str_op2)==7) || (strlen(str_op2)==8)) && (strstr(str_op2, "IX") || strstr(str_op2, "IY")))       
                {
                    /* Clean the (IX+d) or (IY+d) for Op2 and r for Op1 */
                    retCheck=clean_r(str_op1);

                    if (strstr(str_op2, "IX")) 
                    {
                        retCode=CODE_DD_LDRIXD;
                    }
                    else if (strstr(str_op2, "IY")) 
                    {
                        retCode=CODE_FD_LDRIYD;
                    }

                    retCheck=clean_ixn(str_op2);
                    
                    PUSHBIT(retCode, registerToBit(str_op1), 3);                /* Add the first register as bits   */

                    retCode=(retCode<<8)+toValue(str_op2+1, pLen, &lenEff);     /* Prepare the LD r,(IX+d) or the LD r,(IY+d)  */

                    *pLen=THREE_BYTES;
                }

                /* Check if it is a LD r,(HL) or a LD A,(BC) or a LD A,(DE) instruction    */
                if (strlen(str_op1)==1 && strlen(str_op2)==4 && !strchr(str_op2, '#'))       
                {
                    /* Clean the n for Op1 */
                    retCheck=clean_r(str_op1);
                    //retCheck=clean_nn(str_op2);

                    if (!strcmp(str_op2, "(HL)"))
                    {
                        retCode=CODE_LDRHL;
                        PUSHBIT(retCode, registerToBit(str_op1), 3);                /* Add the first register as bits   */
                    }

                    if (!strcmp(str_op2, "(BC)"))
                    {
                        retCode=CODE_LDABC;
                    }

                    if (!strcmp(str_op2, "(DE)"))
                    {
                        retCode=CODE_LDADE;
                    }                  

                    *pLen=ONE_BYTE;
                }

                /* Check if it is a LD A,(nn) instruction    */
                if (strlen(str_op1)==1 && strchr(str_op2, '#') && strchr(str_op2, '(') && strchr(str_op2, ')') && strlen(str_op2)>=4 && strlen(str_op2)<=7 )    
                {
                    retCode=CODE_LDANN;
                    /* Clean the r for Op1 */
                    retCheck=clean_r(str_op1);

                    str_ptr=str_op2+1;                      /* Remove '(' and ')'   */
                    str_ptr[strlen(str_ptr)-1]='\0';

                    word=toValue(str_ptr+1, pLen, &lenEff);

                    retCode=(retCode<<SIZE_2_BYTES) + ((word & FIRST_LOWEST_BYTE) << SIZE_1_BYTE) + ((word & SECOND_LOWEST_BYTE)>>SIZE_1_BYTE);

                    *pLen=THREE_BYTES;
                }

                /* Check if it is LD (BC),A or a LD (DE),A instruction    */
                if (strlen(str_op1)==4 && strlen(str_op2)==1 && !strchr(str_op2, '#') && !strcmp(str_op2, "A"))       
                {
                    /* Clean the r for Op2 */
                    retCheck=clean_r(str_op2);
                    
                    if (!strcmp(str_op1, "(BC)"))
                    {
                        retCode=CODE_LDBCA;
                    }

                    if (!strcmp(str_op1, "(DE)"))
                    {
                        retCode=CODE_LDDEA;
                    }                  

                    *pLen=ONE_BYTE;
                }

                /* Check if it is a LD (nn),A instruction    */
                if (!strcmp(str_op2, "A") && strchr(str_op1, '#') && strchr(str_op1, '(') && strchr(str_op1, ')') && strlen(str_op1)>=4 && strlen(str_op1)<=7 )    
                {
                    retCode=CODE_LDNNA;
                    /* Clean the r for Op1 */
                    retCheck=clean_r(str_op2);

                    str_ptr=str_op1+1;                      /* Remove '(' and ')'   */
                    str_ptr[strlen(str_ptr)-1]='\0';

                    word=toValue(str_ptr+1, pLen, &lenEff);

                    retCode=(retCode<<SIZE_2_BYTES) + ((word & FIRST_LOWEST_BYTE) << SIZE_1_BYTE) + ((word & SECOND_LOWEST_BYTE)>>SIZE_1_BYTE);

                    *pLen=THREE_BYTES;
                }

                /* Check if it is a LD (nn),rr instruction    */
                if (strlen(str_op2)==2 && strchr(str_op1, '#') && strchr(str_op1, '(') && strchr(str_op1, ')') && strlen(str_op1)>=4 && strlen(str_op1)<=7 )    
                {
                    retCode=CODE_LDNNRR;

                    PUSHBIT(retCode, registerToBit(str_op2), 4);
                    /* Clean the r for Op1 */
                    //retCheck=clean_r(str_op2);

                    str_ptr=str_op1+1;                      /* Remove '(' and ')'   */
                    str_ptr[strlen(str_ptr)-1]='\0';

                    word=toValue(str_ptr+1, pLen, &lenEff);

                    retCode=(retCode<<SIZE_2_BYTES) + ((word & FIRST_LOWEST_BYTE) << SIZE_1_BYTE) + ((word & SECOND_LOWEST_BYTE)>>SIZE_1_BYTE);

                    *pLen=THREE_BYTES;
                }

            }
            break;
            
    }

    //printf("rc=<%04X>\n", retCode);
    //printf("ln=%d\n", *pLen);

    return retCode;


}

/* Give the next instruction to execute.    */
uint32_t Z80Machine::getNextInstruction(char *pInstr, uint8_t *pLen)
{
    //uint8_t len=0;
    uint32_t machineCode=0;
    uint16_t address=0;

    address=mRegisterPack.regPC.getValue();

    switch (mMemory->get8bitsValue(address))
    {
        case ALT_CODE_CB:
            *pLen=cb_code_length[mMemory->get8bitsValue(address+1)]+ONE_BYTE;
            break;

        case ALT_CODE_ED:
            *pLen=ed_code_length[mMemory->get8bitsValue(address+1)]+ONE_BYTE;
            break;

        case ALT_CODE_DD:
            if (mMemory->get8bitsValue(address+1)==ALT_CODE_CB)
            {
                *pLen=ddcb_code_length[mMemory->get8bitsValue(address+2)]+TWO_BYTES;
            }
            else
            {
                *pLen=dd_code_length[mMemory->get8bitsValue(address+1)]+ONE_BYTE;
            }
            break;
        
        case ALT_CODE_FD:
            if (mMemory->get8bitsValue(address+1)==ALT_CODE_CB)
            {
                *pLen=fdcb_code_length[mMemory->get8bitsValue(address+2)]+TWO_BYTES;
            }
            else
            {
                *pLen=fd_code_length[mMemory->get8bitsValue(address+1)]+ONE_BYTE;
            }
            break;

        default:
            *pLen=natural_code_length[mMemory->get8bitsValue(address)];
    }

    for (int i=0; i<*pLen/2; i++)
    {
        uint16_t value=mMemory->get8bitsValue(address+i);
        machineCode=(machineCode<<SIZE_1_BYTE)+mMemory->get8bitsValue(address+i);
    }

    interpretCode(machineCode, *pLen, INTP_DETECT);
    strcpy(pInstr, mInstruction);

    return machineCode;
}

/* Analyse the command  */
bool Z80Machine::analyse()
{
    typeOfEntry type=NOTHING;
    bool retValue=false;
    uint8_t lenValue=0;
    uint8_t lenEff=0;
    int32_t valDec=0; 
    uint32_t value=0;
    uint32_t machineCode=0;
    uint32_t codeInHexa=0;
    char instruction[MAX_OP_LENGTH*3];

    if (mCommandIsEntered)
    {
        //printf(">>> analyse [%s] <<<\n", mEntry);

        switch(findEntryType())
        {
            case COMMAND:
                switch (mEntry[0]) {

                /* I have to exit	*/
                case CMD_EXIT:
                    retValue=true;
                    break;
            
                /* OK, display help	*/
                case CMD_HELP:
                    if (mExecMode)
                    {
                        printf("\n");
                        printf("!           toggle from Normal mode to Exec mode.\n");
                        printf("a <code>    translate <code> to assembly langage.\n");
                        printf("              Example: cb22 gives SLA D\n");
                        printf("c <cmd>     translate <cmd> to machine code.\n");
                        printf("              Example: ld c,b gives 0x41\n");
                        printf("R           display all registers.\n");
                        printf("m <addr>    dump 16 bytes memory from <addr>.\n");
                        printf("m (SP)      dump 16 bytes memory from (SP).\n");
                        printf("m (PC)      dump 16 bytes memory from (PC).\n");
                        printf("n           execute the next instruction (only in Exec mode).\n");
                        printf("x <dec>     convert <dec> to hexa.\n");
                        printf("d <hex>     convert <hex> to decimal.\n");
                        printf("b <hex>     convert <hex> to binary.\n");
                        printf("q           quit me.\n");
                        printf("\n");
                        printf("<cmd>       execute the command.\n");
                        printf("<code>      execute the code.\n");
                    }
                    else
                    {
                        printf("\n");
                        printf("!           toggle from Normal mode to Exec mode.\n");
                        printf("a <code>    translate <code> to assembly langage.\n");
                        printf("              Example: cb22 gives SLA D\n");
                        printf("c <cmd>     translate <cmd> to machine code.\n");
                        printf("              Example: ld c,b gives 0x41\n");
                        printf("r           display main registers.\n");
                        printf("R           display all registers.\n");
                        printf("m <addr>    dump 16 bytes memory from <addr>.\n");
                        printf("m (SP)      dump 16 bytes memory from (SP).\n");
                        printf("m (PC)      dump 16 bytes memory from (PC).\n");
                        printf("x <dec>     convert <dec> to hexa.\n");
                        printf("d <hex>     convert <hex> to decimal.\n");
                        printf("b <hex>     convert <hex> to binary.\n");
                        printf("q           quit me.\n");
                        printf("\n");
                        printf("<cmd>       execute the command.\n");
                        printf("<code>      execute the code.\n");
                    }
                    break;
          
                case CMD_EXAMPLE:
                    printf("\n");
                    printf("0x32aaaa - LD (aaaa),A\n");
                    printf("0x36nn   - LD (HL),n\n");
                    printf("0x3Aaaaa - LD A,(aaaa)\n");
                    printf("00x46    - LD B,(HL)\n"); 
                    printf("00x4E    - LD C,(HL)\n"); 
                    printf("00x70    - LD (HL), B\n");
                    printf("00x71    - LD (HL), C\n\n");

                    break;

                /* Mode switching       */
                case CMD_EXEC_MODE_TOGGLE:
                    mExecMode=!mExecMode;

                    break;

                /* Display registers	*/
                case CMD_REGISTER:
                    if (!mExecMode)
                    {
                        displaySimpleRegisters();
                    }

                    break;

                /* Display all the registers	*/
                case CMD_ALL_REGISTER:
                    if (!mExecMode)
                    {
                        displayAllRegisters();
                    }

                    break;

                case CMD_ASSEMBLYCODE:
                    mEntry+=2;
                    codeInHexa=toValue(mEntry, &lenValue, &lenEff);                     /* Transform the instruction into real number  */
                    interpretCode(codeInHexa, lenValue, INTP_DISPLAY);
                    
                    break;
                
                case CMD_MACHINECODE:
                    mEntry+=2;
                    machineCode=findMachineCode(mEntry, &lenValue);
                    interpretCode(machineCode, lenValue, INTP_DISPLAY);
                    
                    break;

                case CMD_TODEC:
                    mEntry+=2;
                    mEntry[FOUR_BYTES]='\0';
                    value=toValue(mEntry, &lenValue, &lenEff);

                    if (lenEff<lenValue)
                    {
                        printf("\nNot an hexa number\n");
                    }
                    else
                    {
                        printf("\n0x%s = %ud\n", mEntry, value);
                    }
                    
                    break;

                case CMD_TOHEXA:
                    mEntry+=2;

                    valDec=toDec(mEntry);

                    if (valDec>0)
                    {
                        printf("\n%sd = 0x%X\n", mEntry, valDec);
                    }
                    else
                    {
                        printf("\nNot a decimal number\n");
                    }
                    
                    break;

                case CMD_TOBIN:
                    mEntry+=2;
                    mEntry[ONE_BYTE]='\0';                             /* Only one byte    */

                    value=toValue(mEntry, &lenValue, &lenEff);

                    if (lenEff<lenValue)
                    {
                        printf("\nNot a decimal number\n"); 
                    }
                    else
                    {
                        printf("\n0x%s = 0b%s\n", mEntry, byteToBinary((uint8_t) value));
                    }
                    
                    break;

                case CMD_DISPLAY_MEMORY:
                    displayMemory(mEntry+2);
                    
                    break;

                case CMD_NEXT_INSTRUCTION:
                    if (mExecMode)
                    {
                        /* Retrieve the instruction         */
                        machineCode=getNextInstruction(instruction, &lenValue);

                        /* Execute the instruction          */
                        interpretCode(machineCode, lenValue, INTP_EXECUTE_BLIND);
                        
                        /* Go to next instruction           */
                        mRegisterPack.regPC.setValue(mRegisterPack.regPC.getValue() + (lenValue/2));
                    }
            }
            break;

            case CODE:
                codeInHexa=toValue(mEntry, &lenValue, &lenEff);                     /* Transform the instruction into real number  */
                interpretCode(codeInHexa, lenValue, INTP_EXECUTE);
                
                break;

            case INSTRUCTION:
                machineCode=findMachineCode(mEntry, &lenValue);
                interpretCode(machineCode, lenValue, INTP_EXECUTE);
                break;
        }
    }
    
    return retValue;
}
