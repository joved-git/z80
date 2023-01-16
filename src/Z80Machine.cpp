#include "../inc/Z80Machine.h"
#include "../inc/instruction_length.h"
#include "../inc/LabelDataset.h"

#include <list>

/* The constructor  */
Z80Machine::Z80Machine()
{
    mCommandIsEntered=false;
    mEntry=NULL;

    /* Define the type of the 16-bit registers  */
    mRegisterPack.regBC.set16bitsRegisterType(HALF);
    mRegisterPack.regBC.setHighLowRegister(&mRegisterPack.regB, &mRegisterPack.regC);
    mRegisterPack.regDE.set16bitsRegisterType(HALF);
    mRegisterPack.regDE.setHighLowRegister(&mRegisterPack.regD, &mRegisterPack.regE);
    mRegisterPack.regHL.set16bitsRegisterType(HALF);
    mRegisterPack.regHL.setHighLowRegister(&mRegisterPack.regH, &mRegisterPack.regL);
    mRegisterPack.regAF.set16bitsRegisterType(HALF);
    mRegisterPack.regAF.setHighLowRegister(&mRegisterPack.regA, &mRegisterPack.regF);

    mRegisterPack.regSP.set16bitsRegisterType(FULL);
    mRegisterPack.regPC.set16bitsRegisterType(FULL);
    mRegisterPack.regIX.set16bitsRegisterType(FULL);
    mRegisterPack.regIY.set16bitsRegisterType(FULL);

    mRegisterPack.regAFp.set16bitsRegisterType(FULL);
    mRegisterPack.regBCp.set16bitsRegisterType(FULL);
    mRegisterPack.regDEp.set16bitsRegisterType(FULL);
    mRegisterPack.regHLp.set16bitsRegisterType(FULL);

    /* Initialize registers to 0 except SP  */
    mRegisterPack.regA.setValue(0x00);
    mRegisterPack.regB.setValue(0x00);
	mRegisterPack.regC.setValue(0x00);
	mRegisterPack.regD.setValue(0x00);
	mRegisterPack.regE.setValue(0x00);
	mRegisterPack.regL.setValue(0x00);
    mRegisterPack.regI.setValue(0x00);  
	mRegisterPack.regR.setValue(0x00);
	mRegisterPack.regF.setValue(0b00000000);
    
    mRegisterPack.regPC.setValue(0x0000);
    mRegisterPack.regIX.setValue(0x0000);
    mRegisterPack.regIY.setValue(0x0000);
    mRegisterPack.regSP.setValue(INITIAL_STACK_POINTER);

    mRegisterPack.regAFp.setValue(0x0000);
    mRegisterPack.regBCp.setValue(0x0000);
    mRegisterPack.regDEp.setValue(0x0000);
    mRegisterPack.regHLp.setValue(0x0000);

    /* Reset the interrupt flip-flops   */
    mRegisterPack.iff1=false;
    mRegisterPack.iff2=false;

     /* Initialize some registers (for test)   */
    mRegisterPack.regB.setValue(0x01);
	mRegisterPack.regC.setValue(0xAA);
	mRegisterPack.regD.setValue(0x0C);
	mRegisterPack.regE.setValue(0x10);
	mRegisterPack.regL.setValue(0xC8);
    mRegisterPack.regR.setValue(0xFE);
    mRegisterPack.regI.setValue(0x33);
    mRegisterPack.regPC.setValue(0x1234);
    mRegisterPack.regHL.setValue(0xFE14);
    mRegisterPack.regIX.setValue(0x1200);
    mRegisterPack.regIY.setValue(0x1400);

    /* Reset all the registers (color bool) */
    resetAllChangedRegister();

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



/* Reset or init all the changed bool onto all registers.   */
void Z80Machine::resetAllChangedRegister()
{
    /* 8-bit registers   */
    mRegisterPack.regB.resetChanged();
    mRegisterPack.regC.resetChanged();
    mRegisterPack.regD.resetChanged();
    mRegisterPack.regE.resetChanged();
    mRegisterPack.regH.resetChanged();
    mRegisterPack.regL.resetChanged();
    mRegisterPack.regA.resetChanged();
    mRegisterPack.regF.resetChanged();
    mRegisterPack.regF.resetColorChangedFlag();
    mRegisterPack.regI.resetChanged();
    mRegisterPack.regR.resetChanged();
    
    /* 16-bit registers   */
    mRegisterPack.regBC.resetChanged();
    mRegisterPack.regDE.resetChanged();
    mRegisterPack.regHL.resetChanged();
    mRegisterPack.regAF.resetChanged();
    mRegisterPack.regPC.resetChanged();
    mRegisterPack.regSP.resetChanged();
    mRegisterPack.regIX.resetChanged();
    mRegisterPack.regIY.resetChanged();

    /* Alternate 16-bit registers   */
    mRegisterPack.regBCp.resetChanged();
    mRegisterPack.regDEp.resetChanged();
    mRegisterPack.regHLp.resetChanged();
    mRegisterPack.regAFp.resetChanged();
}


/* This method give the parity of a byte    */
uint8_t Z80Machine::calcParity(uint8_t pByte)
{
    return(!(parityTable[pByte & 0x0F] ^ parityTable[(pByte & 0xF0) >> 4]));
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

        if (!strcmp(pRegister, "HL") || !strcmp(pRegister, "IX") || !strcmp(pRegister, "IY"))
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


/* Condition to bit converter  */
uint8_t Z80Machine::conditionToBit(char *pCondition)
{
    uint8_t retBit=0;
    
    if (!strcmp(pCondition, STRING_CONDNZ))
    {
        retBit=CONDNZ;
    }

    if (!strcmp(pCondition, STRING_CONDZ))
    {
        retBit=CONDZ;
    }

    if (!strcmp(pCondition, STRING_CONDNC))
    {
        retBit=CONDNC;
    }

    if (!strcmp(pCondition, STRING_CONDC))
    {
        retBit=CONDC;
    }
    
    if (!strcmp(pCondition, STRING_CONDPO))
    {
        retBit=CONDPO;
    }

    if (!strcmp(pCondition, STRING_CONDPE))
    {
        retBit=CONDPE;
    }

    if (!strcmp(pCondition, STRING_CONDP))
    {
        retBit=CONDP;
    }

    if (!strcmp(pCondition, STRING_CONDM))
    {
        retBit=CONDM;
    }
    
    return retBit;

}


/* Convert the 3-bit number (string) into its binary value (from 0 to 7)    */
uint8_t Z80Machine::numberToBit(char *pNumber)
{
    uint8_t retBit=pNumber[0]-'0';
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

        case REGIX:
            strcpy(pRetChar, STRING_REGIX);
            break;

        case REGIY:
            strcpy(pRetChar, STRING_REGIY);
            break;

        default:
            strcpy(pRetChar, STRING_REG_UNDEFINED);
            ret=ERR_NO_REGISTER;
    }

    return ret;
}


/* Convert the uint8_t value into the name of the condition */
uint8_t Z80Machine::bitToCondition(uint8_t pBit, char *pRetChar)
{
    uint8_t ret=0;

    switch (pBit)
    {
        case CONDNZ:
            strcpy(pRetChar, STRING_CONDNZ);
            break;

        case CONDZ:
            strcpy(pRetChar, STRING_CONDZ);
            break;

        case CONDNC:
            strcpy(pRetChar, STRING_CONDNC);
            break;

        case CONDC:
            strcpy(pRetChar, STRING_CONDC);
            break;

        case CONDPO:
            strcpy(pRetChar, STRING_CONDPO);
            break;

        case CONDPE:
            strcpy(pRetChar, STRING_CONDPE);
            break;

        case CONDP:
            strcpy(pRetChar, STRING_CONDP);
            break;

        case CONDM:
            strcpy(pRetChar, STRING_CONDM);
            break;
        default:
            strcpy(pRetChar, STRING_COND_UNDEFINED);
            ret=ERR_NO_CONDITION;
    }

    return ret;
}


/* Tell if the given condition is true or not   */
bool Z80Machine::isConditionTrue(uint8_t pCond)
{
    bool retCond=false;

    switch (pCond)
    {
        case CONDNZ:
            retCond=!mRegisterPack.regF.getZeroFlag();
            break;

        case CONDZ:
            retCond=mRegisterPack.regF.getZeroFlag();
            break;

        case CONDNC:
            retCond=!mRegisterPack.regF.getCarryFlag();
            break;

        case CONDC:
            retCond=mRegisterPack.regF.getCarryFlag();
            break;

        case CONDPO:
            retCond=!mRegisterPack.regF.getParityOverflowFlag();
            break;

        case CONDPE:
            retCond=mRegisterPack.regF.getParityOverflowFlag();
            break;

        case CONDP:
            retCond=!mRegisterPack.regF.getSignFlag();
            break;

        case CONDM:
            retCond=mRegisterPack.regF.getSignFlag();
            break;
    }

    return retCond;
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

    if (!strcmp(mEntry, "CCF") || !strcmp(mEntry, "DAA"))
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

        /* Convert string into uppercase but not the first car of the command and not for the load command   */
        if (mEntry[0]!=CMD_LOAD_CODE)
        {
            toUpper(mEntry+1);
        }
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


/* Convert the hexa string into real value  */
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

    return hexaValue;
}      



/* Convert the decimal string into real value  */
uint32_t Z80Machine::toDecValue(char *pCode, uint8_t *pLen, uint8_t *pLenEffective)
{
    uint32_t decValue=0;
    uint8_t i=0;

    *pLen=strlen(pCode);
    *pLenEffective=*pLen;

    //printf("c0=%1X val=%1X\n", pCode[0], pCode[0]-'0');
    //printf("c1=%1X val=%1X\n", pCode[1], (pCode[1]>'9'?pCode[1]-55:pCode[1]-'0'));

    while (i<*pLen)
    {
        if (pCode[i]<'0' || (pCode[i]>'9'))
        {
            *pLenEffective=i;
            i=*pLen;
        }
        else
        {
            decValue=decValue*10+pCode[i]-'0';
        }

        i++;
    }

    return decValue;
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

        case REGPC:
            regReturn=&(mRegisterPack.regPC);
            break;

        case REGIX:
            regReturn=&(mRegisterPack.regIX);
            break;

        case REGIY:
            regReturn=&(mRegisterPack.regIY);
            break;

        case REGAFP:
            regReturn=&(mRegisterPack.regAFp);
            break;

        case REGBCP:
            regReturn=&(mRegisterPack.regBCp);
            break;
        
        case REGDEP:
            regReturn=&(mRegisterPack.regDEp);
            break;
        
        case REGHLP:
            regReturn=&(mRegisterPack.regHLp);
            break;
    }

    return(regReturn);
}


/* Clean the line from an asm  file */
void Z80Machine::clean_line(char *pStr)
{
    bool noChar=true;
    uint8_t i=0;
    uint8_t j=0;  
    char retStr[MAX_INSTR_LENGTH];

    for (i=0; i<strlen(pStr); i++)
    {
        //printf("<%c>", pStr[i]);
        if (pStr[i]!=' ' && pStr[i]!='\t')
        {
            noChar=false;
            //printf("false\n");
        }

        if (!noChar && pStr[i]!=10)
        {
            retStr[j++]=pStr[i];   
        }
    }

    retStr[j]='\0';
    toUpper(retStr);

    strcpy(pStr, retStr);
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


/* Clean the nn operand    */
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

    return retCode;
}


/* Clean a cc operand   */
int8_t Z80Machine::clean_cc(char *pOp)
{
    return 0;
}


/* Clean the (nn) operand    */
int8_t Z80Machine::clean_inn(char *pOp)
{
    uint8_t retCode=ERR_NO_ERROR;
    char *posChar;
    char op[MAX_OP_LENGTH*3];
    uint8_t len=0;

    //printf("op_in =<%s>\n", pOp);

    strcpy(op,pOp+1);                  /* Remove '(' and ')'   */
    op[strlen(op)-1]='\0';
    
    retCode=clean_nn(op);

    strcpy(pOp, op);
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

    return retCode;
}


/* Clean the r operand  */
int8_t Z80Machine::clean_r(char *pOp)
{
    uint8_t retCode=ERR_NO_ERROR;

    if (strlen(pOp)>1)
    {
        retCode=ERR_BAD_OPERAND;
    }
    else 
    {
        if (pOp[0]!='A' && pOp[0]!='B' && pOp[0]!='C' && pOp[0]!='D' && pOp[0]!='E' && pOp[0]!='F' && pOp[0]!='H' && pOp[0]!='L')
        {
            retCode=ERR_BAD_OPERAND;
        }
    }

    return retCode;
}


/* Clean the rr operand  */
int8_t Z80Machine::clean_rr(char *pOp)
{
    uint8_t retCode=ERR_NO_ERROR;

        if (strlen(pOp)!=2)
    {
        retCode=ERR_BAD_OPERAND;
    }
    else 
    {
        if (strcmp(pOp, "BC") && strcmp(pOp, "DE") && strcmp(pOp, "HL") && strcmp(pOp, "AF") && strcmp(pOp, "IX") && strcmp(pOp, "IY"))
        {
            retCode=ERR_BAD_OPERAND;
        }
    }
    return retCode;
}


/* Clean the e operand  */
int8_t Z80Machine::clean_e(char *pOp)
{
    uint8_t retCode=ERR_NO_ERROR;
    bool cont=true;
    uint8_t i=0;
    uint16_t e=0;

    if (pOp[0]!='$' || (pOp[1]!='+' && pOp[1]!='-') || strlen(pOp)<3 || strlen(pOp)>5)
    {
        retCode=ERR_BAD_OPERAND;
        cont=false;
    }

    i=2;
    
    /* Remove all the non-decimal digits   */
    while (cont)
    {
        if (pOp[i]>'9' || pOp[i]<'0')
        {
            cont=false;
            pOp[i]='\0';
        }
        
        if (i>strlen(pOp))
        {
            cont=false;
        } 

        i++;
    }

    for (i=2; i<strlen(pOp); i++)
    {
        e=e*10+(pOp[i]-'0');
    }

    if (strlen(pOp)<3 || (pOp[1]=='-' && e>126) || (pOp[1]=='+' && e>129))
    {
        retCode=ERR_BAD_OPERAND;
    }

    return retCode;
}


/* Give the execution mode.     */
bool Z80Machine::getExecutionMode()
{
    return mExecMode;
}

/*
void Z80Machine::displayRegB()
{
    // printf("bool=%d\n", mRegisterPack.regB.hasJustChanged()?1:0);

    if (mRegisterPack.regB.hasJustChanged())
    {
        printf("\033[0mB  [\033[33m%02X\033[0m]      ", mRegisterPack.regB.getValue());
    }
    else
    {
        printf("B  [%02X]      ", mRegisterPack.regB.getValue());
    }
}

void Z80Machine::displayRegC()
{
    // printf("bool=%d\n", mRegisterPack.regB.hasJustChanged()?1:0);

    if (mRegisterPack.regC.hasJustChanged())
    {
        printf("\033[0mC  [\033[33m%02X\033[0m]      ", mRegisterPack.regC.getValue());
    }
    else
    {
        printf("C  [%02X]      ", mRegisterPack.regC.getValue());
    }
}

void Z80Machine::displayRegD()
{
    // printf("bool=%d\n", mRegisterPack.regB.hasJustChanged()?1:0);

    if (mRegisterPack.regB.hasJustChanged())
    {
        printf("\033[0mB  [\033[33m%02X\033[0m]      ", mRegisterPack.regD.getValue());
    }
    else
    {
        printf("B  [%02X]      ", mRegisterPack.regD.getValue());
    }
}


void Z80Machine::displayRegE()
{
    // printf("bool=%d\n", mRegisterPack.regB.hasJustChanged()?1:0);

    if (mRegisterPack.regE.hasJustChanged())
    {
        printf("\033[0mC  [\033[33m%02X\033[0m]      ", mRegisterPack.regE.getValue());
    }
    else
    {
        printf("C  [%02X]      ", mRegisterPack.regE.getValue());
    }
}*/


void Z80Machine::displayReg8Bits(Register_8bits *pReg, const char *pCharReg)
{
    // printf("bool=%d\n", pReg->hasJustChanged()?1:0);

    if (pReg->hasJustChanged())
    {
        printf("\033[0m%s   [\033[33m%02X\033[0m]      ", pCharReg, pReg->getValue());
    }
    else
    {
        printf("%s   [%02X]      ", pCharReg, pReg->getValue());
    }
}


void Z80Machine::displayReg16Bits(Register_16bits *pReg, const char *pCharReg)
{
    // printf("bool=%d\n", pReg->hasJustChanged()?1:0);
    if (pReg->hasJustChanged())
    {
        printf("\033[0m%s [\033[33m%04X\033[0m]    ", pCharReg, pReg->getValue());
    }
    else
    {
        printf("%s [%04X]    ", pCharReg, pReg->getValue());
    }
}

void Z80Machine::displayDetailsRegisterF()
{
    /* xxxjoexxx - continue the display below   */
    printf("\b\b\b\b\b\033[0m[");

    /* Display colored SIGN flag    */
    if (mRegisterPack.regF.getColorChangedFlag() & MASK_FLAG_SIGN)
    {
        printf("\033[33m%d\033[0m", mRegisterPack.regF.getSignFlag());
    }
    else
    {
        printf("%d", mRegisterPack.regF.getSignFlag());
    }

    /* Display colored ZERO flag    */
    if (mRegisterPack.regF.getColorChangedFlag() & MASK_FLAG_ZERO)
    {
        printf("\033[33m%d\033[0m", mRegisterPack.regF.getZeroFlag());
    }
    else
    {
        printf("%d", mRegisterPack.regF.getZeroFlag());
    }

    /* Display void flag    */
    printf("\033[0m0");

    /* Display colored HALF CARRY flag    */
    if (mRegisterPack.regF.getColorChangedFlag() & MASK_FLAG_HALF_CARRY)
    {
        printf("\033[33m%d\033[0m", mRegisterPack.regF.getHalfCarryFlag());
    }
    else
    {
        printf("%d", mRegisterPack.regF.getHalfCarryFlag());
    }

    /* Display void flag    */
    printf("\033[0m0");

    /* Display colored PARITY/OVERFLOW flag    */
    if (mRegisterPack.regF.getColorChangedFlag() & MASK_FLAG_PARITY_OVERFLOW)
    {
        printf("\033[33m%d\033[0m", mRegisterPack.regF.getParityOverflowFlag());
    }
    else
    {
        printf("%d", mRegisterPack.regF.getParityOverflowFlag());
    }

    /* Display void flag    */
    printf("\033[0m0");

    /* Display colored ADD/SUBTRACT flag    */
    if (mRegisterPack.regF.getColorChangedFlag() & MASK_FLAG_ADD_SUBTRACT)
    {
        printf("\033[33m%d\033[0m", mRegisterPack.regF.getAddSubtractFlag());
    }
    else
    {
        printf("%d", mRegisterPack.regF.getAddSubtractFlag());
    }

    /* Display colored CARRY flag    */
    if (mRegisterPack.regF.getColorChangedFlag() & MASK_FLAG_CARRY)
    {
        printf("\033[33m%d\033[0m", mRegisterPack.regF.getCarryFlag());
    }
    else
    {
        printf("%d", mRegisterPack.regF.getCarryFlag());
    }

    printf("\033[0m]");

    /* Display colored SIGN flag    */
    printf(" [S:");

    if (mRegisterPack.regF.getColorChangedFlag() & MASK_FLAG_SIGN)
    {
        printf("\033[33m%d\033[0m", mRegisterPack.regF.getSignFlag());
    }
    else
    {
        printf("%d", mRegisterPack.regF.getSignFlag());
    }

    /* Display colored ZERO flag    */
    printf(" Z:");

    if (mRegisterPack.regF.getColorChangedFlag() & MASK_FLAG_ZERO)
    {
        printf("\033[33m%d\033[0m", mRegisterPack.regF.getZeroFlag());
    }
    else
    {
        printf("%d", mRegisterPack.regF.getZeroFlag());
    }

    /* Display colored HALF CARRY flag    */
    printf(" H:");

    if (mRegisterPack.regF.getColorChangedFlag() & MASK_FLAG_HALF_CARRY)
    {
        printf("\033[33m%d\033[0m", mRegisterPack.regF.getHalfCarryFlag());
    }
    else
    {
        printf("%d", mRegisterPack.regF.getHalfCarryFlag());
    }

    /* Display colored PARITY/OVERFLOW flag    */
    printf(" PV:");

    if (mRegisterPack.regF.getColorChangedFlag() & MASK_FLAG_PARITY_OVERFLOW)
    {
        printf("\033[33m%d\033[0m", mRegisterPack.regF.getParityOverflowFlag());
    }
    else
    {
        printf("%d", mRegisterPack.regF.getParityOverflowFlag());
    }

    /* Display colored ADD/SUBTRACT flag    */
    printf(" N:");

    if (mRegisterPack.regF.getColorChangedFlag() & MASK_FLAG_ADD_SUBTRACT)
    {
        printf("\033[33m%d\033[0m", mRegisterPack.regF.getAddSubtractFlag());
    }
    else
    {
        printf("%d", mRegisterPack.regF.getAddSubtractFlag());
    }

    /* Display colored CARRY flag    */
    printf(" C:");

    if (mRegisterPack.regF.getColorChangedFlag() & MASK_FLAG_CARRY)
    {
        printf("\033[33m%d\033[0m", mRegisterPack.regF.getCarryFlag());
    }
    else
    {
        printf("%d", mRegisterPack.regF.getCarryFlag());
    }

    printf("\033[0m]\n");
}

/* Display registers            */
void Z80Machine::displaySimpleRegisters()
{
    printf("\n");
    //printf("[\033[31mTA\033[0m][\033[32m32\033[0m][\033[33m33\033[0m][\033[34m34\033[0m][\033[35m35\033[0m]\n"); 
    displayReg8Bits(REGISTER_B, STRING_REGB);
    displayReg8Bits(REGISTER_C, STRING_REGC);
    printf("\n");
    displayReg8Bits(REGISTER_D, STRING_REGD);
    displayReg8Bits(REGISTER_E, STRING_REGE);
    printf("\n");
    displayReg8Bits(REGISTER_H, STRING_REGH);
    displayReg8Bits(REGISTER_L, STRING_REGL);
    printf("\n");
    displayReg8Bits(REGISTER_A, STRING_REGA);
    displayReg8Bits(REGISTER_F, STRING_REGF);

    displayDetailsRegisterF();

    printf("\n");

    displayReg16Bits(REGISTER_BC, "BC ");
    displayReg16Bits(REGISTER_DE, "DE ");
    printf("\n");

    displayReg16Bits(REGISTER_HL, "HL ");
    displayReg16Bits(REGISTER_AF, "AF ");
    printf("\n");

    displayReg16Bits(REGISTER_IX, "IX ");
    displayReg16Bits(REGISTER_IY, "IY ");
    printf("\n");
    printf("\n");

    displayReg16Bits(REGISTER_PC, "PC ");
    displayReg16Bits(REGISTER_SP, "SP ");
    printf("\n");
}

/* Display details registers            */
void Z80Machine::displayAllRegisters()
{
    printf("\n");
    //printf("[\033[31m31\033[0m][\033[32m32\033[0m][\033[33m33\033[0m][\033[34m34\033[0m][\033[35m35\033[0m]\n"); 
    displayReg8Bits(REGISTER_B, STRING_REGB);
    displayReg8Bits(REGISTER_C, STRING_REGC);
    printf("\n");
    displayReg8Bits(REGISTER_D, STRING_REGD);
    displayReg8Bits(REGISTER_E, STRING_REGE);
    printf("\n");
    displayReg8Bits(REGISTER_H, STRING_REGH);
    displayReg8Bits(REGISTER_L, STRING_REGL);
    printf("\n");
    displayReg8Bits(REGISTER_A, STRING_REGA);
    displayReg8Bits(REGISTER_F, STRING_REGF);

    /* Display details registers            */
    displayDetailsRegisterF();
    
    displayReg8Bits(REGISTER_I, STRING_REGI);
    displayReg8Bits(REGISTER_R, STRING_REGR);
    printf("\n");

    /*
    printf("\b\b\b\b\b[%s] [S:%d Z:%d H:%d PV:%d N:%d C:%d]\n", 
        byteToBinary(mRegisterPack.regF.getValue()), 
        mRegisterPack.regF.getSignFlag(), mRegisterPack.regF.getZeroFlag(), 
        mRegisterPack.regF.getHalfCarryFlag(), mRegisterPack.regF.getParityOverflowFlag(),
        mRegisterPack.regF.getAddSubtractFlag(), mRegisterPack.regF.getCarryFlag());
    */

    printf("\n");

    displayReg16Bits(REGISTER_BC, "BC ");
    displayReg16Bits(REGISTER_DE, "DE ");
    displayReg16Bits(REGISTER_BCp, "BC'");
    displayReg16Bits(REGISTER_DEp, "DE'");
    printf("\n");

    displayReg16Bits(REGISTER_HL, "HL ");
    displayReg16Bits(REGISTER_AF, "AF ");
    displayReg16Bits(REGISTER_HLp, "HL'");
    displayReg16Bits(REGISTER_AFp, "AF'");
    printf("\n");

    displayReg16Bits(REGISTER_IX, "IX ");
    displayReg16Bits(REGISTER_IY, "IY ");
    printf("\n");
    printf("\n");

    displayReg16Bits(REGISTER_PC, "PC ");
    displayReg16Bits(REGISTER_SP, "SP ");
    printf("\n");
}


/* Display registers in exec mode           */
void Z80Machine::displayExecRegisters()
{
    printf("\n");
    displayReg8Bits(REGISTER_B, STRING_REGB);
    displayReg8Bits(REGISTER_C, STRING_REGC);
    displayReg8Bits(REGISTER_D, STRING_REGD);
    displayReg8Bits(REGISTER_E, STRING_REGE);
    displayReg8Bits(REGISTER_H, STRING_REGH);
    displayReg8Bits(REGISTER_L, STRING_REGL);
    displayReg8Bits(REGISTER_A, STRING_REGA);
    displayReg8Bits(REGISTER_F, STRING_REGF);
    
    /* Display details registers            */
    displayDetailsRegisterF();

    /*
    printf("\b\b\b\b\b[%s] [S:%d Z:%d H:%d PV:%d N:%d C:%d]\n", 
            byteToBinary(mRegisterPack.regF.getValue()), mRegisterPack.regF.getSignFlag(), 
            mRegisterPack.regF.getZeroFlag(), mRegisterPack.regF.getHalfCarryFlag(), 
            mRegisterPack.regF.getParityOverflowFlag(), mRegisterPack.regF.getAddSubtractFlag(), 
            mRegisterPack.regF.getCarryFlag());
    */

    displayReg16Bits(REGISTER_BC, "BC ");
    displayReg16Bits(REGISTER_DE, "DE ");
    displayReg16Bits(REGISTER_HL, "HL ");
    displayReg16Bits(REGISTER_AF, "AF ");
    displayReg16Bits(REGISTER_IX, "IX ");
    displayReg16Bits(REGISTER_IY, "IY ");
    printf("\n");

    /*printf("BC  [%04X]    DE  [%04X]    HL  [%04X]    AF  [%04X]    IX  [%04X]    IY  [%04X]\n", 
        mRegisterPack.regBC.getValue(), mRegisterPack.regDE.getValue(),
        mRegisterPack.regHL.getValue(), mRegisterPack.regAF.getValue(),
        mRegisterPack.regIX.getValue(), mRegisterPack.regIY.getValue());*/

    displayReg16Bits(REGISTER_BCp, "BC'");
    displayReg16Bits(REGISTER_DEp, "DE'");
    displayReg16Bits(REGISTER_HLp, "HL'");
    displayReg16Bits(REGISTER_AFp, "AF'");
    printf("\n");

    displayReg16Bits(REGISTER_PC, "PC ");
    displayReg16Bits(REGISTER_SP, "SP ");
    printf("\n");
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
            if (!strcmp(pAddress, "(IX)"))
            {
                value=mRegisterPack.regIX.getValue();
            }
            else
            {
                if (!strcmp(pAddress, "(IY)"))
                {
                    value=mRegisterPack.regIY.getValue();
                }
                else
                {
                    if (!strcmp(pAddress, "(HL)"))
                    {
                        value=mRegisterPack.regHL.getValue();
                    }
                    else
                    {
                        value=toValue(address, &lenValue, &lenEff);
                    }
                }
            }
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


/* Load a file with codes or instructions   */
void Z80Machine::loadCode(char *pFilename)
{
    FILE *file=NULL;
    bool notTheEnd=true;
    char aLine[MAX_INSTR_LENGTH];
    char label[MAX_INSTR_LENGTH];
    uint8_t len=-1;
    uint8_t lenAddr=-1;
    uint8_t lenEff=0;
    int8_t retCheck=0;
    uint32_t machineCode=0;
    uint16_t address=0x0000;
    uint16_t labelAddress=0x0000;
    int16_t delta=0;
    char *posChar=0;
    LabelDataset labelDataset;
    int8_t pos1=0, pos2=0;
    int8_t i=0;
    char sepChar=' ';

    if (!strstr(pFilename, ".asm"))                         /* Add .asm if not present  */
    {
        strcat(pFilename, ".asm");
    }

    if (!(file=fopen(pFilename, "rw")))
    {
        printf(" Cannot open <%s>, please check it.\n", pFilename);
    }
    else
    {
        printf(" OK, opening <%s>.\n", pFilename);

        /* Retrieve and decode all the labels into the file    */
        while (fgets(aLine, MAX_INSTR_LENGTH, file) != NULL)
        {
            clean_line(aLine);

            if (strlen(aLine)!=0)
            {
                if (strstr(aLine, "ORG"))
                {
                    if (posChar=strchr(aLine, '#'))
                    {                  
                        /* Get address and set address and PC here  */
                        retCheck=clean_nn(posChar);                         /* Clean the (nn) operand   */
                        address=toValue(posChar+1, &lenAddr, &lenEff);

                        mRegisterPack.regPC.setValue(address);
                    }
                }

                if (posChar=strchr(aLine, ':'))                 /* Find label in the line   */
                {
                    strcpy(label, aLine);       
                    label[posChar-aLine]='\0';

                    /* Add the label and its address into the dataset       */
                    labelDataset.add(Label(std::string(label), address));
                    // printf("Add %s / #%04X\n", label, address);

                    /* Keep the instruction that is on the line             */
                    strcpy(label, &aLine[posChar-aLine+1]);
                    strcpy(aLine, label);
                    clean_line(aLine);
                }

                machineCode=findMachineCode(aLine, &len);
    
#ifdef DEBUG_DISPLAY_READ_ASM_DATA
                printf("[%08X] - %s - (len=%d) is at %04X\n", machineCode, aLine, len, address);
#endif

                /* Go to the next instruction   */
                if (len!=NOT_DECODED)
                {
                    address+=(len/2);
                }
            }
        }

        /* Charge codes into memory */
        address=mRegisterPack.regPC.getValue();
        fseek(file, SEEK_SET, 0);

        while (fgets(aLine, MAX_INSTR_LENGTH, file) != NULL)
        {
            clean_line(aLine);

            if (strlen(aLine)!=0)
            {
                if (posChar=strchr(aLine, ':'))                 /* Find label in the line   */
                {
                    strcpy(label, aLine);       
                    label[posChar-aLine]='\0';

                    /* Keep the instruction that is on the line             */
                    strcpy(label, &aLine[posChar-aLine+1]);
                    strcpy(aLine, label);
                    clean_line(aLine);
                }

                /* Is it a JP or a CALL     */
                if (strstr(aLine, "JP") || strstr(aLine, "CALL")) 
                {
                    if (!strchr(aLine, '#') && !strchr(aLine, ')' && !strchr(aLine, '(')))           /* This is a label          */
                    {
                        std::string strLine(aLine);

                        /* Find the last ' ' or ','    */
                        pos1=strLine.rfind(' ');
                        pos2=strLine.rfind(',');

                        if (pos1+pos2)                      /* ' ' or ',' was found     */
                        {
                            /* Find the last ' ' or ',' */
                            if (pos1>pos2)
                            {
                                sepChar=' ';
                            }
                            else 
                            {
                                pos1=pos2;
                                sepChar=',';
                            }

                            strcpy(label, &aLine[pos1+1]);
                            strLine.resize(pos1);
                            labelAddress=labelDataset.findAddress(label);

                            sprintf(aLine, "%s%c#%04X", strLine.c_str(), sepChar, labelAddress);
                        }

                    }
                }

                if (strstr(aLine, "JR") || strstr(aLine, "DJNZ")) 
                {
                    if (!strchr(aLine, '$') && !strchr(aLine, '+' && !strchr(aLine, '-')))           /* This is a label          */
                    {
                        std::string strLine(aLine);

                        /* Find the last ' ' or ','    */
                        pos1=strLine.rfind(' ');
                        pos2=strLine.rfind(',');

                        if (pos1+pos2)                      /* ' ' or ',' was found     */
                        {
                            /* Find the last ' ' or ',' */
                            if (pos1>pos2)
                            {
                                sepChar=' ';
                            }
                            else 
                            {
                                pos1=pos2;
                                sepChar=',';
                            }

                            strcpy(label, &aLine[pos1+1]);
                            strLine.resize(pos1);
                            labelAddress=labelDataset.findAddress(label);
                            delta=labelAddress-address;

#ifdef DEBUG_DISPLAY_LABEL_ASM_DATA
                            printf("address=%04X / label=%s / label @=%04X / delta=%d\n", address, label, labelAddress, delta);
#endif 
                            if (delta>=0)
                            {
                                sprintf(aLine, "%s%c$%c%d", strLine.c_str(), sepChar, '+', delta);
                            }
                            else
                            {
                                sprintf(aLine, "%s%c$%c%d", strLine.c_str(), sepChar, '-', -delta);
                            }
                        }
                    }
                }

                machineCode=findMachineCode(aLine, &len);

                if (machineCode!=NOT_DECODED)
                {
                    for (i=(len/2)-1; i>=0; i--)
                    {
                        mMemory->set8bitsValue(address+i, machineCode & FIRST_LOWEST_BYTE);
                        machineCode=machineCode >> SIZE_1_BYTE;
                    }
                    
                    address+=(len/2);
                }
            }
        }

        fclose(file);
    }
}

/* Interpret the machine code   */
uint8_t Z80Machine::interpretCode(uint32_t codeInHexa, uint8_t len, uint8_t pMode)
{
    //uint32_t codeInHexa;
    //uint8_t len=0;
    uint8_t lenEff=0;
    uint8_t op1=0;
    int16_t op1s=0;
    uint8_t op2=0;
    uint16_t op16=0;
    //uint16_t instruction=CODE_NO_INSTRUCTION;
    uint32_t instruction=CODE_NO_INSTRUCTION;
    Register_8bits *reg8_1=NULL;
    Register_8bits *reg8_2=NULL;
    Register_16bits *reg16_1=NULL;
    Register_16bits *reg16_2=NULL;
    uint16_t address=0x0000;
    uint8_t newVal=0;
    uint8_t val=0;
    uint8_t bit=0;
    char strInstr[MAX_OP_LENGTH*3];
    uint8_t carry=0;
    uint8_t ret=NOTHING_SPECIAL;
    uint8_t retInterpret=NOTHING_SPECIAL;
    char sop1[MAX_OP_LENGTH], sop2[MAX_OP_LENGTH];

#ifdef DEBUG_DISPLAY_INSTR_DATA 
    printf("code=<%08X> / len=%d\n", codeInHexa, len);
    //printf("ColorFlagChanged=%s\n", byteToBinary(mRegisterPack.regF.getColorChangedFlag()));
#endif
    
    /* bottom 0 */

    sprintf(mInstruction, "not yet decoded ");

    /* This is a NOP    */
    if ((codeInHexa & MASK_NOP)==CODE_NOP && len == NATURAL_CODE_LENGTH(CODE_NOP))                  
    {
        instruction=CODE_NOP;
    }

    //printf("%02x %02x\n", (codeInHexa & MASK_LDRR), CODE_LDRR);     
    
    /* This is a LD r,r' */
    if ((codeInHexa & MASK_LDRR)==CODE_LDRR && len == NATURAL_CODE_LENGTH(CODE_LDRR))
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
    if ((codeInHexa & MASK_LDHLR)==CODE_LDHLR && len == NATURAL_CODE_LENGTH(CODE_LDHLR))
    {
        instruction=CODE_LDHLR;
        
        /* Extract the value of the register (in bits)    */
        op1=REGIHL;
        op2=EXTRACT(codeInHexa, 0, 3);
    }

    /* This is a LD r,(HL)  */
    if ((codeInHexa & MASK_LDRHL)==CODE_LDRHL && len == NATURAL_CODE_LENGTH(CODE_LDRHL))
    {
        instruction=CODE_LDRHL;
        
        /* Extract the value of the register (in bits)    */
        op1=EXTRACT(codeInHexa, 3, 3);
        op2=REGIHL;
    }

    /* This is a LD r,n  */
    if (((codeInHexa>>SIZE_1_BYTE) & MASK_LDRN)==CODE_LDRN && len == NATURAL_CODE_LENGTH(CODE_LDRN))
    {
        instruction=CODE_LDRN;
        
        /* Extract the value of the register (in bits)    */
        op1=EXTRACT(codeInHexa, 11, 3);
        op2=codeInHexa & 0xFF;
    }

    /* This is a LD (HL),n  */
    if ((codeInHexa>>SIZE_1_BYTE & MASK_LDHLN)==CODE_LDHLN && len == NATURAL_CODE_LENGTH(CODE_LDHLN))
    {
        instruction=CODE_LDHLN;
        
        /* Extract the value of the operand #2 (n)    */
        op2=codeInHexa & FIRST_LOWEST_BYTE;
    }

    /* This is a HALT    */
    if ((codeInHexa & MASK_HALT)==CODE_HALT && (len == NATURAL_CODE_LENGTH(CODE_HALT)))                  
    {
        instruction=CODE_HALT;
    }

    /* This is a LD A,(BC) */
    if ((codeInHexa & MASK_LDABC)==CODE_LDABC && (len == NATURAL_CODE_LENGTH(CODE_LDABC)))
    {
        instruction=CODE_LDABC;
    }

    /* This is a LD A,(DE) */
    if ((codeInHexa & MASK_LDADE)==CODE_LDADE && (len == NATURAL_CODE_LENGTH(CODE_LDADE)))
    {
        instruction=CODE_LDADE;
    }

    /* This is a LD A,(nn)  */
    if ((codeInHexa>>SIZE_2_BYTES & MASK_LDANN)==CODE_LDANN && len == NATURAL_CODE_LENGTH(CODE_LDANN))
    {
        instruction=CODE_LDANN;
        
        /* Extract the value of the operand #2 (nn)    */
        op1=codeInHexa & FIRST_LOWEST_BYTE;
        op2=(codeInHexa & SECOND_LOWEST_BYTE) >> SIZE_1_BYTE;
    }

    /* This is a LD (nn),A  */
    if ((codeInHexa>>SIZE_2_BYTES & MASK_LDNNA)==CODE_LDNNA && (len == NATURAL_CODE_LENGTH(CODE_LDNNA)))
    {
        instruction=CODE_LDNNA;
        
        /* Extract the value of the operand #2 (nn)    */
        op1=codeInHexa & FIRST_LOWEST_BYTE;
        op2=(codeInHexa & SECOND_LOWEST_BYTE) >> SIZE_1_BYTE;
    }

    /* This is a LD (BC),A  */
    if ((codeInHexa & MASK_LDBCA)==CODE_LDBCA && (len == NATURAL_CODE_LENGTH(CODE_LDBCA)))
    {
        instruction=CODE_LDBCA;
        
    }

    /* This is a LD (DE),A  */
    if ((codeInHexa & MASK_LDDEA)==CODE_LDDEA && (len == NATURAL_CODE_LENGTH(CODE_LDDEA)))
    {
        instruction=CODE_LDDEA;
        
    }

    /* This is a LD r,(IX+d)    */
    if (((codeInHexa>>SIZE_1_BYTE) & MASK_LDRIXD)==CODE_DD_LDRIXD && len == DD_CODE_LENGTH(CODE_DD_LDRIXD))
    {
        instruction=CODE_DD_LDRIXD; 
        op1=EXTRACT(codeInHexa, 11, 3);
        op2=codeInHexa & FIRST_LOWEST_BYTE;
    }

    /* This is a LD rr,nn    */
    if ((codeInHexa>>SIZE_2_BYTES & MASK_LDRRNN)==CODE_LDRRNN && (len == NATURAL_CODE_LENGTH(CODE_LDRRNN)))
    {
        instruction=CODE_LDRRNN;
        
        /* Extract the value of the operand #1 (rr) and #2 (nn)    */
        op1=EXTRACT(codeInHexa, 20,2) | 0b1000;

        op16=((codeInHexa & FIRST_LOWEST_BYTE)<<SIZE_1_BYTE)+((codeInHexa & SECOND_LOWEST_BYTE)>>SIZE_1_BYTE);
    }

    /* This is a LD r,(IY+d)    */
    if (((codeInHexa>>SIZE_1_BYTE) & MASK_LDRIYD)==CODE_FD_LDRIYD && len == FD_CODE_LENGTH(CODE_FD_LDRIYD))
    {
        instruction=CODE_FD_LDRIYD; 
        op1=EXTRACT(codeInHexa, 11, 3);
        op2=codeInHexa & FIRST_LOWEST_BYTE;
    }

    /* This is a LD (IX+d),r    */
    if (((codeInHexa>>SIZE_1_BYTE) & MASK_LDIXDR)==CODE_DD_LDIXDR && len == DD_CODE_LENGTH(CODE_DD_LDIXDR))
    {
        instruction=CODE_DD_LDIXDR;
        op1=codeInHexa & FIRST_LOWEST_BYTE;
        op2=EXTRACT(codeInHexa, 8, 3);
    }

    /* This is a LD (IY+d),r    */
    if (((codeInHexa>>SIZE_1_BYTE) & MASK_LDIYDR)==CODE_FD_LDIYDR && len == DD_CODE_LENGTH(CODE_FD_LDIYDR))
    {
        instruction=CODE_FD_LDIYDR; 
        op1=codeInHexa & FIRST_LOWEST_BYTE;
        op2=EXTRACT(codeInHexa, 8, 3);
    }

    /* This is a LD IX,nn    */
    if (((codeInHexa>>SIZE_2_BYTES) & MASK_LDIXNN)==CODE_DD_LDIXNN && len == DD_CODE_LENGTH(CODE_DD_LDIXNN))
    {
        instruction=CODE_DD_LDIXNN; 
        op16=((codeInHexa & FIRST_LOWEST_BYTE)<<SIZE_1_BYTE)+((codeInHexa & SECOND_LOWEST_BYTE)>>SIZE_1_BYTE);
    }

    /* This is a LD IY,nn    */
    if (((codeInHexa>>SIZE_2_BYTES) & MASK_LDIYNN)==CODE_FD_LDIYNN && len == FD_CODE_LENGTH(CODE_FD_LDIYNN))
    {
        instruction=CODE_FD_LDIYNN; 
        op16=((codeInHexa & FIRST_LOWEST_BYTE)<<SIZE_1_BYTE)+((codeInHexa & SECOND_LOWEST_BYTE)>>SIZE_1_BYTE);
    }

    /* This is a LD dd,(nn)    */
    if (((codeInHexa>>SIZE_2_BYTES) & MASK_LDDDNN)==CODE_ED_LDDDNN && len == ED_CODE_LENGTH(CODE_ED_LDDDNN))
    {
        instruction=CODE_ED_LDDDNN; 
        op1=EXTRACT(codeInHexa, 20, 2) | 0b1000;
        op16=((codeInHexa & FIRST_LOWEST_BYTE)<<SIZE_1_BYTE)+((codeInHexa & SECOND_LOWEST_BYTE)>>SIZE_1_BYTE);
    }

    /* This is a LD HL,(nn)    */
    if ((codeInHexa>>SIZE_2_BYTES & MASK_LDHLNN)==CODE_LDHLNN && (len == NATURAL_CODE_LENGTH(CODE_LDHLNN)))
    {
        instruction=CODE_LDHLNN; 
        op16=((codeInHexa & FIRST_LOWEST_BYTE)<<SIZE_1_BYTE)+((codeInHexa & SECOND_LOWEST_BYTE)>>SIZE_1_BYTE);
    }

    /* This is a LD IX,(nn)    */
    if (((codeInHexa>>SIZE_2_BYTES) & MASK_LDIXANN)==CODE_DD_LDIXANN && len == DD_CODE_LENGTH(CODE_DD_LDIXANN))
    {
        instruction=CODE_DD_LDIXANN; 
        op16=((codeInHexa & FIRST_LOWEST_BYTE)<<SIZE_1_BYTE)+((codeInHexa & SECOND_LOWEST_BYTE)>>SIZE_1_BYTE);
    }

    /* This is a LD IY,(nn)    */
    if (((codeInHexa>>SIZE_2_BYTES) & MASK_LDIYANN)==CODE_FD_LDIYANN && len == DD_CODE_LENGTH(CODE_FD_LDIYANN))
    {
        instruction=CODE_FD_LDIYANN; 
        op16=((codeInHexa & FIRST_LOWEST_BYTE)<<SIZE_1_BYTE)+((codeInHexa & SECOND_LOWEST_BYTE)>>SIZE_1_BYTE);
    }

    /* This is a LD (nn),HL    */
    if ((codeInHexa>>SIZE_2_BYTES & MASK_LDNNHL)==CODE_LDNNHL && len == NATURAL_CODE_LENGTH(CODE_LDNNHL))
    {
        instruction=CODE_LDNNHL; 
        op16=((codeInHexa & FIRST_LOWEST_BYTE)<<SIZE_1_BYTE)+((codeInHexa & SECOND_LOWEST_BYTE)>>SIZE_1_BYTE);
    }

    /* This is a INC r */
    if ((codeInHexa & MASK_INCR)==CODE_INCR && (len == NATURAL_CODE_LENGTH(CODE_INCR)))
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

    /* This is a PUSH qq */
    if ((codeInHexa & MASK_PUSHQQ)==CODE_PUSHQQ && len == NATURAL_CODE_LENGTH(CODE_PUSHQQ))
    {
        instruction=CODE_PUSHQQ;
               
        op1=EXTRACT(codeInHexa, 4, 2) | 0b1000;

        if (op1==REGSP)                          /* This is the case of qq operand       */
        {                                           
            op1=REGAF;
        }
    }

    /* This is a POP qq */
    if ((codeInHexa & MASK_POPQQ)==CODE_POPQQ && len == NATURAL_CODE_LENGTH(CODE_POPQQ))
    {
        instruction=CODE_POPQQ;
               
        op1=EXTRACT(codeInHexa, 4, 2) | 0b1000;

        if (op1==REGSP)                          /* This is the case of qq operand       */
        {                                           
            op1=REGAF;
        }
    }

    /* This is a POP IX */
    if ((codeInHexa & MASK_POPIX)==CODE_DD_POPIX && len == DD_CODE_LENGTH(CODE_DD_POPIX))
    {
        instruction=CODE_DD_POPIX;
    }

        /* This is a POP IY */
    if ((codeInHexa & MASK_POPIY)==CODE_FD_POPIY && len == FD_CODE_LENGTH(CODE_FD_POPIY))
    {
        instruction=CODE_FD_POPIY;
    }

    /* This is a PUSH IX */
    if ((codeInHexa & MASK_PUSHIX)==CODE_DD_PUSHIX && len == DD_CODE_LENGTH(CODE_DD_PUSHIX))
    {
        instruction=CODE_DD_PUSHIX;
    }

    /* This is a PUSH IY */
    if ((codeInHexa & MASK_PUSHIY)==CODE_FD_PUSHIY && len == FD_CODE_LENGTH(CODE_FD_PUSHIY))
    {
        instruction=CODE_FD_PUSHIY;
    }

    /* This is a LD (nn),rr    */
    if (((codeInHexa>>SIZE_2_BYTES)  & MASK_LDNNRR)==CODE_ED_LDNNRR && len==ED_CODE_LENGTH(CODE_ED_LDNNRR))
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
    if (((codeInHexa & FIRST_TWO_LOWEST_BYTES) & MASK_RLCR)==CODE_CB_RLCR && len == CB_CODE_LENGTH(CODE_CB_RLCR))
    {
        instruction=CODE_CB_RLCR;
        
        /* Extract the value of the register    */
        op1=EXTRACT(codeInHexa, 0,3);
    }

    /* This is a RLC (HL)  */
    if (((codeInHexa & FIRST_TWO_LOWEST_BYTES) & MASK_RLCHL)==CODE_CB_RLCHL && len == CB_CODE_LENGTH(CODE_CB_RLCHL))
    {
        instruction=CODE_CB_RLCHL;

    }

    /* This is a RRCA  */
    if ((codeInHexa & MASK_RRCA)==CODE_RRCA && len==NATURAL_CODE_LENGTH(CODE_RRCA))
    {
        instruction=CODE_RRCA;
    }

    /* This is a RRC r  */
    if (((codeInHexa & FIRST_TWO_LOWEST_BYTES) & MASK_RRCR)==CODE_CB_RRCR && len == CB_CODE_LENGTH(CODE_CB_RRCR))
    {
        instruction=CODE_CB_RRCR;
        /* Extract the value of the register    */
        op1=EXTRACT(codeInHexa, 0, 3);
    }

    /* This is a RRC (HL)  */
    if (((codeInHexa & FIRST_TWO_LOWEST_BYTES) & MASK_RRCHL)==CODE_CB_RRCHL && len == CB_CODE_LENGTH(CODE_CB_RRCHL))
    {
        instruction=CODE_CB_RRCHL;
    }


    /* This is a EX AF,AF'  */
    if ((codeInHexa & MASK_EXAFAF)==CODE_EXAFAF && len==NATURAL_CODE_LENGTH(CODE_EXAFAF))
    {
        instruction=CODE_EXAFAF;
    }

    /* This is a EXX        */
    if ((codeInHexa & MASK_EXX)==CODE_EXX && len==NATURAL_CODE_LENGTH(CODE_EXX))
    {
        instruction=CODE_EXX;
    }

    /* This is a ADD A,r */
    if ((codeInHexa & MASK_ADDAR)==CODE_ADDAR && len == NATURAL_CODE_LENGTH(CODE_ADDAR))
    {
        instruction=CODE_ADDAR;
               
        op1=EXTRACT(codeInHexa, 0, 3);
    }

    /* This is a ADD A,(HL) */
    if ((codeInHexa & MASK_ADDAHL)==CODE_ADDAHL && len == NATURAL_CODE_LENGTH(CODE_ADDAHL))
    {
        instruction=CODE_ADDAHL;
    }

        /* This is a ADC A,r */
    if ((codeInHexa & MASK_ADCAR)==CODE_ADCAR && len == NATURAL_CODE_LENGTH(CODE_ADCAR))
    {
        instruction=CODE_ADCAR;
               
        op1=EXTRACT(codeInHexa, 0, 3);
    }

    /* This is a ADC A,(HL) */
    if ((codeInHexa & MASK_ADCAHL)==CODE_ADCAHL && len == NATURAL_CODE_LENGTH(CODE_ADCAHL))
    {
        instruction=CODE_ADCAHL;
    }

    /* This is a LD (nn),IX    */
    if (((codeInHexa>>SIZE_2_BYTES) & MASK_LDNNIX)==CODE_DD_LDNNIX && len == DD_CODE_LENGTH(CODE_DD_LDNNIX))
    {
        instruction=CODE_DD_LDNNIX; 
        op16=((codeInHexa & FIRST_LOWEST_BYTE)<<SIZE_1_BYTE)+((codeInHexa & SECOND_LOWEST_BYTE)>>SIZE_1_BYTE);
    }

    /* This is a LD (nn),IY    */
    if (((codeInHexa>>SIZE_2_BYTES) & MASK_LDNNIY)==CODE_FD_LDNNIY && len == DD_CODE_LENGTH(CODE_FD_LDNNIY))
    {
        instruction=CODE_FD_LDNNIY; 
        op16=((codeInHexa & FIRST_LOWEST_BYTE)<<SIZE_1_BYTE)+((codeInHexa & SECOND_LOWEST_BYTE)>>SIZE_1_BYTE);
    }

    /* This is a ADD HL,rr      */
    if ((codeInHexa & MASK_ADDHLRR)==CODE_ADDHLRR && len == NATURAL_CODE_LENGTH(CODE_ADDHLRR))
    {
        instruction=CODE_ADDHLRR;
               
        op1=EXTRACT(codeInHexa, 4, 2) | 0b1000;
    }

    /* This is a ADD IX,pp      */
    if ((codeInHexa & MASK_ADDIXPP)==CODE_DD_ADDIXPP && len == DD_CODE_LENGTH(CODE_DD_ADDIXPP))
    {
        instruction=CODE_DD_ADDIXPP;
               
        op2=EXTRACT(codeInHexa, 4, 2) | 0b1000;
    }

    /* This is a ADD IY,qq      */
    if ((codeInHexa & MASK_ADDIYQQ)==CODE_FD_ADDIYQQ && len == FD_CODE_LENGTH(CODE_FD_ADDIYQQ))
    {
        instruction=CODE_FD_ADDIYQQ;
               
        op2=EXTRACT(codeInHexa, 4, 2) | 0b1000;
    }

    /* This is a ADC HL,rr      */
    if ((codeInHexa & MASK_ADCHLRR)==CODE_ED_ADCHLRR && len == ED_CODE_LENGTH(CODE_ED_ADCHLRR))
    {
        instruction=CODE_ED_ADCHLRR;
               
        op1=EXTRACT(codeInHexa, 4, 2) | 0b1000;
    }

    /* This is a SBC HL,rr      */
    if ((codeInHexa & MASK_SBCHLRR)==CODE_ED_SBCHLRR && len == ED_CODE_LENGTH(CODE_ED_SBCHLRR))
    {
        instruction=CODE_ED_SBCHLRR;
               
        op1=EXTRACT(codeInHexa, 4, 2) | 0b1000;
    }

    /* This is a BIT b,r */
    if ((codeInHexa & MASK_BITBR)==CODE_CB_BITBR && len == CB_CODE_LENGTH(CODE_CB_BITBR))
    {
        instruction=CODE_CB_BITBR;
               
        op1=EXTRACT(codeInHexa, 3, 3);          /* This is b    */
        op2=EXTRACT(codeInHexa, 0, 3);          /* This is r    */
    }

    /* This is a BIT b,(HL) */
    if ((codeInHexa & MASK_BITBHL)==CODE_CB_BITBHL && len == CB_CODE_LENGTH(CODE_CB_BITBHL))
    {
        instruction=CODE_CB_BITBHL;
               
        op1=EXTRACT(codeInHexa, 3, 3);          /* This is b    */
    }
    
    /* This is a CALL nn */
    if ((codeInHexa>>SIZE_2_BYTES & MASK_CALLNN)==CODE_CALLNN && len == NATURAL_CODE_LENGTH(CODE_CALLNN))
    {
        instruction=CODE_CALLNN;
        retInterpret=NO_PC_CHANGE;                       /* Don't change the PC value after execution    */
             
        op16=EXTRACT(codeInHexa, 0, 16); 
    }

    /* This is a RET cc */
    if ((codeInHexa & MASK_RETCC)==CODE_RETCC && len == NATURAL_CODE_LENGTH(CODE_RETCC))
    {
        instruction=CODE_RETCC;
        retInterpret=NO_PC_CHANGE;                       /* Don't change the PC value after execution    */

        op1=EXTRACT(codeInHexa, 3, 3);
    }

    /* This is a RST p */
    if ((codeInHexa & MASK_RSTP)==CODE_RSTP && len == NATURAL_CODE_LENGTH(CODE_RSTP))
    {
        instruction=CODE_RSTP;
        retInterpret=NO_PC_CHANGE;                       /* Don't change the PC value after execution    */

        op1=EXTRACT(codeInHexa, 3, 3);
    }

    /* This is a RET cc,nn */
    if ((codeInHexa>>SIZE_2_BYTES & MASK_CALLCCNN)==CODE_CALLCCNN && len == NATURAL_CODE_LENGTH(CODE_CALLCCNN))
    {
        instruction=CODE_CALLCCNN;
        retInterpret=NO_PC_CHANGE;                       /* Don't change the PC value after execution    */

        op1=EXTRACT(codeInHexa, 19, 3);
        op16=EXTRACT(codeInHexa, 0, 16);
    }

    /* This is a JP cc,nn */
    if ((codeInHexa>>SIZE_2_BYTES & MASK_JPCCNN)==CODE_JPCCNN && len == NATURAL_CODE_LENGTH(CODE_JPCCNN))
    {
        instruction=CODE_JPCCNN;
        retInterpret=NO_PC_CHANGE;                       /* Don't change the PC value after execution    */

        op1=EXTRACT(codeInHexa, 19, 3);         /* This is cc   */
        op16=EXTRACT(codeInHexa, 0, 16);        /* This is nn   */
    }

    /* This is a RLC (IX+d) */
    if (((codeInHexa & MASK_RLCIXD)==CODE_DDCB_RLCIXD && len == DDCB_CODE_LENGTH(CODE_DDCB_RLCIXD)))
    {
        instruction=CODE_DDCB_RLCIXD;
        op1=EXTRACT(codeInHexa,8, 8);
    }

    /* This is a RLC (IY+d) */
    if (((codeInHexa & MASK_RLCIYD)==CODE_FDCB_RLCIYD && len == FDCB_CODE_LENGTH(CODE_FDCB_RLCIYD)))
    {
        instruction=CODE_FDCB_RLCIYD;
        op1=EXTRACT(codeInHexa,8, 8);
    }

    /* This is a RL (IX+d) */
    if (((codeInHexa & MASK_RLIXD)==CODE_DDCB_RLIXD && len == DDCB_CODE_LENGTH(CODE_DDCB_RLIXD)))
    {
        instruction=CODE_DDCB_RLIXD;
        op1=EXTRACT(codeInHexa,8, 8);
    }

    /* This is a RL (IY+d) */
    if (((codeInHexa & MASK_RLIYD)==CODE_FDCB_RLIYD && len == FDCB_CODE_LENGTH(CODE_FDCB_RLIYD)))
    {
        instruction=CODE_FDCB_RLIYD;
        op1=EXTRACT(codeInHexa,8, 8);
    }

    /* This is a RR (IX+d) */
    if (((codeInHexa & MASK_RRIXD)==CODE_DDCB_RRIXD && len == DDCB_CODE_LENGTH(CODE_DDCB_RRIXD)))
    {
        instruction=CODE_DDCB_RRIXD;
        op1=EXTRACT(codeInHexa,8, 8);
    }

    /* This is a RR (IY+d) */
    if (((codeInHexa & MASK_RRIYD)==CODE_FDCB_RRIYD && len == FDCB_CODE_LENGTH(CODE_FDCB_RRIYD)))
    {
        instruction=CODE_FDCB_RRIYD;
        op1=EXTRACT(codeInHexa,8, 8);
    }

    /* This is a RRC (IX+d) */
    if (((codeInHexa & MASK_RRCIXD)==CODE_DDCB_RRCIXD && len == DDCB_CODE_LENGTH(CODE_DDCB_RRCIXD)))
    {
        instruction=CODE_DDCB_RRCIXD;
        op1=EXTRACT(codeInHexa,8, 8);
    }

    /* This is a RRC (IY+d) */
    if (((codeInHexa & MASK_RRCIYD)==CODE_FDCB_RRCIYD && len == FDCB_CODE_LENGTH(CODE_FDCB_RRCIYD)))
    {
        instruction=CODE_FDCB_RRCIYD;
        op1=EXTRACT(codeInHexa,8, 8);
    }

    /* This is a DEC rr */
    if (((codeInHexa & MASK_DECRR)==CODE_DECRR && len == NATURAL_CODE_LENGTH(CODE_DECRR)))
    {
        instruction=CODE_DECRR;
        op1=EXTRACT(codeInHexa,4, 2) | 0b1000;
    }

    /* This is a DEC (HL) */
    if ((codeInHexa & MASK_DECHL)==CODE_DECHL && len == NATURAL_CODE_LENGTH(CODE_DECHL))
    {
        instruction=CODE_DECHL;
    }

    /* This is a INC (HL) */
    if ((codeInHexa & MASK_INCHL)==CODE_INCHL && len == NATURAL_CODE_LENGTH(CODE_INCHL))
    {
        instruction=CODE_INCHL;
    }

    /* This is a RES b,r */
    if ((codeInHexa & MASK_RESBR)==CODE_CB_RESBR && len == CB_CODE_LENGTH(CODE_CB_RESBR))
    {
        instruction=CODE_CB_RESBR;
               
        op1=EXTRACT(codeInHexa, 3, 3);          /* This is b    */
        op2=EXTRACT(codeInHexa, 0, 3);          /* This is r    */
    }

    /* This is a SET b,r */
    if ((codeInHexa & MASK_SETBR)==CODE_CB_SETBR && len == CB_CODE_LENGTH(CODE_CB_SETBR))
    {
        instruction=CODE_CB_SETBR;
               
        op1=EXTRACT(codeInHexa, 3, 3);          /* This is b    */
        op2=EXTRACT(codeInHexa, 0, 3);          /* This is r    */
    }

       /* This is a RES b,(HL) */
    if ((codeInHexa & MASK_RESBHL)==CODE_CB_RESBHL && len == CB_CODE_LENGTH(CODE_CB_RESBHL))
    {
        instruction=CODE_CB_RESBHL;
               
        op1=EXTRACT(codeInHexa, 3, 3);          /* This is b    */
    }

    /* This is a SET b,(HL) */
    if ((codeInHexa & MASK_SETBHL)==CODE_CB_SETBHL && len == CB_CODE_LENGTH(CODE_CB_SETBHL))
    {
        instruction=CODE_CB_SETBHL;
               
        op1=EXTRACT(codeInHexa, 3, 3);          /* This is b    */
    }

    /* This is a RLA    */
    if ((codeInHexa & MASK_RLA)==CODE_RLA && len == NATURAL_CODE_LENGTH(CODE_RLA))
    {
        instruction=CODE_RLA;
    }

    /* This is a RRA    */
    if ((codeInHexa & MASK_RRA)==CODE_RRA && len == NATURAL_CODE_LENGTH(CODE_RRA))
    {
        instruction=CODE_RRA;
    }

    /* This is a RL r */
    if ((codeInHexa & MASK_RLR)==CODE_CB_RLR && len == CB_CODE_LENGTH(CODE_CB_RLR))
    {
        instruction=CODE_CB_RLR;
               
        op1=EXTRACT(codeInHexa, 0, 3);          /* This is r    */
    }

    /* This is a RR r */
    if ((codeInHexa & MASK_RRR)==CODE_CB_RRR && len == CB_CODE_LENGTH(CODE_CB_RRR))
    {
        instruction=CODE_CB_RRR;
               
        op1=EXTRACT(codeInHexa, 0, 3);          /* This is r    */
    }

    /* This is a SLA r */
    if ((codeInHexa & MASK_SLAR)==CODE_CB_SLAR && len == CB_CODE_LENGTH(CODE_CB_SLAR))
    {
        instruction=CODE_CB_SLAR;
               
        op1=EXTRACT(codeInHexa, 0, 3);          /* This is r    */
    }

    /* This is a SRA r */
    if ((codeInHexa & MASK_SRAR)==CODE_CB_SRAR && len == CB_CODE_LENGTH(CODE_CB_SRAR))
    {
        instruction=CODE_CB_SRAR;
               
        op1=EXTRACT(codeInHexa, 0, 3);          /* This is SRA r    */
    }

        /* This is a SRL r */
    if ((codeInHexa & MASK_SRLR)==CODE_CB_SRLR && len == CB_CODE_LENGTH(CODE_CB_SRLR))
    {
        instruction=CODE_CB_SRLR;
               
        op1=EXTRACT(codeInHexa, 0, 3);          /* This is r    */
    }

    /* This is a RL (HL)  */
    if ((codeInHexa & MASK_RLHL)==CODE_CB_RLHL && len == CB_CODE_LENGTH(CODE_CB_RLHL))
    {
        instruction=CODE_CB_RLHL;

    }

    /* This is a RR (HL)  */
    if ((codeInHexa & MASK_RRHL)==CODE_CB_RRHL && len == CB_CODE_LENGTH(CODE_CB_RRHL))
    {
        instruction=CODE_CB_RRHL;
    }

    /* This is a SLA (HL)  */
    if ((codeInHexa & MASK_SLAHL)==CODE_CB_SLAHL && len == CB_CODE_LENGTH(CODE_CB_SLAHL))
    {
        instruction=CODE_CB_SLAHL;
    }

    /* This is a SRA (HL)  */
    if ((codeInHexa & MASK_SRAHL)==CODE_CB_SRAHL && len == CB_CODE_LENGTH(CODE_CB_SRAHL))
    {
        instruction=CODE_CB_SRAHL;
    }

    /* This is a SRL (HL)  */
    if ((codeInHexa & MASK_SRLHL)==CODE_CB_SRLHL && len == CB_CODE_LENGTH(CODE_CB_SRLHL))
    {
        instruction=CODE_CB_SRLHL;
    }

    /* This is a SLA (IX+d) */
    if (((codeInHexa & MASK_SLAIXD)==CODE_DDCB_SLAIXD && len == DDCB_CODE_LENGTH(CODE_DDCB_SLAIXD)))
    {
        instruction=CODE_DDCB_SLAIXD;
        op1=EXTRACT(codeInHexa,8, 8);
    }

    /* This is a SRA (IX+d) */
    if (((codeInHexa & MASK_SRAIXD)==CODE_DDCB_SRAIXD && len == DDCB_CODE_LENGTH(CODE_DDCB_SRAIXD)))
    {
        instruction=CODE_DDCB_SRAIXD;
        op1=EXTRACT(codeInHexa,8, 8);
    }

    /* This is a SRL (IX+d) */
    if (((codeInHexa & MASK_SRLIXD)==CODE_DDCB_SRLIXD && len == DDCB_CODE_LENGTH(CODE_DDCB_SRLIXD)))
    {
        instruction=CODE_DDCB_SRLIXD;
        op1=EXTRACT(codeInHexa,8, 8);
    }

    /* This is a SLA (IY+d) */
    if (((codeInHexa & MASK_SLAIYD)==CODE_FDCB_SLAIYD && len == FDCB_CODE_LENGTH(CODE_FDCB_SLAIYD)))
    {
        instruction=CODE_FDCB_SLAIYD;
        op1=EXTRACT(codeInHexa,8, 8);
    }

    /* This is a SRA (IY+d) */
    if (((codeInHexa & MASK_SRAIYD)==CODE_FDCB_SRAIYD && len == FDCB_CODE_LENGTH(CODE_FDCB_SRAIYD)))
    {
        instruction=CODE_FDCB_SRAIYD;
        op1=EXTRACT(codeInHexa,8, 8);
    }

    /* This is a SRL (IY+d) */
    if (((codeInHexa & MASK_SRLIYD)==CODE_FDCB_SRLIYD && len == FDCB_CODE_LENGTH(CODE_FDCB_SRLIYD)))
    {
        instruction=CODE_FDCB_SRLIYD;
        op1=EXTRACT(codeInHexa,8, 8);
    }

    /* This is a CPL  */
    if ((codeInHexa & MASK_CPL)==CODE_CPL && len == NATURAL_CODE_LENGTH(CODE_CPL))
    {
        instruction=CODE_CPL;

    }

    /* This is a NEG  */
    if ((codeInHexa & MASK_NEG)==CODE_ED_NEG && len == ED_CODE_LENGTH(CODE_ED_NEG))
    {
        instruction=CODE_ED_NEG;

    }

    /* This is a BIT (IX+d) */
    if (((codeInHexa & MASK_BITBIXD)==CODE_DDCB_BITBIXD && len == DDCB_CODE_LENGTH(CODE_DDCB_BITBIXD)))
    {
        instruction=CODE_DDCB_BITBIXD;
        op1=EXTRACT(codeInHexa,3, 3);           // This is b
        op2=EXTRACT(codeInHexa,8, 8);           // This is d
    }

    /* This is a BIT (IY+d) */
    if (((codeInHexa & MASK_BITBIYD)==CODE_FDCB_BITBIYD && len == FDCB_CODE_LENGTH(CODE_FDCB_BITBIYD)))
    {
        instruction=CODE_FDCB_BITBIYD;
        op1=EXTRACT(codeInHexa,3, 3);           // This is b
        op2=EXTRACT(codeInHexa,8, 8);           // This is d
    }

    /* This is a SET b,(IX+d) */
    if (((codeInHexa & MASK_SETBIXD)==CODE_DDCB_SETBIXD && len == DDCB_CODE_LENGTH(CODE_DDCB_SETBIXD)))
    {
        instruction=CODE_DDCB_SETBIXD;
        op1=EXTRACT(codeInHexa,3, 3);           // This is b
        op2=EXTRACT(codeInHexa,8, 8);           // This is d
    }

    /* This is a SET b,(IY+d) */
    if (((codeInHexa & MASK_SETBIYD)==CODE_FDCB_SETBIYD && len == FDCB_CODE_LENGTH(CODE_FDCB_SETBIYD)))
    {
        instruction=CODE_FDCB_SETBIYD;
        op1=EXTRACT(codeInHexa,3, 3);           // This is b
        op2=EXTRACT(codeInHexa,8, 8);           // This is d
    }

    /* This is a RES b,(IX+d) */
    if (((codeInHexa & MASK_RESBIXD)==CODE_DDCB_RESBIXD && len == DDCB_CODE_LENGTH(CODE_DDCB_RESBIXD)))
    {
        instruction=CODE_DDCB_RESBIXD;
        op1=EXTRACT(codeInHexa,3, 3);           // This is b
        op2=EXTRACT(codeInHexa,8, 8);           // This is d
    }

    /* This is a RES b,(IY+d) */
    if (((codeInHexa & MASK_RESBIYD)==CODE_FDCB_RESBIYD && len == FDCB_CODE_LENGTH(CODE_FDCB_SETBIYD)))
    {
        instruction=CODE_FDCB_RESBIYD;
        op1=EXTRACT(codeInHexa,3, 3);           // This is b
        op2=EXTRACT(codeInHexa,8, 8);           // This is d
    }
    
    /* This is a JP nn */
    if ((codeInHexa>>SIZE_2_BYTES & MASK_JPNN)==CODE_JPNN && len == NATURAL_CODE_LENGTH(CODE_JPNN))
    {
        instruction=CODE_JPNN;
        retInterpret=NO_PC_CHANGE;                       /* Don't change the PC value after execution    */

        op16=EXTRACT(codeInHexa, 0, 16);
    }

    /* This is a JR e */
    if ((codeInHexa>>SIZE_1_BYTE & MASK_JRE)==CODE_JRE && len == NATURAL_CODE_LENGTH(CODE_JRE))
    {
        instruction=CODE_JRE;
        retInterpret=NO_PC_CHANGE;                       /* Don't change the PC value after execution    */

        op1=EXTRACT(codeInHexa, 0, 8);
    }

    /* This is a LD (IX+d),n */
    if ((((codeInHexa >> SIZE_2_BYTES) & MASK_LDIXDN)==CODE_DD_LDIXDN && len == DD_CODE_LENGTH(CODE_DD_LDIXDN)))
    {
        instruction=CODE_DD_LDIXDN;

        op1=EXTRACT(codeInHexa,8, 8);           // This is d
        op2=EXTRACT(codeInHexa,0, 8);           // This is n
    }

    /* This is a LD (IY+d),n */
    if ((((codeInHexa >> SIZE_2_BYTES) & MASK_LDIYDN)==CODE_FD_LDIYDN && len == FD_CODE_LENGTH(CODE_FD_LDIYDN)))
    {
        instruction=CODE_FD_LDIYDN;

        op1=EXTRACT(codeInHexa,8, 8);           // This is d
        op2=EXTRACT(codeInHexa,0, 8);           // This is n
    }

    /* This is a LD SP,HL */
    if (((codeInHexa & MASK_LDSPHL)==CODE_LDSPHL && len == NATURAL_CODE_LENGTH(CODE_LDSPHL)))
    {
        instruction=CODE_LDSPHL;
    }

    /* This is a LD SP,IX */
    if (((codeInHexa & MASK_LDSPIX)==CODE_DD_LDSPIX && len == DD_CODE_LENGTH(CODE_DD_LDSPIX)))
    {
        instruction=CODE_DD_LDSPIX;
    }

    /* This is a LD SP,IY */
    if (((codeInHexa & MASK_LDSPIY)==CODE_FD_LDSPIY && len == FD_CODE_LENGTH(CODE_FD_LDSPIY)))
    {
        instruction=CODE_FD_LDSPIY;
    }

    /* This is a DJNZ e */
    if ((((codeInHexa >> SIZE_1_BYTE) & MASK_DJNZ)==CODE_DJNZ && len == NATURAL_CODE_LENGTH(CODE_DJNZ)))
    {
        instruction=CODE_DJNZ;
        retInterpret=NO_PC_CHANGE;                       /* Don't change the PC value after execution    */

        op1=EXTRACT(codeInHexa,0, 8);           // This is e
    }

    /* This is a RLD    */
    if (((codeInHexa & MASK_RLD)==CODE_ED_RLD && len == ED_CODE_LENGTH(CODE_ED_RLD)))
    {
        instruction=CODE_ED_RLD;
    }

    /* This is a RRD    */
    if (((codeInHexa & MASK_RRD)==CODE_ED_RRD && len == ED_CODE_LENGTH(CODE_ED_RRD)))
    {
        instruction=CODE_ED_RRD;
    }

    /* This is a LDI    */
    if (((codeInHexa & MASK_LDI)==CODE_ED_LDI && len == ED_CODE_LENGTH(CODE_ED_LDI)))
    {
        instruction=CODE_ED_LDI;
    }

    /* This is a LDIR    */
    if (((codeInHexa & MASK_LDIR)==CODE_ED_LDIR && len == ED_CODE_LENGTH(CODE_ED_LDIR)))
    {
        instruction=CODE_ED_LDIR;
    }

        /* This is a LDD    */
    if (((codeInHexa & MASK_LDD)==CODE_ED_LDD && len == ED_CODE_LENGTH(CODE_ED_LDD)))
    {
        instruction=CODE_ED_LDD;
    }

    /* This is a LDDR    */
    if (((codeInHexa & MASK_LDDR)==CODE_ED_LDDR && len == ED_CODE_LENGTH(CODE_ED_LDDR)))
    {
        instruction=CODE_ED_LDDR;
    }

    /* This is a CPI    */
    if (((codeInHexa & MASK_CPI)==CODE_ED_CPI && len == ED_CODE_LENGTH(CODE_ED_CPI)))
    {
        instruction=CODE_ED_CPI;
    }

    /* This is a CPIR    */
    if (((codeInHexa & MASK_CPIR)==CODE_ED_CPIR && len == ED_CODE_LENGTH(CODE_ED_CPIR)))
    {
        instruction=CODE_ED_CPIR;
    }

        /* This is a CPD    */
    if (((codeInHexa & MASK_CPD)==CODE_ED_CPD && len == ED_CODE_LENGTH(CODE_ED_CPD)))
    {
        instruction=CODE_ED_CPD;
    }

    /* This is a CPDR    */
    if (((codeInHexa & MASK_CPDR)==CODE_ED_CPDR && len == ED_CODE_LENGTH(CODE_ED_CPDR)))
    {
        instruction=CODE_ED_CPDR;
    }

    /* This is a EX (SP),HL    */
    if (((codeInHexa & MASK_EXSPHL)==CODE_EXSPHL && len == NATURAL_CODE_LENGTH(CODE_EXSPHL)))
    {
        instruction=CODE_EXSPHL;
    }

    /* This is a EX,(SP),IX    */
    if (((codeInHexa & MASK_EXSPIX)==CODE_DD_EXSPIX && len == DD_CODE_LENGTH(CODE_DD_EXSPIX)))
    {
        instruction=CODE_DD_EXSPIX;
    }

    /* This is a EX,(SP),IY    */
    if (((codeInHexa & MASK_EXSPIY)==CODE_FD_EXSPIY && len == FD_CODE_LENGTH(CODE_FD_EXSPIY)))
    {
        instruction=CODE_FD_EXSPIY;
    }

    /* This is a ADD A,n  */
    if (((codeInHexa>>SIZE_1_BYTE) & MASK_ADDAN)==CODE_ADDAN && len == NATURAL_CODE_LENGTH(CODE_ADDAN))
    {
        instruction=CODE_ADDAN;
        
        /* Extract the value of the register (in bits)    */
        op1=EXTRACT(codeInHexa, 0, 8);
    }

    /* This is a ADC A,n  */
    if (((codeInHexa>>SIZE_1_BYTE) & MASK_ADCAN)==CODE_ADCAN && len == NATURAL_CODE_LENGTH(CODE_ADCAN))
    {
        instruction=CODE_ADCAN;
        
        /* Extract the value of the register (in bits)    */
        op1=EXTRACT(codeInHexa, 0, 8);
    }

    /* This is a ADD A,(IX+d)  */
    if (((codeInHexa>>SIZE_1_BYTE) & MASK_ADDAIXD)==CODE_DD_ADDAIXD && len == DD_CODE_LENGTH(CODE_DD_ADDAIXD))
    {
        instruction=CODE_DD_ADDAIXD;
        
        /* Extract the value of the register (in bits)    */
        op1=EXTRACT(codeInHexa, 0, 8);
    }

    /* This is a ADD A,(IY+d)  */
    if (((codeInHexa>>SIZE_1_BYTE) & MASK_ADDAIYD)==CODE_FD_ADDAIYD && len == FD_CODE_LENGTH(CODE_FD_ADDAIYD))
    {
        instruction=CODE_FD_ADDAIYD;
        
        /* Extract the value of the register (in bits)    */
        op1=EXTRACT(codeInHexa, 0, 8);
    }

    /* This is a INC (IX+d)  */
    if (((codeInHexa>>SIZE_1_BYTE) & MASK_INCIXD)==CODE_DD_INCIXD && len == DD_CODE_LENGTH(CODE_DD_INCIXD))
    {
        instruction=CODE_DD_INCIXD;
        
        /* Extract the value of the register (in bits)    */
        op1=EXTRACT(codeInHexa, 0, 8);
    }

    /* This is a INC (IY+d)  */
    if (((codeInHexa>>SIZE_1_BYTE) & MASK_INCIYD)==CODE_FD_INCIYD && len == FD_CODE_LENGTH(CODE_FD_INCIYD))
    {
        instruction=CODE_FD_INCIYD;
        
        /* Extract the value of the register (in bits)    */
        op1=EXTRACT(codeInHexa, 0, 8);
    }

    /* This is a DEC (IX+d)  */
    if (((codeInHexa>>SIZE_1_BYTE) & MASK_DECIXD)==CODE_DD_DECIXD && len == DD_CODE_LENGTH(CODE_DD_DECIXD))
    {
        instruction=CODE_DD_DECIXD;
        
        /* Extract the value of the register (in bits)    */
        op1=EXTRACT(codeInHexa, 0, 8);
    }

    /* This is a DEC (IY+d)  */
    if (((codeInHexa>>SIZE_1_BYTE) & MASK_DECIYD)==CODE_FD_DECIYD && len == FD_CODE_LENGTH(CODE_FD_DECIYD))
    {
        instruction=CODE_FD_DECIYD;
        
        /* Extract the value of the register (in bits)    */
        op1=EXTRACT(codeInHexa, 0, 8);
    }

    /* This is a AND r */
    if ((codeInHexa & MASK_ANDR)==CODE_ANDR && len == NATURAL_CODE_LENGTH(CODE_ANDR))
    {
        instruction=CODE_ANDR;
               
        op1=EXTRACT(codeInHexa, 0, 3);
    }

    /* This is a AND n */
    if (((codeInHexa >> SIZE_1_BYTE) & MASK_ANDN)==CODE_ANDN && len == NATURAL_CODE_LENGTH(CODE_ANDN))
    {
        instruction=CODE_ANDN;
               
        op1=codeInHexa & FIRST_LOWEST_BYTE;
    }

    /* This is a AND (HL) */
    if ((codeInHexa & MASK_ANDHL)==CODE_ANDHL && len == NATURAL_CODE_LENGTH(CODE_ANDHL))
    {
        instruction=CODE_ANDHL;
    }

    /* This is a AND (IX+d) */
    if (((codeInHexa >> SIZE_1_BYTE) & MASK_ANDIXD)==CODE_DD_ANDIXD && len == DD_CODE_LENGTH(CODE_DD_ANDIXD))
    {
        instruction=CODE_DD_ANDIXD;
               
        op1=codeInHexa & FIRST_LOWEST_BYTE;
    }

    /* This is a AND (IY+d) */
    if (((codeInHexa >> SIZE_1_BYTE) & MASK_ANDIYD)==CODE_FD_ANDIYD && len == FD_CODE_LENGTH(CODE_FD_ANDIYD))
    {
        instruction=CODE_FD_ANDIYD;
               
        op1=codeInHexa & FIRST_LOWEST_BYTE;
    }

    /* This is a OR r */
    if ((codeInHexa & MASK_ORR)==CODE_ORR && len == NATURAL_CODE_LENGTH(CODE_ORR))
    {
        instruction=CODE_ORR;
               
        op1=EXTRACT(codeInHexa, 0, 3);
    }

    /* This is a OR n */
    if (((codeInHexa >> SIZE_1_BYTE) & MASK_ORN)==CODE_ORN && len == NATURAL_CODE_LENGTH(CODE_ORN))
    {
        instruction=CODE_ORN;
               
        op1=codeInHexa & FIRST_LOWEST_BYTE;
    }

    /* This is a OR (HL) */
    if ((codeInHexa & MASK_ORHL)==CODE_ORHL && len == NATURAL_CODE_LENGTH(CODE_ORHL))
    {
        instruction=CODE_ORHL;
    }

    /* This is a OR (IX+d) */
    if (((codeInHexa >> SIZE_1_BYTE) & MASK_ORIXD)==CODE_DD_ORIXD && len == DD_CODE_LENGTH(CODE_DD_ORIXD))
    {
        instruction=CODE_DD_ORIXD;
               
        op1=codeInHexa & FIRST_LOWEST_BYTE;
    }

    /* This is a OR (IY+d) */
    if (((codeInHexa >> SIZE_1_BYTE) & MASK_ORIYD)==CODE_FD_ORIYD && len == FD_CODE_LENGTH(CODE_FD_ORIYD))
    {
        instruction=CODE_FD_ORIYD;
               
        op1=codeInHexa & FIRST_LOWEST_BYTE;
    }

    /* This is a XOR r */
    if ((codeInHexa & MASK_XORR)==CODE_XORR && len == NATURAL_CODE_LENGTH(CODE_XORR))
    {
        instruction=CODE_XORR;
               
        op1=EXTRACT(codeInHexa, 0, 3);
    }

    /* This is a XOR n */
    if (((codeInHexa >> SIZE_1_BYTE) & MASK_XORN)==CODE_XORN && len == NATURAL_CODE_LENGTH(CODE_XORN))
    {
        instruction=CODE_XORN;
               
        op1=codeInHexa & FIRST_LOWEST_BYTE;
    }

    /* This is a XOR (HL) */
    if ((codeInHexa & MASK_XORHL)==CODE_XORHL && len == NATURAL_CODE_LENGTH(CODE_XORHL))
    {
        instruction=CODE_XORHL;
    }

    /* This is a XOR (IX+d) */
    if (((codeInHexa >> SIZE_1_BYTE) & MASK_XORIXD)==CODE_DD_XORIXD && len == DD_CODE_LENGTH(CODE_DD_XORIXD))
    {
        instruction=CODE_DD_XORIXD;
               
        op1=codeInHexa & FIRST_LOWEST_BYTE;
    }

    /* This is a XOR (IY+d) */
    if (((codeInHexa >> SIZE_1_BYTE) & MASK_XORIYD)==CODE_FD_XORIYD && len == FD_CODE_LENGTH(CODE_FD_XORIYD))
    {
        instruction=CODE_FD_XORIYD;
               
        op1=codeInHexa & FIRST_LOWEST_BYTE;
    }

    /* This is a CP r */
    if ((codeInHexa & MASK_CPR)==CODE_CPR && len == NATURAL_CODE_LENGTH(CODE_CPR))
    {
        instruction=CODE_CPR;
               
        op1=EXTRACT(codeInHexa, 0, 3);
    }

    /* This is a CP n */
    if (((codeInHexa >> SIZE_1_BYTE) & MASK_CPN)==CODE_CPN && len == NATURAL_CODE_LENGTH(CODE_CPN))
    {
        instruction=CODE_CPN;
               
        op1=codeInHexa & FIRST_LOWEST_BYTE;
    }

    /* This is a CP (HL) */
    if ((codeInHexa & MASK_CPHL)==CODE_CPHL && len == NATURAL_CODE_LENGTH(CODE_CPHL))
    {
        instruction=CODE_CPHL;
    }

    /* This is a CP (IX+d) */
    if (((codeInHexa >> SIZE_1_BYTE) & MASK_CPIXD)==CODE_DD_CPIXD && len == DD_CODE_LENGTH(CODE_DD_CPIXD))
    {
        instruction=CODE_DD_CPIXD;
               
        op1=codeInHexa & FIRST_LOWEST_BYTE;
    }

    /* This is a CP (IY+d) */
    if (((codeInHexa >> SIZE_1_BYTE) & MASK_CPIYD)==CODE_FD_CPIYD && len == FD_CODE_LENGTH(CODE_FD_CPIYD))
    {
        instruction=CODE_FD_CPIYD;
               
        op1=codeInHexa & FIRST_LOWEST_BYTE;
    }

    /* This is a INC IX */
    if ((codeInHexa & MASK_INCIX)==CODE_DD_INCIX && len == DD_CODE_LENGTH(CODE_DD_INCIX))
    {
        instruction=CODE_DD_INCIX;
    }

    /* This is a INC IY */
    if ((codeInHexa & MASK_INCIY)==CODE_FD_INCIY && len == FD_CODE_LENGTH(CODE_FD_INCIY))
    {
        instruction=CODE_FD_INCIY;
    }

    /* This is a DEC IX */
    if ((codeInHexa & MASK_DECIX)==CODE_DD_DECIX && len == DD_CODE_LENGTH(CODE_DD_DECIX))
    {
        instruction=CODE_DD_DECIX;
    }

    /* This is a DEC IY */
    if ((codeInHexa & MASK_DECIY)==CODE_FD_DECIY && len == FD_CODE_LENGTH(CODE_FD_DECIY))
    {
        instruction=CODE_FD_DECIY;
    }

    /* This is a JR C,e */
    if ((codeInHexa>>SIZE_1_BYTE & MASK_JRCE)==CODE_JRCE && len == NATURAL_CODE_LENGTH(CODE_JRCE))
    {
        instruction=CODE_JRCE;
        retInterpret=NO_PC_CHANGE;                       /* Don't change the PC value after execution    */

        op1=EXTRACT(codeInHexa, 0, 8);
    }

    /* This is a JR NC,e */
    if ((codeInHexa>>SIZE_1_BYTE & MASK_JRNCE)==CODE_JRNCE && len == NATURAL_CODE_LENGTH(CODE_JRNCE))
    {
        instruction=CODE_JRNCE;
        retInterpret=NO_PC_CHANGE;                       /* Don't change the PC value after execution    */

        op1=EXTRACT(codeInHexa, 0, 8);
    }

    /* This is a JR Z,e */
    if ((codeInHexa>>SIZE_1_BYTE & MASK_JRZE)==CODE_JRZE && len == NATURAL_CODE_LENGTH(CODE_JRZE))
    {
        instruction=CODE_JRZE;
        retInterpret=NO_PC_CHANGE;                       /* Don't change the PC value after execution    */

        op1=EXTRACT(codeInHexa, 0, 8);
    }

    /* This is a JR NZ,e */
    if ((codeInHexa>>SIZE_1_BYTE & MASK_JRNZE)==CODE_JRNZE && len == NATURAL_CODE_LENGTH(CODE_JRNZE))
    {
        instruction=CODE_JRNZE;
        retInterpret=NO_PC_CHANGE;                       /* Don't change the PC value after execution    */

        op1=EXTRACT(codeInHexa, 0, 8);
    }

    /* This is a SUB A,r */
    if ((codeInHexa & MASK_SUBAR)==CODE_SUBAR && len == NATURAL_CODE_LENGTH(CODE_SUBAR))
    {
        instruction=CODE_SUBAR;
               
        op1=EXTRACT(codeInHexa, 0, 3);
    }

    /* This is a SUB A,(HL) */
    if ((codeInHexa & MASK_SUBAHL)==CODE_SUBAHL && len == NATURAL_CODE_LENGTH(CODE_SUBAHL))
    {
        instruction=CODE_SUBAHL;
    }

    /* This is a SUB A,n  */
    if (((codeInHexa>>SIZE_1_BYTE) & MASK_SUBAN)==CODE_SUBAN && len == NATURAL_CODE_LENGTH(CODE_SUBAN))
    {
        instruction=CODE_SUBAN;
        
        /* Extract the value of the register (in bits)    */
        op1=EXTRACT(codeInHexa, 0, 8);
    }

    /* This is a SUB A,(IX+d)  */
    if (((codeInHexa>>SIZE_1_BYTE) & MASK_SUBAIXD)==CODE_DD_SUBAIXD && len == DD_CODE_LENGTH(CODE_DD_SUBAIXD))
    {
        instruction=CODE_DD_SUBAIXD;
        
        /* Extract the value of the register (in bits)    */
        op1=EXTRACT(codeInHexa, 0, 8);
    }

    /* This is a SUB A,(IY+d)  */
    if (((codeInHexa>>SIZE_1_BYTE) & MASK_SUBAIYD)==CODE_FD_SUBAIYD && len == FD_CODE_LENGTH(CODE_FD_SUBAIYD))
    {
        instruction=CODE_FD_SUBAIYD;
        
        /* Extract the value of the register (in bits)    */
        op1=EXTRACT(codeInHexa, 0, 8);
    }

    /* This is a SBC A,r */
    if ((codeInHexa & MASK_SBCAR)==CODE_SBCAR && len == NATURAL_CODE_LENGTH(CODE_SBCAR))
    {
        instruction=CODE_SBCAR;
               
        op1=EXTRACT(codeInHexa, 0, 3);
    }

    /* This is a SBC A,(HL) */
    if ((codeInHexa & MASK_SBCAHL)==CODE_SBCAHL && len == NATURAL_CODE_LENGTH(CODE_SBCAHL))
    {
        instruction=CODE_SBCAHL;
    }

    /* This is a SBC A,n  */
    if (((codeInHexa>>SIZE_1_BYTE) & MASK_SBCAN)==CODE_SBCAN && len == NATURAL_CODE_LENGTH(CODE_SBCAN))
    {
        instruction=CODE_SBCAN;
        
        /* Extract the value of the register (in bits)    */
        op1=EXTRACT(codeInHexa, 0, 8);
    }

    /* This is a SBC A,(IX+d)  */
    if (((codeInHexa>>SIZE_1_BYTE) & MASK_SBCAIXD)==CODE_DD_SBCAIXD && len == DD_CODE_LENGTH(CODE_DD_SBCAIXD))
    {
        instruction=CODE_DD_SBCAIXD;
        
        /* Extract the value of the register (in bits)    */
        op1=EXTRACT(codeInHexa, 0, 8);
    }

    /* This is a SBC A,(IY+d)  */
    if (((codeInHexa>>SIZE_1_BYTE) & MASK_SBCAIYD)==CODE_FD_SBCAIYD && len == FD_CODE_LENGTH(CODE_FD_SBCAIYD))
    {
        instruction=CODE_FD_SBCAIYD;
        
        /* Extract the value of the register (in bits)    */
        op1=EXTRACT(codeInHexa, 0, 8);
    }

    /* This is a JP (HL) */
    if ((codeInHexa & MASK_JPHL)==CODE_JPHL && len == NATURAL_CODE_LENGTH(CODE_JPHL))
    {
        instruction=CODE_JPHL;
        retInterpret=NO_PC_CHANGE;                       /* Don't change the PC value after execution    */
    }

    /* This is a JP (IX) */
    if ((codeInHexa & MASK_JPIX)==CODE_DD_JPIX && len == DD_CODE_LENGTH(CODE_DD_JPIX))
    {
        instruction=CODE_DD_JPIX;
        retInterpret=NO_PC_CHANGE;                       /* Don't change the PC value after execution    */
    }

    /* This is a JP (IY) */
    if ((codeInHexa & MASK_JPIX)==CODE_FD_JPIY && len == FD_CODE_LENGTH(CODE_FD_JPIY))
    {
        instruction=CODE_FD_JPIY;
        retInterpret=NO_PC_CHANGE;                       /* Don't change the PC value after execution    */
    }

    /* This is a CCF */
    if ((codeInHexa & MASK_CCF)==CODE_CCF && len == NATURAL_CODE_LENGTH(CODE_CCF))
    {
        instruction=CODE_CCF;
    }

    /* This is a SCF */
    if ((codeInHexa & MASK_SCF)==CODE_SCF && len == NATURAL_CODE_LENGTH(CODE_SCF))
    {
        instruction=CODE_SCF;
    }

    /* This is a RET */
    if ((codeInHexa & MASK_RET)==CODE_RET && len == NATURAL_CODE_LENGTH(CODE_RET))
    {
        instruction=CODE_RET;
        retInterpret=NO_PC_CHANGE;                       /* Don't change the PC value after execution    */
    }

    /* This is a DAA */
    if ((codeInHexa & MASK_DAA)==CODE_DAA && len == NATURAL_CODE_LENGTH(CODE_DAA))
    {
        instruction=CODE_DAA;
    }

    /* This is a LD A,I */
    if ((codeInHexa & MASK_LDAI)==CODE_ED_LDAI && len == ED_CODE_LENGTH(CODE_ED_LDAI))
    {
        instruction=CODE_ED_LDAI;
    }

    /* This is a LD A,R */
    if ((codeInHexa & MASK_LDAR)==CODE_ED_LDAR && len == ED_CODE_LENGTH(CODE_ED_LDAR))
    {
        instruction=CODE_ED_LDAR;
    }

        /* This is a LD I,A */
    if ((codeInHexa & MASK_LDIA)==CODE_ED_LDIA && len == ED_CODE_LENGTH(CODE_ED_LDIR))
    {
        instruction=CODE_ED_LDIA;
    }

    /* This is a LD R,A */
    if ((codeInHexa & MASK_LDRA)==CODE_ED_LDRA && len == ED_CODE_LENGTH(CODE_ED_LDRA))
    {
        instruction=CODE_ED_LDRA;
    }

    /* This is a DI */
    if ((codeInHexa & MASK_DI)==CODE_DI && len == NATURAL_CODE_LENGTH(CODE_DI))
    {
        instruction=CODE_DI;
    }

    /* This is a EI */
    if ((codeInHexa & MASK_EI)==CODE_EI && len == NATURAL_CODE_LENGTH(CODE_EI))
    {
        instruction=CODE_EI;
    }

    /* This is a EX DE,HL */
    if ((codeInHexa & MASK_EXDEHL)==CODE_EXDEHL && len == NATURAL_CODE_LENGTH(CODE_EXDEHL))
    {
        instruction=CODE_EXDEHL;
    }

    // bottom 1
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

        case CODE_LDRRNN:                                    /* This is a LD rr,nn          */   
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
            
            if (pMode==INTP_EXECUTE || pMode==INTP_EXECUTE_BLIND)                      /* Execute LD r,(IY+d)*/
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
                address=mRegisterPack.regIY.getValue()-op1;
                sprintf(mInstruction, "LD (IY-#%02X),%s", op1, sop2);
            }
            else
            {
                address=mRegisterPack.regIY.getValue()+op1;
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

        case CODE_DD_LDIXDN:                                        /* This is a LD (IX+d),n  */
            
            if (SIGN(op1))                                          /* Check if op2 is negative */
            {
                op1=~op1+1;
                address=mRegisterPack.regIX.getValue()-op1;
                sprintf(mInstruction, "LD (IX-#%02X),#%02X", op1, op2);
            }
            else
            {
                address=mRegisterPack.regIX.getValue()+op1;
                sprintf(mInstruction, "LD (IX+#%02X),#%02X", op1, op2);
            }

            if (pMode==INTP_EXECUTE || pMode==INTP_EXECUTE_BLIND)                           /* Execute LD (IX+d),n  */
            {
                mMemory->set8bitsValue(address, op2);
            
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

        case CODE_FD_LDIYDN:                                        /* This is a LD (IY+d),n  */
            
            if (SIGN(op1))                                          /* Check if op2 is negative */
            {
                op1=~op1+1;
                address=mRegisterPack.regIY.getValue()-op1;
                sprintf(mInstruction, "LD (IY-#%02X),#%02X", op1, op2);
            }
            else
            {
                address=mRegisterPack.regIY.getValue()+op1;
                sprintf(mInstruction, "LD (IY+#%02X),#%02X", op1, op2);
            }

            if (pMode==INTP_EXECUTE || pMode==INTP_EXECUTE_BLIND)                           /* Execute LD (IY+d),n  */
            {
                mMemory->set8bitsValue(address, op2);
            
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

        case CODE_LDSPHL:                                    /* This is a LD SP,HL          */   
            sprintf(mInstruction, "LD SP,HL");
            
            if (pMode==INTP_EXECUTE || pMode==INTP_EXECUTE_BLIND)
            {
                reg16_1=get16bitsRegisterAddress(REGSP);
                reg16_2=get16bitsRegisterAddress(REGHL);
                reg16_1->setValue(reg16_2->getValue());

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

        case CODE_DD_LDSPIX:                                    /* This is a LD SP,IX          */   
            sprintf(mInstruction, "LD SP,IX");
            
            if (pMode==INTP_EXECUTE || pMode==INTP_EXECUTE_BLIND)
            {
                reg16_1=get16bitsRegisterAddress(REGSP);
                reg16_2=get16bitsRegisterAddress(REGIX);
                reg16_1->setValue(reg16_2->getValue());

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

        case CODE_FD_LDSPIY:                                    /* This is a LD SP,IY          */   
            sprintf(mInstruction, "LD SP,IY");
            
            if (pMode==INTP_EXECUTE || pMode==INTP_EXECUTE_BLIND)
            {
                reg16_1=get16bitsRegisterAddress(REGSP);
                reg16_2=get16bitsRegisterAddress(REGIY);
                reg16_1->setValue(reg16_2->getValue());

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

        case CODE_DD_LDNNIX:                                       /* This is a LD (nn),IX    */   
            sprintf(mInstruction, "LD (#%04X),IX", op16);

            if (pMode==INTP_EXECUTE || pMode==INTP_EXECUTE_BLIND)                            
            {
                reg16_1=get16bitsRegisterAddress(REGIX);
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

        case CODE_FD_LDNNIY:                                       /* This is a LD (nn),IY    */   
            sprintf(mInstruction, "LD (#%04X),IY", op16);

            if (pMode==INTP_EXECUTE || pMode==INTP_EXECUTE_BLIND)                            
            {
                reg16_1=get16bitsRegisterAddress(REGIY);
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
                S_IS(SIGN(reg8_1->getValue()));
                Z_IS(ZERO(reg8_1->getValue()));
                N_RESET;

                if (reg8_1->getValue()==0x10)
                {
                    H_SET;
                }
                else
                {
                    H_RESET;
                }

                if (reg8_1->getValue()==0x80)
                {
                    PV_SET;
                }
                else
                {
                    PV_RESET;
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
                S_IS(SIGN(reg8_1->getValue()));
                Z_IS(ZERO(reg8_1->getValue()));
                N_SET;

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

        case CODE_DD_INCIX:                                            /* This is a INC IX    */
            sprintf(mInstruction, "INC IX");

            if (pMode==INTP_EXECUTE || pMode==INTP_EXECUTE_BLIND)                                
            {
                reg16_1=get16bitsRegisterAddress(REGIX);
                reg16_1->setValue(reg16_1->getValue()+1);

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
        
        case CODE_FD_INCIY:                                            /* This is a INC IY    */
            sprintf(mInstruction, "INC IY");

            if (pMode==INTP_EXECUTE || pMode==INTP_EXECUTE_BLIND)                                
            {
                reg16_1=get16bitsRegisterAddress(REGIY);
                reg16_1->setValue(reg16_1->getValue()+1);

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

        case CODE_DD_DECIX:                                            /* This is a DEC IX    */
            sprintf(mInstruction, "DEC IX");

            if (pMode==INTP_EXECUTE || pMode==INTP_EXECUTE_BLIND)                                
            {
                reg16_1=get16bitsRegisterAddress(REGIX);
                reg16_1->setValue(reg16_1->getValue()-1);

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
        
        case CODE_FD_DECIY:                                            /* This is a DEC IY    */
            sprintf(mInstruction, "DEC IY");

            if (pMode==INTP_EXECUTE || pMode==INTP_EXECUTE_BLIND)                                
            {
                reg16_1=get16bitsRegisterAddress(REGIY);
                reg16_1->setValue(reg16_1->getValue()-1);

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

        case CODE_POPQQ:                                       /* This is a POP qq                    */
            ret=bitToRegister(op1, sop1);

            sprintf(mInstruction, "POP %s", sop1);

            if (pMode==INTP_EXECUTE || pMode==INTP_EXECUTE_BLIND)                            
            {
                reg16_1=get16bitsRegisterAddress(op1);
                reg16_1->setValue(mMemory->getAddress(mRegisterPack.regSP.getValue()));

                mRegisterPack.regSP.setValue(mRegisterPack.regSP.getValue()+2);

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

        case CODE_DD_POPIX:                                       /* This is a POP IX                    */
            sprintf(mInstruction, "POP IX");

            if (pMode==INTP_EXECUTE || pMode==INTP_EXECUTE_BLIND)                            
            {
                reg16_1=get16bitsRegisterAddress(REGIX);
                reg16_1->setValue(mMemory->getAddress(mRegisterPack.regSP.getValue()));

                mRegisterPack.regSP.setValue(mRegisterPack.regSP.getValue()+2);

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

        case CODE_FD_POPIY:                                       /* This is a POP IY                    */
            sprintf(mInstruction, "POP IY");

            if (pMode==INTP_EXECUTE || pMode==INTP_EXECUTE_BLIND)                            
            {
                reg16_1=get16bitsRegisterAddress(REGIY);
                reg16_1->setValue(mMemory->getAddress(mRegisterPack.regSP.getValue()));

                mRegisterPack.regSP.setValue(mRegisterPack.regSP.getValue()+2);

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

        case CODE_DD_PUSHIX:                                       /* This is a PUSH IX                    */
            sprintf(mInstruction, "PUSH IX");

            if (pMode==INTP_EXECUTE || pMode==INTP_EXECUTE_BLIND)                            
            {
                mRegisterPack.regSP.setValue(mRegisterPack.regSP.getValue()-2);
                reg16_1=get16bitsRegisterAddress(REGIX);
                mMemory->setAddress(mRegisterPack.regSP.getValue(), reg16_1->getValue());

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

        case CODE_FD_PUSHIY:                                       /* This is a PUSH IY                    */
            sprintf(mInstruction, "PUSH IY");

            if (pMode==INTP_EXECUTE || pMode==INTP_EXECUTE_BLIND)                            
            {
                mRegisterPack.regSP.setValue(mRegisterPack.regSP.getValue()-2);
                reg16_1=get16bitsRegisterAddress(REGIY);
                mMemory->setAddress(mRegisterPack.regSP.getValue(), reg16_1->getValue());

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
                /* Retrieve the byte and rotate it  */
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

        case CODE_CB_RLHL:                                         /* This is a RL (HL)  */
            sprintf(mInstruction, "RL (HL)");

            if (pMode==INTP_EXECUTE || pMode==INTP_EXECUTE_BLIND)                            
            {
                /* Retrieve the byte and rotate it  */
                val=mMemory->get8bitsValue(mRegisterPack.regHL.getValue());
                bit=EXTRACT(val, 7, 1);
                
                newVal=((val<<1) & FIRST_LOWEST_BYTE) | mRegisterPack.regF.getBit(BITPOS_FLAG_CARRY);

                mMemory->set8bitsValue(mRegisterPack.regHL.getValue(), newVal);            
                mRegisterPack.regF.setCarryFlag((bool) bit);

                /* Modify flags here    */
                S_IS(SIGN(newVal));
                Z_IS(ZERO(newVal));
                H_RESET;
                PV_IS(EVEN(newVal));
                N_RESET;

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

        case CODE_DDCB_RLIXD:                                           /* This is a RL (IX+d)  */        
            if (SIGN(op1))                                              /* Check if op1 is negative */
            {
                op1=~op1+1;
                address=mRegisterPack.regIX.getValue()-op1;
                sprintf(mInstruction, "RL (IX-#%02X)", op1);
            }
            else
            {
                address=mRegisterPack.regIX.getValue()+op1;
                sprintf(mInstruction, "RL (IX+#%02X)", op1);
            }

            if (pMode==INTP_EXECUTE || pMode==INTP_EXECUTE_BLIND)                       /* Execute RL (IX+d)    */
            {
                /* Retrieve the byte and rotate it  */
                val=mMemory->get8bitsValue(address);
                bit=EXTRACT(val, 7, 1);
                
                newVal=((val<<1) & FIRST_LOWEST_BYTE) | mRegisterPack.regF.getBit(BITPOS_FLAG_CARRY);
                mMemory->set8bitsValue(address, newVal);
       
                mRegisterPack.regF.setCarryFlag((bool) bit);

                /* Modify flags here    */
                S_IS(SIGN(newVal));
                Z_IS(ZERO(newVal));
                H_RESET;
                PV_IS(EVEN(newVal));
                N_RESET;

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

        case CODE_FDCB_RLIYD:                                           /* This is a RL (IY+d)  */        
            if (SIGN(op1))                                              /* Check if op1 is negative */
            {
                op1=~op1+1;
                address=mRegisterPack.regIY.getValue()-op1;
                sprintf(mInstruction, "RL (IY-#%02X)", op1);
            }
            else
            {
                address=mRegisterPack.regIY.getValue()+op1;
                sprintf(mInstruction, "RL (IY+#%02X)", op1);
            }

            if (pMode==INTP_EXECUTE || pMode==INTP_EXECUTE_BLIND)                       /* Execute RL (IY+d)    */
            {
                /* Retrieve the byte and rotate it  */
                val=mMemory->get8bitsValue(address);
                bit=EXTRACT(val, 7, 1);
                
                newVal=((val<<1) & FIRST_LOWEST_BYTE) | mRegisterPack.regF.getBit(BITPOS_FLAG_CARRY);
                mMemory->set8bitsValue(address, newVal);
       
                mRegisterPack.regF.setCarryFlag((bool) bit);

                /* Modify flags here    */
                S_IS(SIGN(newVal));
                Z_IS(ZERO(newVal));
                H_RESET;
                PV_IS(EVEN(newVal));
                N_RESET;

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

        case CODE_CB_RRCHL:                                         /* This is a RRC (HL)  */
            sprintf(mInstruction, "RRC (HL)");

            if (pMode==INTP_EXECUTE || pMode==INTP_EXECUTE_BLIND)                            
            {
                /* Retrieve the byte and rotate it  */
                val=mMemory->get8bitsValue(mRegisterPack.regHL.getValue());    
                newVal=(val>>1) | (BIT(val, 0)<<7);
                mMemory->set8bitsValue(mRegisterPack.regHL.getValue(), newVal);

                /* Modify flags here    */
                S_IS(SIGN(newVal));
                Z_IS(ZERO(newVal));
                H_RESET;
                PV_IS(EVEN(newVal));
                N_RESET;
                C_IS(BIT(newVal, 7));

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

        case CODE_RRCA:                                         /* This is a RRCA  */
            sprintf(mInstruction, "RRCA");

            if (pMode==INTP_EXECUTE || pMode==INTP_EXECUTE_BLIND)                            
            {
                reg8_1=get8bitsRegisterAddress(REGA);
                newVal=(reg8_1->getValue()>>1) | (BIT(reg8_1->getValue(), 0)<<7);
                reg8_1->setValue(newVal);

                /* Modify flags here    */
                H_RESET;
                N_RESET;
                C_IS(BIT(reg8_1->getValue(), 7));

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

        case CODE_CB_RRCR:                                         /* This is a RRC r  */
            ret=bitToRegister(op1, sop1);

            sprintf(mInstruction, "RRC %s", sop1);

            if (pMode==INTP_EXECUTE || pMode==INTP_EXECUTE_BLIND)                            
            {
                reg8_1=get8bitsRegisterAddress(op1);
                newVal=(reg8_1->getValue()>>1) | (BIT(reg8_1->getValue(), 0)<<7);
                reg8_1->setValue(newVal);

                /* Modify flags here    */
                S_IS(SIGN(newVal));
                Z_IS(ZERO(newVal));
                H_RESET;
                PV_IS(EVEN(newVal));
                N_RESET;
                C_IS(BIT(reg8_1->getValue(), 7));

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

        case CODE_EXAFAF:                                         /* This is a EX AF,AF'  */
            sprintf(mInstruction, "EX AF,AF'");

            if (pMode==INTP_EXECUTE || pMode==INTP_EXECUTE_BLIND)                            
            {
                /* Switch AF and AF'    */
                reg16_1=get16bitsRegisterAddress(REGAF);
                reg16_2=get16bitsRegisterAddress(REGAFP);
                
                op16=reg16_1->getValue();
                reg16_1->setValue(reg16_2->getValue());
                reg16_2->setValue(op16);

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

        case CODE_EXX:                                          /* This is a EXX         */
            sprintf(mInstruction, "EXX");

            if (pMode==INTP_EXECUTE || pMode==INTP_EXECUTE_BLIND)                            
            {
                /* Switch BC and BC'    */
                reg16_1=get16bitsRegisterAddress(REGBC);
                reg16_2=get16bitsRegisterAddress(REGBCP);
                
                op16=reg16_1->getValue();
                reg16_1->setValue(reg16_2->getValue());
                reg16_2->setValue(op16);

                /* Switch DE and DE'    */
                reg16_1=get16bitsRegisterAddress(REGDE);
                reg16_2=get16bitsRegisterAddress(REGDEP);
                
                op16=reg16_1->getValue();
                reg16_1->setValue(reg16_2->getValue());
                reg16_2->setValue(op16);

                /* Switch HL and HL'    */
                reg16_1=get16bitsRegisterAddress(REGHL);
                reg16_2=get16bitsRegisterAddress(REGHLP);
                
                op16=reg16_1->getValue();
                reg16_1->setValue(reg16_2->getValue());
                reg16_2->setValue(op16);

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

        case CODE_ADDAR:                                         /* This is a ADD A,r  */
            ret=bitToRegister(op1, sop1);

            sprintf(mInstruction, "ADD A,%s", sop1);

            if (pMode==INTP_EXECUTE || pMode==INTP_EXECUTE_BLIND)                            
            {
                reg8_1=get8bitsRegisterAddress(REGA);
                reg8_2=get8bitsRegisterAddress(op1);

                /* Modify flags here before operation   */
                /* Is there an Half Carry ?             */
                H_IS(checkHalfCarryOnAdd8(reg8_1->getValue(), reg8_2->getValue()));
                
                /* Is there a Carry ?                   */
                C_IS(checkCarryOnAdd8(reg8_1->getValue(), reg8_2->getValue()));
                
                /* IS there an overflow ?               */
                PV_IS(checkOverflowOnAdd8(reg8_1->getValue(), reg8_2->getValue()));

                reg8_1->setValue(reg8_1->getValue() + reg8_2->getValue());

                /* Modify flags here after operation    */
                S_IS(SIGN(reg8_1->getValue()));
                Z_IS(ZERO(reg8_1->getValue()));
                N_RESET;

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

        case CODE_ADDAHL:                                         /* This is a ADD A,(HL)  */
            sprintf(mInstruction, "ADD A,(HL)");

            if (pMode==INTP_EXECUTE || pMode==INTP_EXECUTE_BLIND)                            
            {
                reg8_1=get8bitsRegisterAddress(REGA);
                reg16_1=get16bitsRegisterAddress(REGHL);
                val=mMemory->get8bitsValue(reg16_1->getValue());

                /* Modify flags here before operation   */
                /* Is there an Half Carry ?             */
                H_IS(checkHalfCarryOnAdd8(reg8_1->getValue(), val));
                
                /* Is there a Carry ?                   */
                C_IS(checkCarryOnAdd8(reg8_1->getValue(), val));
                
                /* IS there an overflow ?               */
                PV_IS(checkOverflowOnAdd8(reg8_1->getValue(), val));

                reg8_1->setValue(reg8_1->getValue() + val);

                /* Modify flags here after operation    */
                S_IS(SIGN(reg8_1->getValue()));
                Z_IS(ZERO(reg8_1->getValue()));
                N_RESET;

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

        case CODE_DD_ADDAIXD:                                           /* This is a ADD A,(IX+d)  */
            if (SIGN(op1))                                              /* Check if op1 is negative */
            {
                op1=~op1+1;
                address=mRegisterPack.regIX.getValue()-op1;
                sprintf(mInstruction, "ADD A,(IX-#%02X)", op1);
            }
            else
            {
                address=mRegisterPack.regIX.getValue()+op1;
                sprintf(mInstruction, "ADD A,(IX+#%02X)", op1);
            }

            if (pMode==INTP_EXECUTE || pMode==INTP_EXECUTE_BLIND)                            
            {
                reg8_1=get8bitsRegisterAddress(REGA);
                val=mMemory->get8bitsValue(address);

                /* Modify flags here before operation   */
                /* Is there an Half Carry ?             */
                H_IS(checkHalfCarryOnAdd8(reg8_1->getValue(), val));
                
                /* Is there a Carry ?                   */
                C_IS(checkCarryOnAdd8(reg8_1->getValue(), val));
                
                /* IS there an overflow ?               */
                PV_IS(checkOverflowOnAdd8(reg8_1->getValue(), val));

                reg8_1->setValue(reg8_1->getValue() + val);

                /* Modify flags here after operation    */
                S_IS(SIGN(reg8_1->getValue()));
                Z_IS(ZERO(reg8_1->getValue()));
                N_RESET;

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

        case CODE_ANDR:                                         /* This is a AND r  */
            ret=bitToRegister(op1, sop1);

            sprintf(mInstruction, "AND %s", sop1);

            if (pMode==INTP_EXECUTE || pMode==INTP_EXECUTE_BLIND)                            
            {
                reg8_1=get8bitsRegisterAddress(REGA);
                reg8_2=get8bitsRegisterAddress(op1);
                reg8_1->setValue(reg8_1->getValue() & reg8_2->getValue());

                /* Modify flags here after operation   */
                S_IS(SIGN(reg8_1->getValue()));
                Z_IS(ZERO(reg8_1->getValue()));
                H_SET;
                PV_IS(calcParity(reg8_2->getValue()));
                N_RESET;
                C_RESET;

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

        case CODE_ANDN:                                            /* This is a AND n  */
            sprintf(mInstruction, "AND #%02X", op1);

            if (pMode==INTP_EXECUTE || pMode==INTP_EXECUTE_BLIND)                            
            {
                reg8_1=get8bitsRegisterAddress(REGA);
                reg8_1->setValue(reg8_1->getValue() & op1);

                /* Modify flags here after operation   */
                S_IS(SIGN(reg8_1->getValue()));
                Z_IS(ZERO(reg8_1->getValue()));
                H_SET;
                PV_IS(calcParity(reg8_1->getValue()));
                N_RESET;
                C_RESET;

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

        case CODE_ANDHL:                                         /* This is a AND (HL)  */
            sprintf(mInstruction, "AND (HL)");

            if (pMode==INTP_EXECUTE || pMode==INTP_EXECUTE_BLIND)                            
            {
                reg8_1=get8bitsRegisterAddress(REGA);
                reg16_1=get16bitsRegisterAddress(REGHL);
                val=(mMemory->get8bitsValue(reg16_1->getValue()) & reg8_1->getValue());
                reg8_1->setValue(val);

                /* Modify flags here after operation   */
                S_IS(SIGN(val));
                Z_IS(ZERO(val));
                H_SET;
                PV_IS(calcParity(val));
                N_RESET;
                C_RESET;

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

        case CODE_DD_ANDIXD:                                            /* This is a AND (IX+d)  */
            if (SIGN(op1))                                              /* Check if op1 is negative */
            {
                op1=~op1+1;
                address=mRegisterPack.regIX.getValue()-op1;
                sprintf(mInstruction, "AND (IX-#%02X)", op1);
            }
            else
            {
                address=mRegisterPack.regIX.getValue()+op1;
                sprintf(mInstruction, "AND (IX+#%02X)", op1);
            }

            if (pMode==INTP_EXECUTE || pMode==INTP_EXECUTE_BLIND)                            
            {
                reg8_1=get8bitsRegisterAddress(REGA);
                val=(mMemory->get8bitsValue(address) & reg8_1->getValue());
                reg8_1->setValue(val);

                /* Modify flags here after operation   */
                S_IS(SIGN(val));
                Z_IS(ZERO(val));
                H_SET;
                PV_IS(calcParity(val));
                N_RESET;
                C_RESET;

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

            case CODE_FD_ANDIYD:                                            /* This is a AND (IY+d)  */
            if (SIGN(op1))                                                  /* Check if op1 is negative */
            {
                op1=~op1+1;
                address=mRegisterPack.regIY.getValue()-op1;
                sprintf(mInstruction, "AND (IY-#%02X)", op1);
            }
            else
            {
                address=mRegisterPack.regIY.getValue()+op1;
                sprintf(mInstruction, "AND (IY+#%02X)", op1);
            }

            if (pMode==INTP_EXECUTE || pMode==INTP_EXECUTE_BLIND)                            
            {
                reg8_1=get8bitsRegisterAddress(REGA);
                val=(mMemory->get8bitsValue(address) & reg8_1->getValue());
                reg8_1->setValue(val);

                /* Modify flags here after operation   */
                S_IS(SIGN(val));
                Z_IS(ZERO(val));
                H_SET;
                PV_IS(calcParity(val));
                N_RESET;
                C_RESET;

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

        case CODE_CPR:                                         /* This is a CP r  */
            ret=bitToRegister(op1, sop1);

            sprintf(mInstruction, "CP %s", sop1);

            if (pMode==INTP_EXECUTE || pMode==INTP_EXECUTE_BLIND)                            
            {
                reg8_1=get8bitsRegisterAddress(REGA);
                reg8_2=get8bitsRegisterAddress(op1);
                val=reg8_1->getValue() - reg8_2->getValue();           /* A - r is executed    */

                /* Modify flags here after operation   */
                S_IS(SIGN(val));
                Z_IS(ZERO(val));
                H_IS(checkHalfBorrowOnSub8(reg8_1->getValue(), reg8_2->getValue()));
                PV_IS(checkOverflowOnSub8(reg8_1->getValue(), reg8_2->getValue())); 
                
                N_SET;
                C_IS(checkBorrowOnSub8(reg8_1->getValue(), reg8_2->getValue()));

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

        case CODE_CPN:                                            /* This is a CP n  */
            sprintf(mInstruction, "CP #%02X", op1);

            if (pMode==INTP_EXECUTE || pMode==INTP_EXECUTE_BLIND)                            
            {
                reg8_1=get8bitsRegisterAddress(REGA);
                val=reg8_1->getValue() - op1;           /* A - n is executed    */

                /* Modify flags here after operation   */
                S_IS(SIGN(val));
                Z_IS(ZERO(val));
                H_IS(checkHalfBorrowOnSub8(reg8_1->getValue(), op1));
                PV_IS(checkOverflowOnSub8(reg8_1->getValue(), op1)); 
                
                N_SET;
                C_IS(checkBorrowOnSub8(reg8_1->getValue(), op1));

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

        case CODE_CPHL:                                         /* This is a CP (HL)  */
            sprintf(mInstruction, "CP (HL)");

            if (pMode==INTP_EXECUTE || pMode==INTP_EXECUTE_BLIND)                            
            {
                reg8_1=get8bitsRegisterAddress(REGA);
                val=mMemory->get8bitsValue(mRegisterPack.regHL.getValue());
                newVal=reg8_1->getValue() - val;     /* A - (HL) is executed    */

                /* Modify flags here after operation   */
                S_IS(SIGN(newVal));
                Z_IS(ZERO(newVal));
                H_IS(checkHalfBorrowOnSub8(reg8_1->getValue(), val));
                PV_IS(checkOverflowOnSub8(reg8_1->getValue(), val)); 
                
                N_SET;
                C_IS(checkBorrowOnSub8(reg8_1->getValue(), val));

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

        case CODE_DD_CPIXD:                                            /* This is a CP (IX+d)  */
            if (SIGN(op1))                                              /* Check if op1 is negative */
            {
                op1=~op1+1;
                address=mRegisterPack.regIX.getValue()-op1;
                sprintf(mInstruction, "CP (IX-#%02X)", op1);
            }
            else
            {
                address=mRegisterPack.regIX.getValue()+op1;
                sprintf(mInstruction, "CP (IX+#%02X)", op1);
            }

            if (pMode==INTP_EXECUTE || pMode==INTP_EXECUTE_BLIND)                            
            {
                reg8_1=get8bitsRegisterAddress(REGA);
                val=mMemory->get8bitsValue(address);
                newVal=reg8_1->getValue() - val;                /* A - (IX+d) is executed    */

                /* Modify flags here after operation   */
                S_IS(SIGN(newVal));
                Z_IS(ZERO(newVal));
                H_IS(checkHalfBorrowOnSub8(reg8_1->getValue(), val));
                PV_IS(checkOverflowOnSub8(reg8_1->getValue(), val)); 
                
                N_SET;
                C_IS(checkBorrowOnSub8(reg8_1->getValue(), val));

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

            case CODE_FD_CPIYD:                                            /* This is a CP (IY+d)  */
            if (SIGN(op1))                                                  /* Check if op1 is negative */
            {
                op1=~op1+1;
                address=mRegisterPack.regIY.getValue()-op1;
                sprintf(mInstruction, "CP (IY-#%02X)", op1);
            }
            else
            {
                address=mRegisterPack.regIY.getValue()+op1;
                sprintf(mInstruction, "CP (IY+#%02X)", op1);
            }

            if (pMode==INTP_EXECUTE || pMode==INTP_EXECUTE_BLIND)                            
            {
                reg8_1=get8bitsRegisterAddress(REGA);
                val=mMemory->get8bitsValue(address);
                newVal=reg8_1->getValue() - val;                        /* A - (IY+d) is executed    */

                /* Modify flags here after operation   */
                S_IS(SIGN(newVal));
                Z_IS(ZERO(newVal));
                H_IS(checkHalfBorrowOnSub8(reg8_1->getValue(), val));
                PV_IS(checkOverflowOnSub8(reg8_1->getValue(), val)); 
                
                N_SET;
                C_IS(checkBorrowOnSub8(reg8_1->getValue(), val));

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
        

        case CODE_ORR:                                         /* This is a OR r  */
            ret=bitToRegister(op1, sop1);

            sprintf(mInstruction, "OR %s", sop1);

            if (pMode==INTP_EXECUTE || pMode==INTP_EXECUTE_BLIND)                            
            {
                reg8_1=get8bitsRegisterAddress(REGA);
                reg8_2=get8bitsRegisterAddress(op1);
                reg8_1->setValue(reg8_1->getValue() | reg8_2->getValue());

                /* Modify flags here after operation   */
                S_IS(SIGN(reg8_1->getValue()));
                Z_IS(ZERO(reg8_1->getValue()));
                H_SET;
                PV_IS(calcParity(reg8_2->getValue()));
                N_RESET;
                C_RESET;

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

        case CODE_ORN:                                            /* This is a OR n  */
            sprintf(mInstruction, "OR #%02X", op1);

            if (pMode==INTP_EXECUTE || pMode==INTP_EXECUTE_BLIND)                            
            {
                reg8_1=get8bitsRegisterAddress(REGA);
                reg8_1->setValue(reg8_1->getValue() | op1);

                /* Modify flags here after operation   */
                S_IS(SIGN(reg8_1->getValue()));
                Z_IS(ZERO(reg8_1->getValue()));
                H_SET;
                PV_IS(calcParity(reg8_1->getValue()));
                N_RESET;
                C_RESET;

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

        case CODE_ORHL:                                         /* This is a OR (HL)  */
            sprintf(mInstruction, "OR (HL)");

            if (pMode==INTP_EXECUTE || pMode==INTP_EXECUTE_BLIND)                            
            {
                reg8_1=get8bitsRegisterAddress(REGA);
                reg16_1=get16bitsRegisterAddress(REGHL);
                val=(mMemory->get8bitsValue(reg16_1->getValue()) | reg8_1->getValue());
                reg8_1->setValue(val);

                /* Modify flags here after operation   */
                S_IS(SIGN(val));
                Z_IS(ZERO(val));
                H_SET;
                PV_IS(calcParity(val));
                N_RESET;
                C_RESET;

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

        case CODE_DD_ORIXD:                                            /* This is a OR (IX+d)  */
            if (SIGN(op1))                                              /* Check if op1 is negative */
            {
                op1=~op1+1;
                address=mRegisterPack.regIX.getValue()-op1;
                sprintf(mInstruction, "OR (IX-#%02X)", op1);
            }
            else
            {
                address=mRegisterPack.regIX.getValue()+op1;
                sprintf(mInstruction, "OR (IX+#%02X)", op1);
            }

            if (pMode==INTP_EXECUTE || pMode==INTP_EXECUTE_BLIND)                            
            {
                reg8_1=get8bitsRegisterAddress(REGA);
                val=(mMemory->get8bitsValue(address) | reg8_1->getValue());
                reg8_1->setValue(val);

                /* Modify flags here after operation   */
                S_IS(SIGN(val));
                Z_IS(ZERO(val));
                H_SET;
                PV_IS(calcParity(val));
                N_RESET;
                C_RESET;

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

            case CODE_FD_ORIYD:                                             /* This is a OR (IY+d)  */
            if (SIGN(op1))                                                  /* Check if op1 is negative */
            {
                op1=~op1+1;
                address=mRegisterPack.regIY.getValue()-op1;
                sprintf(mInstruction, "OR (IY-#%02X)", op1);
            }
            else
            {
                address=mRegisterPack.regIY.getValue()+op1;
                sprintf(mInstruction, "OR (IY+#%02X)", op1);
            }

            if (pMode==INTP_EXECUTE || pMode==INTP_EXECUTE_BLIND)                            
            {
                reg8_1=get8bitsRegisterAddress(REGA);
                val=(mMemory->get8bitsValue(address) | reg8_1->getValue());
                reg8_1->setValue(val);

                /* Modify flags here after operation   */
                S_IS(SIGN(val));
                Z_IS(ZERO(val));
                H_SET;
                PV_IS(calcParity(val));
                N_RESET;
                C_RESET;

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

        case CODE_XORR:                                         /* This is a XOR r  */
            ret=bitToRegister(op1, sop1);

            sprintf(mInstruction, "XOR %s", sop1);

            if (pMode==INTP_EXECUTE || pMode==INTP_EXECUTE_BLIND)                            
            {
                reg8_1=get8bitsRegisterAddress(REGA);
                reg8_2=get8bitsRegisterAddress(op1);
                reg8_1->setValue(reg8_1->getValue() ^ reg8_2->getValue());

                /* Modify flags here after operation   */
                S_IS(SIGN(reg8_1->getValue()));
                Z_IS(ZERO(reg8_1->getValue()));
                H_SET;
                PV_IS(calcParity(reg8_2->getValue()));
                N_RESET;
                C_RESET;

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

        case CODE_XORN:                                            /* This is a XOR n  */
            sprintf(mInstruction, "XOR #%02X", op1);

            if (pMode==INTP_EXECUTE || pMode==INTP_EXECUTE_BLIND)                            
            {
                reg8_1=get8bitsRegisterAddress(REGA);
                reg8_1->setValue(reg8_1->getValue() ^ op1);

                /* Modify flags here after operation   */
                S_IS(SIGN(reg8_1->getValue()));
                Z_IS(ZERO(reg8_1->getValue()));
                H_SET;
                PV_IS(calcParity(reg8_1->getValue()));
                N_RESET;
                C_RESET;

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

        case CODE_XORHL:                                         /* This is a XOR (HL)  */
            sprintf(mInstruction, "XOR (HL)");

            if (pMode==INTP_EXECUTE || pMode==INTP_EXECUTE_BLIND)                            
            {
                reg8_1=get8bitsRegisterAddress(REGA);
                reg16_1=get16bitsRegisterAddress(REGHL);
                val=(mMemory->get8bitsValue(reg16_1->getValue()) ^ reg8_1->getValue());
                reg8_1->setValue(val);

                /* Modify flags here after operation   */
                S_IS(SIGN(val));
                Z_IS(ZERO(val));
                H_SET;
                PV_IS(calcParity(val));
                N_RESET;
                C_RESET;

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

        case CODE_DD_XORIXD:                                             /* This is a XOR (IX+d)  */
            if (SIGN(op1))                                              /* Check if op1 is negative */
            {
                op1=~op1+1;
                address=mRegisterPack.regIX.getValue()-op1;
                sprintf(mInstruction, "XOR (IX-#%02X)", op1);
            }
            else
            {
                address=mRegisterPack.regIX.getValue()+op1;
                sprintf(mInstruction, "XOR (IX+#%02X)", op1);
            }

            if (pMode==INTP_EXECUTE || pMode==INTP_EXECUTE_BLIND)                            
            {
                reg8_1=get8bitsRegisterAddress(REGA);
                val=(mMemory->get8bitsValue(address) ^ reg8_1->getValue());
                reg8_1->setValue(val);

                /* Modify flags here after operation   */
                S_IS(SIGN(val));
                Z_IS(ZERO(val));
                H_SET;
                PV_IS(calcParity(val));
                N_RESET;
                C_RESET;

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

            case CODE_FD_XORIYD:                                            /* This is a OXR (IY+d)  */
            if (SIGN(op1))                                                  /* Check if op1 is negative */
            {
                op1=~op1+1;
                address=mRegisterPack.regIY.getValue()-op1;
                sprintf(mInstruction, "XOR (IY-#%02X)", op1);
            }
            else
            {
                address=mRegisterPack.regIY.getValue()+op1;
                sprintf(mInstruction, "XOR (IY+#%02X)", op1);
            }

            if (pMode==INTP_EXECUTE || pMode==INTP_EXECUTE_BLIND)                            
            {
                reg8_1=get8bitsRegisterAddress(REGA);
                val=(mMemory->get8bitsValue(address) ^ reg8_1->getValue());
                reg8_1->setValue(val);

                /* Modify flags here after operation   */
                S_IS(SIGN(val));
                Z_IS(ZERO(val));
                H_SET;
                PV_IS(calcParity(val));
                N_RESET;
                C_RESET;

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

        case CODE_DD_INCIXD:                                            /* This is a INC (IX+d)     */
            if (SIGN(op1))                                              /* Check if op1 is negative */
            {
                op1=~op1+1;
                address=mRegisterPack.regIX.getValue()-op1;
                sprintf(mInstruction, "INC (IX-#%02X)", op1);
            }
            else
            {
                address=mRegisterPack.regIX.getValue()+op1;
                sprintf(mInstruction, "INC (IX+#%02X)", op1);
            }

            if (pMode==INTP_EXECUTE || pMode==INTP_EXECUTE_BLIND)                            
            {
                val=mMemory->get8bitsValue(address);

                /* Modify flags here before operation   */
                /* Is there an Half Carry ?             */
                H_IS(checkHalfCarryOnAdd8(val, 1));
                               
                /* IS there an overflow ?               */
                PV_IS(checkOverflowOnAdd8(val, 1));

                mMemory->set8bitsValue(address, val+1);

                /* Modify flags here after operation    */
                S_IS(SIGN(val+1));
                Z_IS(ZERO(val+1));
                N_RESET;

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

        case CODE_FD_INCIYD:                                            /* This is a INC (IY+d)     */
            if (SIGN(op1))                                              /* Check if op1 is negative */
            {
                op1=~op1+1;
                address=mRegisterPack.regIY.getValue()-op1;
                sprintf(mInstruction, "INC (IY-#%02X)", op1);
            }
            else
            {
                address=mRegisterPack.regIY.getValue()+op1;
                sprintf(mInstruction, "INC (IY+#%02X)", op1);
            }

            if (pMode==INTP_EXECUTE || pMode==INTP_EXECUTE_BLIND)                            
            {
                val=mMemory->get8bitsValue(address);

                /* Modify flags here before operation   */
                /* Is there an Half Carry ?             */
                H_IS(checkHalfCarryOnAdd8(val, 1));
                               
                /* IS there an overflow ?               */
                PV_IS(checkOverflowOnAdd8(val, 1));

                mMemory->set8bitsValue(address, val+1);

                /* Modify flags here after operation    */
                S_IS(SIGN(val+1));
                Z_IS(ZERO(val+1));
                N_RESET;

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

        case CODE_DD_DECIXD:                                            /* This is a DEC (IX+d)     */
            if (SIGN(op1))                                              /* Check if op1 is negative */
            {
                op1=~op1+1;
                address=mRegisterPack.regIX.getValue()-op1;
                sprintf(mInstruction, "DEC (IX-#%02X)", op1);
            }
            else
            {
                address=mRegisterPack.regIX.getValue()+op1;
                sprintf(mInstruction, "DEC (IX+#%02X)", op1);
            }

            if (pMode==INTP_EXECUTE || pMode==INTP_EXECUTE_BLIND)                            
            {
                val=mMemory->get8bitsValue(address);

                /* Modify flags here before operation   */
                /* Is there an Half Carry ?             */
                if (val==0x10)
                {
                    H_SET; 
                }
                else
                {
                    H_RESET;
                }
                               
                /* IS there an overflow ?               */
                if (val==0x80)
                {
                    PV_SET; 
                }
                else
                {
                    PV_RESET;
                }

                mMemory->set8bitsValue(address, val-1);

                /* Modify flags here after operation    */
                S_IS(SIGN(val));
                Z_IS(ZERO(val));
                N_RESET;

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

        case CODE_FD_DECIYD:                                            /* This is a DEC (IY+d)     */
            if (SIGN(op1))                                              /* Check if op1 is negative */
            {
                op1=~op1+1;
                address=mRegisterPack.regIY.getValue()-op1;
                sprintf(mInstruction, "DEC (IY-#%02X)", op1);
            }
            else
            {
                address=mRegisterPack.regIX.getValue()+op1;
                sprintf(mInstruction, "DEC (IY+#%02X)", op1);
            }

            if (pMode==INTP_EXECUTE || pMode==INTP_EXECUTE_BLIND)                            
            {
                val=mMemory->get8bitsValue(address);

                /* Modify flags here before operation   */
                /* Is there an Half Carry ?             */
                if (val==0x10)
                {
                    H_SET; 
                }
                else
                {
                    H_RESET;
                }
                               
                /* IS there an overflow ?               */
                if (val==0x80)
                {
                    PV_SET; 
                }
                else
                {
                    PV_RESET;
                }

                mMemory->set8bitsValue(address, val-1);

                /* Modify flags here after operation    */
                S_IS(SIGN(val));
                Z_IS(ZERO(val));
                N_RESET;

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

        case CODE_FD_ADDAIYD:                                           /* This is a ADD A,(IY+d)  */
            if (SIGN(op1))                                              /* Check if op1 is negative */
            {
                op1=~op1+1;
                address=mRegisterPack.regIY.getValue()-op1;
                sprintf(mInstruction, "ADD A,(IY-#%02X)", op1);
            }
            else
            {
                address=mRegisterPack.regIY.getValue()+op1;
                sprintf(mInstruction, "ADD A,(IY+#%02X)", op1);
            }

            if (pMode==INTP_EXECUTE || pMode==INTP_EXECUTE_BLIND)                            
            {
                reg8_1=get8bitsRegisterAddress(REGA);
                val=mMemory->get8bitsValue(address);

                /* Modify flags here before operation   */
                /* Is there an Half Carry ?             */
                H_IS(checkHalfCarryOnAdd8(reg8_1->getValue(), val));
                
                /* Is there a Carry ?                   */
                C_IS(checkCarryOnAdd8(reg8_1->getValue(), val));
                
                /* IS there an overflow ?               */
                PV_IS(checkOverflowOnAdd8(reg8_1->getValue(), val));

                reg8_1->setValue(reg8_1->getValue() + val);

                /* Modify flags here after operation    */
                S_IS(SIGN(reg8_1->getValue()));
                Z_IS(ZERO(reg8_1->getValue()));
                N_RESET;

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

        case CODE_DD_ADCAIXD:                                           /* This is a ADC A,(IX+d)  */
            if (SIGN(op1))                                              /* Check if op1 is negative */
            {
                op1=~op1+1;
                address=mRegisterPack.regIX.getValue()-op1;
                sprintf(mInstruction, "ADC A,(IX-#%02X)", op1);
            }
            else
            {
                address=mRegisterPack.regIX.getValue()+op1;
                sprintf(mInstruction, "ADC A,(IX+#%02X)", op1);
            }

            if (pMode==INTP_EXECUTE || pMode==INTP_EXECUTE_BLIND)                            
            {
                reg8_1=get8bitsRegisterAddress(REGA);
                val=mMemory->get8bitsValue(address);
                carry=(mRegisterPack.regF.getCarryFlag()?1:0);

                /* Modify flags here before operation   */
                /* Is there an Half Carry ?             */
                H_IS(checkHalfCarryOnAdd8(reg8_1->getValue(), val+carry));
                
                /* Is there a Carry ?                   */
                C_IS(checkCarryOnAdd8(reg8_1->getValue(), val+carry));
                
                /* IS there an overflow ?               */
                PV_IS(checkOverflowOnAdd8(reg8_1->getValue(), val+carry));

                reg8_1->setValue(reg8_1->getValue() + val+carry);

                /* Modify flags here after operation    */
                S_IS(SIGN(reg8_1->getValue()));
                Z_IS(ZERO(reg8_1->getValue()));
                N_RESET;

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

        case CODE_FD_ADCAIYD:                                           /* This is a ADC A,(IY+d)  */
            if (SIGN(op1))                                              /* Check if op1 is negative */
            {
                op1=~op1+1;
                address=mRegisterPack.regIY.getValue()-op1;
                sprintf(mInstruction, "ADC A,(IY-#%02X)", op1);
            }
            else
            {
                address=mRegisterPack.regIY.getValue()+op1;
                sprintf(mInstruction, "ADC A,(IY+#%02X)", op1);
            }

            if (pMode==INTP_EXECUTE || pMode==INTP_EXECUTE_BLIND)                            
            {
                reg8_1=get8bitsRegisterAddress(REGA);
                val=mMemory->get8bitsValue(address);
                carry=(mRegisterPack.regF.getCarryFlag()?1:0);

                /* Modify flags here before operation   */
                /* Is there an Half Carry ?             */
                H_IS(checkHalfCarryOnAdd8(reg8_1->getValue(), val+carry));
                
                /* Is there a Carry ?                   */
                C_IS(checkCarryOnAdd8(reg8_1->getValue(), val+carry));
                
                /* IS there an overflow ?               */
                PV_IS(checkOverflowOnAdd8(reg8_1->getValue(), val+carry));

                reg8_1->setValue(reg8_1->getValue() + val+carry);

                /* Modify flags here after operation    */
                S_IS(SIGN(reg8_1->getValue()));
                Z_IS(ZERO(reg8_1->getValue()));
                N_RESET;

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

        case CODE_ADCAR:                                         /* This is a ADC A,r  */
            ret=bitToRegister(op1, sop1);

            sprintf(mInstruction, "ADC A,%s", sop1);

            if (pMode==INTP_EXECUTE || pMode==INTP_EXECUTE_BLIND)                            
            {
                reg8_1=get8bitsRegisterAddress(REGA);
                reg8_2=get8bitsRegisterAddress(op1);
                carry=mRegisterPack.regF.getCarryFlag()?1:0;

                /* Modify flags here before operation   */
                /* Is there an Half Carry ?             */
                H_IS(checkHalfCarryOnAdd8(reg8_1->getValue(), reg8_2->getValue()+carry));
                
                /* Is there a Carry ?                   */
                C_IS(checkCarryOnAdd8(reg8_1->getValue(), reg8_2->getValue()+carry));
                
                /* IS there an overflow ?               */
                PV_IS(checkOverflowOnAdd8(reg8_1->getValue(), reg8_2->getValue()+carry));

                reg8_1->setValue(reg8_1->getValue() + reg8_2->getValue()+carry);

                /* Modify flags here after operation    */
                S_IS(SIGN(reg8_1->getValue()));
                Z_IS(ZERO(reg8_1->getValue()));
                N_RESET;

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

        case CODE_ADCAHL:                                         /* This is a ADC A,(HL)  */
            ret=bitToRegister(op1, sop1);

            sprintf(mInstruction, "ADC A,(HL)");

            if (pMode==INTP_EXECUTE || pMode==INTP_EXECUTE_BLIND)                            
            {
                reg8_1=get8bitsRegisterAddress(REGA);
                reg16_1=get16bitsRegisterAddress(REGHL);
                val=mMemory->get8bitsValue(reg16_1->getValue());
                carry=mRegisterPack.regF.getCarryFlag()?1:0;

                /* Modify flags here before operation   */
                /* Is there an Half Carry ?             */
                H_IS(checkHalfCarryOnAdd8(reg8_1->getValue(), val + carry));
                
                /* Is there a Carry ?                   */
                C_IS(checkCarryOnAdd8(reg8_1->getValue(), val + carry));
                
                /* IS there an overflow ?               */
                PV_IS(checkOverflowOnAdd8(reg8_1->getValue(), val + carry));

                reg8_1->setValue(reg8_1->getValue() + val + carry);

                /* Modify flags here after operation    */
                S_IS(SIGN(reg8_1->getValue()));
                Z_IS(ZERO(reg8_1->getValue()));
                N_RESET;

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

        case CODE_ADDHLRR:                                         /* This is a ADD HL,rr  */
            ret=bitToRegister(op1, sop1);

            sprintf(mInstruction, "ADD HL,%s", sop1);

            if (pMode==INTP_EXECUTE || pMode==INTP_EXECUTE_BLIND)                            
            {
                reg16_1=get16bitsRegisterAddress(REGHL);
                reg16_2=get16bitsRegisterAddress(op1);

                /* Modify flags here before operation   */
                /* Is there an Half Carry ?             */
                H_IS(checkHalfCarryOnAdd16(reg16_1->getValue(), reg16_2->getValue()));
                
                /* Is there a Carry ?                   */
                C_IS(checkCarryOnAdd16(reg16_1->getValue(), reg16_2->getValue()));

                reg16_1->setValue(reg16_1->getValue() + reg16_2->getValue());

                /* Modify flags here after operation    */
                N_RESET;

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

        case CODE_DD_ADDIXPP:                                         /* This is a ADD IX,pp  */
            /* Change HL to IX  */
            if (op2==REGHL)
            {
                op2=REGIX;
            }

            ret=bitToRegister(op2, sop2);

            sprintf(mInstruction, "ADD IX,%s", sop2);

            if (pMode==INTP_EXECUTE || pMode==INTP_EXECUTE_BLIND)                            
            {
                reg16_1=get16bitsRegisterAddress(REGIX);
                reg16_2=get16bitsRegisterAddress(op2);

                /* Modify flags here before operation   */
                /* Is there an Half Carry ?             */
                H_IS(checkHalfCarryOnAdd16(reg16_1->getValue(), reg16_2->getValue()));
                
                /* Is there a Carry ?                   */
                C_IS(checkCarryOnAdd16(reg16_1->getValue(), reg16_2->getValue()));

                reg16_1->setValue(reg16_1->getValue() + reg16_2->getValue());

                /* Modify flags here after operation    */
                N_RESET;

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

        case CODE_FD_ADDIYQQ:                                         /* This is a ADD IY,qq  */
            /* Change HL to IX  */
            if (op2==REGHL)
            {
                op2=REGIY;
            }

            ret=bitToRegister(op2, sop2);

            sprintf(mInstruction, "ADD IY,%s", sop2);

            if (pMode==INTP_EXECUTE || pMode==INTP_EXECUTE_BLIND)                            
            {
                reg16_1=get16bitsRegisterAddress(REGIY);
                reg16_2=get16bitsRegisterAddress(op2);

                /* Modify flags here before operation   */
                /* Is there an Half Carry ?             */
                H_IS(checkHalfCarryOnAdd16(reg16_1->getValue(), reg16_2->getValue()));
                
                /* Is there a Carry ?                   */
                C_IS(checkCarryOnAdd16(reg16_1->getValue(), reg16_2->getValue()));

                reg16_1->setValue(reg16_1->getValue() + reg16_2->getValue());

                /* Modify flags here after operation    */
                N_RESET;

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

        case CODE_ED_ADCHLRR:                                      /* This is a ADC HL,rr  */
            ret=bitToRegister(op1, sop1);

            sprintf(mInstruction, "ADC HL,%s", sop1);

            if (pMode==INTP_EXECUTE || pMode==INTP_EXECUTE_BLIND)                            
            {
                reg16_1=get16bitsRegisterAddress(REGHL);
                reg16_2=get16bitsRegisterAddress(op1);
                carry=(mRegisterPack.regF.getCarryFlag()?1:0);

                /* Modify flags here before operation   */
                /* Is there an Half Carry ?             */
                H_IS(checkHalfCarryOnAdd16(reg16_1->getValue(), reg16_2->getValue()+carry));
                
                /* Is there a Carry ?                   */
                C_IS(checkCarryOnAdd16(reg16_1->getValue(), reg16_2->getValue()+carry));

                reg16_1->setValue(reg16_1->getValue() + reg16_2->getValue()+carry);

                /* Modify flags here after operation    */
                N_RESET;

                Z_IS((reg16_1->getValue()==0)?1:0);
                S_IS((reg16_1->getValue() & 0x7FFF)?1:0);

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

        case CODE_ED_SBCHLRR:                                      /* This is a SBC HL,rr  */
            ret=bitToRegister(op1, sop1);

            sprintf(mInstruction, "SBC HL,%s", sop1);

            if (pMode==INTP_EXECUTE || pMode==INTP_EXECUTE_BLIND)                            
            {
                reg16_1=get16bitsRegisterAddress(REGHL);
                reg16_2=get16bitsRegisterAddress(op1);
                carry=(mRegisterPack.regF.getCarryFlag()?1:0);

                /* Modify flags here before operation   */
                /* Is there an Half Carry ?             */
                H_IS(checkHalfBorrowOnSub16(reg16_1->getValue(), reg16_2->getValue() + carry));
                
                /* Is there a Carry ?                   */
                C_IS(checkBorrowOnSub16(reg16_1->getValue(), reg16_2->getValue() + carry));

                reg16_1->setValue(reg16_1->getValue() - reg16_2->getValue() - carry);

                /* Modify flags here after operation    */
                N_SET;

                Z_IS((reg16_1->getValue()==0)?1:0);
                S_IS((reg16_1->getValue() & 0x7FFF)?1:0);

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

        case CODE_CB_BITBR:                                         /* This is a BIT b,r  */
            ret=bitToRegister(op2, sop2);

            sprintf(mInstruction, "BIT %d,%s", op1, sop2);

            if (pMode==INTP_EXECUTE || pMode==INTP_EXECUTE_BLIND)                            
            {
                reg8_1=get8bitsRegisterAddress(op2);

                /* Modify flags here    */
                Z_IS(reg8_1->getBit(op1)==1?0:1);
                H_SET;
                N_RESET;

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

        case CODE_CB_RESBR:                                         /* This is a RES b,r  */
            ret=bitToRegister(op2, sop2);

            sprintf(mInstruction, "RES %d,%s", op1, sop2);

            if (pMode==INTP_EXECUTE || pMode==INTP_EXECUTE_BLIND)                            
            {
                reg8_1=get8bitsRegisterAddress(op2);
                reg8_1->resetBit(op1);
                
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

        case CODE_CB_SETBR:                                         /* This is a SET b,r  */
            ret=bitToRegister(op2, sop2);

            sprintf(mInstruction, "SET %d,%s", op1, sop2);

            if (pMode==INTP_EXECUTE || pMode==INTP_EXECUTE_BLIND)                            
            {
                reg8_1=get8bitsRegisterAddress(op2);
                reg8_1->setBit(op1);
                
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

        case CODE_CB_BITBHL:                                         /* This is a BIT b,(HL)  */
            sprintf(mInstruction, "BIT %d,(HL)", op1);

            if (pMode==INTP_EXECUTE || pMode==INTP_EXECUTE_BLIND)                            
            {
                reg16_1=get16bitsRegisterAddress(REGHL);
                newVal=mMemory->get8bitsValue(reg16_1->getValue());

                /* Modify flags here    */
                Z_IS(BIT(newVal, op1)==1?0:1);
                H_SET;
                N_RESET;

                if (pMode==INTP_EXECUTE)
                {
                    printf("\n%s was executed\n", mInstruction);
                }
            }
            
            if (pMode==INTP_DISPLAY)
            {
                printf("\n[%42X] is %s\n", codeInHexa, mInstruction);
            }
            break;

        case CODE_DDCB_BITBIXD:                                         /* This is a BIT b,(IX+d)   */
            if (SIGN(op2))                                              /* Check if op2 is negative */
            {
                op2=~op2+1;
                address=mRegisterPack.regIX.getValue()-op2;
                sprintf(mInstruction, "BIT %d,(IX-#%02X)", op1, op2);
            }
            else
            {
                address=mRegisterPack.regIX.getValue()+op2;
                sprintf(mInstruction, "BIT %d,(IX+#%02X)", op1, op2);
            }

            if (pMode==INTP_EXECUTE || pMode==INTP_EXECUTE_BLIND)                            
            {
                newVal=mMemory->get8bitsValue(address);

                /* Modify flags here    */
                Z_IS(BIT(newVal, op1)==1?0:1);
                H_SET;
                N_RESET;

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

        case CODE_FDCB_BITBIYD:                                         /* This is a BIT b,(IY+d)   */
            if (SIGN(op2))                                              /* Check if op2 is negative */
            {
                op2=~op2+1;
                address=mRegisterPack.regIY.getValue()-op2;
                sprintf(mInstruction, "BIT %d,(IY-#%02X)", op1, op2);
            }
            else
            {
                address=mRegisterPack.regIY.getValue()+op2;
                sprintf(mInstruction, "BIT %d,(IY+#%02X)", op1, op2);
            }

            if (pMode==INTP_EXECUTE || pMode==INTP_EXECUTE_BLIND)                            
            {
                newVal=mMemory->get8bitsValue(address);

                /* Modify flags here    */
                Z_IS(BIT(newVal, op1)==1?0:1);
                H_SET;
                N_RESET;

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

        case CODE_DDCB_SETBIXD:                                            /* This is a SET b,(IX+d)   */
            if (SIGN(op2))                                                  /* Check if op2 is negative */
            {
                op2=~op2+1;
                address=mRegisterPack.regIX.getValue()-op2;
                sprintf(mInstruction, "SET %d,(IX-#%02X)", op1, op2);
            }
            else
            {
                address=mRegisterPack.regIX.getValue()+op2;
                sprintf(mInstruction, "SET %d,(IX+#%02X)", op1, op2);
            }

            if (pMode==INTP_EXECUTE || pMode==INTP_EXECUTE_BLIND)                            
            {
                val=mMemory->get8bitsValue(address);
                PUSHBIT(val, 1, op1);
                mMemory->set8bitsValue(address, val);

                /* Modify flags here    */
                /* None here            */

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

        case CODE_FDCB_SETBIYD:                                         /* This is a SET b,(IY+d)   */
            if (SIGN(op2))                                              /* Check if op2 is negative */
            {
                op2=~op2+1;
                address=mRegisterPack.regIY.getValue()-op2;
                sprintf(mInstruction, "SET %d,(IY-#%02X)", op1, op2);
            }
            else
            {
                address=mRegisterPack.regIY.getValue()+op2;
                sprintf(mInstruction, "SET %d,(IY+#%02X)", op1, op2);
            }

            if (pMode==INTP_EXECUTE || pMode==INTP_EXECUTE_BLIND)                            
            {
                val=mMemory->get8bitsValue(address);
                PUSHBIT(val, 1, op1);
                mMemory->set8bitsValue(address, val);

                /* Modify flags here    */
                /* None here            */

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

        case CODE_DDCB_RESBIXD:                                             /* This is a RES b,(IX+d)   */
            if (SIGN(op2))                                                  /* Check if op2 is negative */
            {
                op2=~op2+1;
                address=mRegisterPack.regIX.getValue()-op2;
                sprintf(mInstruction, "RES %d,(IX-#%02X)", op1, op2);
            }
            else
            {
                address=mRegisterPack.regIX.getValue()+op2;
                sprintf(mInstruction, "RES %d,(IX+#%02X)", op1, op2);
            }

            if (pMode==INTP_EXECUTE || pMode==INTP_EXECUTE_BLIND)                            
            {
                val=mMemory->get8bitsValue(address);
                PUSHBIT(val, 0, op1);
                mMemory->set8bitsValue(address, val);

                /* Modify flags here    */
                /* None here            */

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

        case CODE_FDCB_RESBIYD:                                         /* This is a RES b,(IY+d)   */
            if (SIGN(op2))                                              /* Check if op2 is negative */
            {
                op2=~op2+1;
                address=mRegisterPack.regIY.getValue()-op2;
                sprintf(mInstruction, "RES %d,(IY-#%02X)", op1, op2);
            }
            else
            {
                address=mRegisterPack.regIY.getValue()+op2;
                sprintf(mInstruction, "RES %d,(IY+#%02X)", op1, op2);
            }

            if (pMode==INTP_EXECUTE || pMode==INTP_EXECUTE_BLIND)                            
            {
                val=mMemory->get8bitsValue(address);
                PUSHBIT(val, 0, op1);
                mMemory->set8bitsValue(address, val);

                /* Modify flags here    */
                /* None here            */

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

        case CODE_CB_RESBHL:                                         /* This is a RES b,(HL)  */
            sprintf(mInstruction, "RES %d,(HL)", op1);

            if (pMode==INTP_EXECUTE || pMode==INTP_EXECUTE_BLIND)                            
            {
                reg16_1=get16bitsRegisterAddress(REGHL);
                val=mMemory->get8bitsValue(reg16_1->getValue());
                PUSHBIT(val, 0, op1);
                mMemory->set8bitsValue(reg16_1->getValue(), val);

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

        case CODE_CB_SETBHL:                                         /* This is a SET b,(HL)  */
            sprintf(mInstruction, "SET %d,(HL)", op1);

            if (pMode==INTP_EXECUTE || pMode==INTP_EXECUTE_BLIND)                            
            {
                reg16_1=get16bitsRegisterAddress(REGHL);
                val=mMemory->get8bitsValue(reg16_1->getValue());
                PUSHBIT(val, 1, op1);
                mMemory->set8bitsValue(reg16_1->getValue(), val);

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

       case CODE_CALLNN:                                       /* This is a CALL nn                    */
            REVERT(op16);
            sprintf(mInstruction, "CALL #%04X", op16);

            if (pMode==INTP_EXECUTE || pMode==INTP_EXECUTE_BLIND)                            
            {
                mRegisterPack.regSP.setValue(mRegisterPack.regSP.getValue()-2);                 /* The SP is changing   */
                reg16_1=get16bitsRegisterAddress(REGPC);
                mMemory->setAddress(mRegisterPack.regSP.getValue(), reg16_1->getValue()+3);     /* Save the (PC+3)      */
                reg16_1->setValue(op16);                                                        /* The PC is changing   */

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

        case CODE_RET:                                                  /* This is a RET                    */
            sprintf(mInstruction, "RET");

            if (pMode==INTP_EXECUTE || pMode==INTP_EXECUTE_BLIND)                            
            {
                                                               /* The PC is changing       */
                mRegisterPack.regPC.setValue(mMemory->getAddress(mRegisterPack.regSP.getValue()));
                mRegisterPack.regSP.setValue(mRegisterPack.regSP.getValue()+2);             /* The SP is changing       */

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

            case CODE_RETCC:                                                  /* This is a RET cc                   */
            ret=bitToCondition(op1, sop1);
            sprintf(mInstruction, "RET %s", sop1);

            if (pMode==INTP_EXECUTE || pMode==INTP_EXECUTE_BLIND)                            
            {
                if (isConditionTrue(op1))
                {
                    mRegisterPack.regPC.setValue(mMemory->getAddress(mRegisterPack.regSP.getValue()));
                    mRegisterPack.regSP.setValue(mRegisterPack.regSP.getValue()+2);             /* The SP is changing       */
                }                
                else
                {
                    retInterpret=NOTHING_SPECIAL;                         /* Go to next instruction   */
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

        case CODE_RSTP:                                                 /* This is a RST p                  */
            op1=op1*0x8;                                                /* Calc the p                       */          
            sprintf(mInstruction, "RST #%02X", op1);

            if (pMode==INTP_EXECUTE || pMode==INTP_EXECUTE_BLIND)                            
            {
                /* Push the PC  */
                mRegisterPack.regSP.setValue(mRegisterPack.regSP.getValue()-2);             /* Change the SP    */
                reg16_1=get16bitsRegisterAddress(REGPC);
                mMemory->setAddress(mRegisterPack.regSP.getValue(), reg16_1->getValue());

                /* Change the PC    */
                reg16_1->setValue((uint16_t) op1);

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

        case CODE_JPNN:                                                     /* This is a JP nn                    */
            REVERT(op16);
            sprintf(mInstruction, "JP #%04X", op16);

            if (pMode==INTP_EXECUTE || pMode==INTP_EXECUTE_BLIND)                            
            {
                mRegisterPack.regPC.setValue(op16);                         /* The PC is changing       */
                
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

        case CODE_JPHL:                                                     /* This is a JP (HL)                    */
            sprintf(mInstruction, "JP (HL)");

            if (pMode==INTP_EXECUTE || pMode==INTP_EXECUTE_BLIND)                            
            {
                mRegisterPack.regPC.setValue(mRegisterPack.regHL.getValue());           /* The PC is changing       */
                
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

        case CODE_DD_JPIX:                                                     /* This is a JP (IX)                    */
            sprintf(mInstruction, "JP (IX)");

            if (pMode==INTP_EXECUTE || pMode==INTP_EXECUTE_BLIND)                            
            {
                mRegisterPack.regPC.setValue(mRegisterPack.regIX.getValue());           /* The PC is changing       */
                
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

        case CODE_FD_JPIY:                                                     /* This is a JP (IY)                    */
            sprintf(mInstruction, "JP (IY)");

            if (pMode==INTP_EXECUTE || pMode==INTP_EXECUTE_BLIND)                            
            {
                mRegisterPack.regPC.setValue(mRegisterPack.regIY.getValue());           /* The PC is changing       */
                
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

        case CODE_JRE:                                                      /* This is a JR e                       */
            if (op1<=127 || op1>=254)                                       /* Is e positive or negative ?    */
            {
                op1=op1+2;
                op1s=op1;
                sprintf(mInstruction, "JR $+%0d", op1);            
            }
            else
            {
                op1=((~op1&FIRST_LOWEST_BYTE)+1)-2;
                op1s=-op1;
                sprintf(mInstruction, "JR $-%0d", op1);
            }

            if (pMode==INTP_EXECUTE || pMode==INTP_EXECUTE_BLIND)                            
            {
                /* The PC is changing       */
                mRegisterPack.regPC.setValue(mRegisterPack.regPC.getValue()+op1s);                         /* The SP is changing       */

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

        case CODE_JRCE:                                                         /* This is a JR C,e                         */
            if (op1<=127 || op1>=254)                                           /* Is e positive or negative ?              */
            {
                op1=op1+2;
                op1s=op1;
                sprintf(mInstruction, "JR C,$+%0d", op1);            
            }
            else
            {
                op1=((~op1&FIRST_LOWEST_BYTE)+1)-2;
                op1s=-op1;
                sprintf(mInstruction, "JR C,$-%0d", op1);
            }

            if (pMode==INTP_EXECUTE || pMode==INTP_EXECUTE_BLIND)                            
            {
                /* The PC is changing       */
                if (isConditionTrue(CONDC))
                {
                    mRegisterPack.regPC.setValue(mRegisterPack.regPC.getValue()+op1s);      /* The PC is changing       */
                }          
                else
                {
                    retInterpret=NOTHING_SPECIAL;                                           /* Go to next instruction   */
                }     
                

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

        case CODE_JRNCE:                                                        /* This is a JR NC,e                        */
            if (op1<=127 || op1>=254)                                           /* Is e positive or negative ?              */
            {
                op1=op1+2;
                op1s=op1;
                sprintf(mInstruction, "JR NC,$+%0d", op1);            
            }
            else
            {
                op1=((~op1&FIRST_LOWEST_BYTE)+1)-2;
                op1s=-op1;
                sprintf(mInstruction, "JR NC,$-%0d", op1);
            }

            if (pMode==INTP_EXECUTE || pMode==INTP_EXECUTE_BLIND)                            
            {
                /* The PC is changing       */
                if (isConditionTrue(CONDNC))
                {
                    mRegisterPack.regPC.setValue(mRegisterPack.regPC.getValue()+op1s);      /* The PC is changing       */
                }          
                else
                {
                    retInterpret=NOTHING_SPECIAL;                                           /* Go to next instruction   */
                }     
                

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

        case CODE_JRZE:                                                         /* This is a JR Z,e                         */
            if (op1<=127 || op1>=254)                                           /* Is e positive or negative ?              */
            {
                op1=op1+2;
                op1s=op1;
                sprintf(mInstruction, "JR Z,$+%0d", op1);            
            }
            else
            {
                op1=((~op1&FIRST_LOWEST_BYTE)+1)-2;
                op1s=-op1;
                sprintf(mInstruction, "JR Z,$-%0d", op1);
            }

            if (pMode==INTP_EXECUTE || pMode==INTP_EXECUTE_BLIND)                            
            {
                /* The PC is changing       */
                if (isConditionTrue(CONDZ))
                {
                    mRegisterPack.regPC.setValue(mRegisterPack.regPC.getValue()+op1s);      /* The PC is changing       */
                }          
                else
                {
                    retInterpret=NOTHING_SPECIAL;                                           /* Go to next instruction   */
                }     
                

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

        case CODE_JRNZE:                                                        /* This is a JR NZ,e                        */
            if (op1<=127 || op1>=254)                                           /* Is e positive or negative ?              */
            {
                op1=op1+2;
                op1s=op1;
                sprintf(mInstruction, "JR NZ,$+%0d", op1);            
            }
            else
            {
                op1=((~op1&FIRST_LOWEST_BYTE)+1)-2;
                op1s=-op1;
                sprintf(mInstruction, "JR NZ,$-%0d", op1);
            }

            if (pMode==INTP_EXECUTE || pMode==INTP_EXECUTE_BLIND)                            
            {
                /* The PC is changing       */
                if (isConditionTrue(CONDNZ))
                {
                    mRegisterPack.regPC.setValue(mRegisterPack.regPC.getValue()+op1s);      /* The PC is changing       */
                }          
                else
                {
                    retInterpret=NOTHING_SPECIAL;                                           /* Go to next instruction   */
                }     
                

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

        case CODE_DJNZ:                                                  /* This is a DJNZE e              */
            if (op1<=127 || op1>=254)                                     /* Is e positive or negative ?    */
            {
                op1=op1+2;
                op1s=op1;
                sprintf(mInstruction, "DJNZ $+%0d", op1);            
            }
            else
            {
                op1=((~op1&FIRST_LOWEST_BYTE)+1)-2;
                op1s=-op1;
                sprintf(mInstruction, "DJNZ $-%0d", op1);
            }

            if (pMode==INTP_EXECUTE || pMode==INTP_EXECUTE_BLIND)                            
            {
                /* Decremente B */
                mRegisterPack.regB.setValue(mRegisterPack.regB.getValue()-1);

                /* Branch to $+e if B!=0       */
                if (mRegisterPack.regB.getValue())
                {
                    mRegisterPack.regPC.setValue(mRegisterPack.regPC.getValue()+op1s);
                }       
                else 
                {
                    /* Go to next instruction PC=PC+2   */
                    mRegisterPack.regPC.setValue(mRegisterPack.regPC.getValue()+TWO_BYTES/2);
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

       case CODE_CALLCCNN:                                       /* This is a CALL cc,nn                    */
           ret=bitToCondition(op1, sop1);
           REVERT(op16);
           sprintf(mInstruction, "CALL %s,#%04X", sop1, op16);

            if (pMode==INTP_EXECUTE || pMode==INTP_EXECUTE_BLIND)                            
            {
                if (isConditionTrue(op1))
                {
                    mRegisterPack.regSP.setValue(mRegisterPack.regSP.getValue()-2);             /* The SP is changing   */
                    reg16_1=get16bitsRegisterAddress(REGPC);
                    mMemory->setAddress(mRegisterPack.regSP.getValue(), reg16_1->getValue()+3);  /* Save the (PC+3)      */
                    reg16_1->setValue(op16);                                                    /* The PC is changing   */
                }                
                else
                {
                    retInterpret=NOTHING_SPECIAL;                         /* Go to next instruction   */
                }               

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

        case CODE_JPCCNN:                                                       /* This is a JP cc,nn                    */
           ret=bitToCondition(op1, sop1);
           REVERT(op16);
           
           sprintf(mInstruction, "JP %s,#%04X", sop1, op16);

            if (pMode==INTP_EXECUTE || pMode==INTP_EXECUTE_BLIND)                            
            {
                if (isConditionTrue(op1))
                {
                    mRegisterPack.regPC.setValue(op16);                   /* The PC is changing       */
                }          
                else
                {
                    retInterpret=NOTHING_SPECIAL;                         /* Go to next instruction   */
                }                   

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

        case CODE_DDCB_RLCIXD:                                          /* This is a RLC (IX+d)  */        
            if (SIGN(op1))                                              /* Check if op1 is negative */
            {
                op1=~op1+1;
                address=mRegisterPack.regIX.getValue()-op1;
                sprintf(mInstruction, "RLC (IX-#%02X)", op1);
            }
            else
            {
                address=mRegisterPack.regIX.getValue()+op1;
                sprintf(mInstruction, "RLC (IX+#%02X)", op1);
            }

            if (pMode==INTP_EXECUTE || pMode==INTP_EXECUTE_BLIND)                       /* Execute RLC (IX+d)    */
            {
                /* Retrieve the byte and rotate it  */
                val=mMemory->get8bitsValue(address);
                newVal=(val<<1) | BIT(val, 7);
                mMemory->set8bitsValue(address, newVal);

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
                printf("\n[%06X] is %s\n", codeInHexa, mInstruction);
            }
            break;

        case CODE_FDCB_RLCIYD:                                          /* This is a RLC (IY+d)  */        
            if (SIGN(op1))                                              /* Check if op1 is negative */
            {
                op1=~op1+1;
                address=mRegisterPack.regIY.getValue()-op1;
                sprintf(mInstruction, "RLC (IY-#%02X)", op1);
            }
            else
            {
                address=mRegisterPack.regIY.getValue()+op1;
                sprintf(mInstruction, "RLC (IY+#%02X)", op1);
            }

            if (pMode==INTP_EXECUTE || pMode==INTP_EXECUTE_BLIND)                       /* Execute RLC (IY+d)    */
            {
                /* Retrieve the byte and rotate it  */
                val=mMemory->get8bitsValue(address);
                newVal=(val<<1) | BIT(val, 7);
                mMemory->set8bitsValue(address, newVal);

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
                printf("\n[%06X] is %s\n", codeInHexa, mInstruction);
            }
            break;

        case CODE_DDCB_RRCIXD:                                          /* This is a RRC (IX+d)  */        
            if (SIGN(op1))                                              /* Check if op1 is negative */
            {
                op1=~op1+1;
                address=mRegisterPack.regIX.getValue()-op1;
                sprintf(mInstruction, "RRC (IX-#%02X)", op1);
            }
            else
            {
                address=mRegisterPack.regIX.getValue()+op1;
                sprintf(mInstruction, "RRC (IX+#%02X)", op1);
            }

            if (pMode==INTP_EXECUTE || pMode==INTP_EXECUTE_BLIND)                       /* Execute RLC (IX+d)    */
            {
                /* Retrieve the byte and rotate it  */
                /* From RRC (HL)    */
                val=mMemory->get8bitsValue(address);    
                newVal=(val>>1) | (BIT(val, 0)<<7);
                mMemory->set8bitsValue(address, newVal);

                /* Modify flags here    */
                S_IS(SIGN(newVal));
                Z_IS(ZERO(newVal));
                H_RESET;
                PV_IS(EVEN(newVal));
                N_RESET;
                C_IS(BIT(newVal, 7));
               
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

        case CODE_FDCB_RRCIYD:                                          /* This is a RRC (IY+d)  */        
            if (SIGN(op1))                                              /* Check if op1 is negative */
            {
                op1=~op1+1;
                address=mRegisterPack.regIY.getValue()-op1;
                sprintf(mInstruction, "RRC (IY-#%02X)", op1);
            }
            else
            {
                address=mRegisterPack.regIY.getValue()+op1;
                sprintf(mInstruction, "RRC (IY+#%02X)", op1);
            }

            if (pMode==INTP_EXECUTE || pMode==INTP_EXECUTE_BLIND)                       /* Execute RLC (IY+d)    */
            {
                /* Retrieve the byte and rotate it  */
                val=mMemory->get8bitsValue(address);
                newVal=(val>>1) | (BIT(val, 0)<<7);
                mMemory->set8bitsValue(address, newVal);

                /* Modify flags here    */
                S_IS(SIGN(newVal));
                Z_IS(ZERO(newVal));
                H_RESET;
                PV_IS(EVEN(newVal));
                N_RESET;
                C_IS(BIT(newVal, 7));

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

        case CODE_DECRR:                                            /* This is a DEC rr    */
            ret=bitToRegister(op1, sop1);

            sprintf(mInstruction, "DEC %s", sop1);

            if (pMode==INTP_EXECUTE || pMode==INTP_EXECUTE_BLIND)                                
            {
                reg16_1=get16bitsRegisterAddress(op1);
                reg16_1->setValue(reg16_1->getValue()-1);

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

        case CODE_DECHL:                                            /* This is a DEC (HL)    */
            sprintf(mInstruction, "DEC (HL)");

            if (pMode==INTP_EXECUTE || pMode==INTP_EXECUTE_BLIND)                                
            {
                reg16_1=get16bitsRegisterAddress(REGHL);
                val=mMemory->get8bitsValue(reg16_1->getValue())-1;
                mMemory->set8bitsValue(reg16_1->getValue(), val);

                /* Modify flags here    */
                S_IS(SIGN(val));
                Z_IS(ZERO(val));
                N_SET;

                if (val==0x0F)
                {
                    H_SET; 
                }
                else
                {
                    H_RESET;
                }

                if (val==0x7F)
                {
                    PV_SET; 
                }
                else
                {
                    PV_RESET;
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
        
        case CODE_INCHL:                                            /* This is a INC (HL)    */
            sprintf(mInstruction, "INC (HL)");

            if (pMode==INTP_EXECUTE || pMode==INTP_EXECUTE_BLIND)                                
            {
                reg16_1=get16bitsRegisterAddress(REGHL);
                val=mMemory->get8bitsValue(reg16_1->getValue())+1;
                mMemory->set8bitsValue(reg16_1->getValue(), val);

                /* Modify flags here    */
                S_IS(SIGN(val));
                Z_IS(ZERO(val));
                N_RESET;

                if (val==0x10)
                {
                    H_SET;
                }
                else
                {
                    H_RESET;
                }

                if (val==0x80)
                {
                    PV_SET;
                }
                else
                {
                    PV_RESET;
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

        case CODE_RLA:                                         /* This is a RLA  */
            sprintf(mInstruction, "RLA");

            if (pMode==INTP_EXECUTE || pMode==INTP_EXECUTE_BLIND)                            
            {
                reg8_1=get8bitsRegisterAddress(REGA);
                bit=reg8_1->getBit(BIT_7);
                //printf("bit7=%1d\n", bit);
                //printf("carry=%1d\n", mRegisterPack.regF.getBit(BITPOS_FLAG_CARRY));  
                //printf("rl=%02X\n", (reg8_1->getValue()<<1) & FIRST_LOWEST_BYTE);
                
                reg8_1->setValue(((reg8_1->getValue()<<1) & FIRST_LOWEST_BYTE) | mRegisterPack.regF.getBit(BITPOS_FLAG_CARRY));
                //printf("a=%02X\n", reg8_1->getValue());
                mRegisterPack.regF.setCarryFlag((bool) bit);

                /* Modify flags here    */
                H_RESET;
                N_RESET;

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

        case CODE_CB_RLR:                                              /* This is a RL r  */
            ret=bitToRegister(op1, sop1);

            sprintf(mInstruction, "RL %s", sop1);

            if (pMode==INTP_EXECUTE || pMode==INTP_EXECUTE_BLIND)                            
            {
                reg8_1=get8bitsRegisterAddress(op1);
                bit=reg8_1->getBit(BIT_7);
                
                reg8_1->setValue(((reg8_1->getValue()<<1) & FIRST_LOWEST_BYTE) | mRegisterPack.regF.getBit(BITPOS_FLAG_CARRY));
                mRegisterPack.regF.setCarryFlag((bool) bit);

                /* Modify flags here    */
                S_IS(SIGN(newVal));
                Z_IS(ZERO(newVal));
                H_RESET;
                PV_IS(EVEN(newVal));
                N_RESET;

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

        case CODE_RRA:                                         /* This is a RRA  */
            sprintf(mInstruction, "RRA");

            if (pMode==INTP_EXECUTE || pMode==INTP_EXECUTE_BLIND)                            
            {
                reg8_1=get8bitsRegisterAddress(REGA);
                bit=reg8_1->getBit(BIT_0);
                // printf("bit7=%1d\n", bit);
                // printf("carry=%1d\n", mRegisterPack.regF.getBit(BITPOS_FLAG_CARRY));  
                // printf("rl=%02X\n", (reg8_1->getValue()<<1) & FIRST_LOWEST_BYTE);
                
                reg8_1->setValue(((reg8_1->getValue()>>1) & FIRST_LOWEST_BYTE) | (mRegisterPack.regF.getBit(BITPOS_FLAG_CARRY)<<7));
                // printf("a=%02X\n", reg8_1->getValue());
                mRegisterPack.regF.setCarryFlag((bool) bit);
                // printf("carry=%1d\n", mRegisterPack.regF.getBit(BITPOS_FLAG_CARRY));  

                /* Modify flags here    */
                H_RESET;
                N_RESET;

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

        case CODE_CB_RRR:                                         /* This is a RR r  */
            ret=bitToRegister(op1, sop1);

            sprintf(mInstruction, "RR %s", sop1);

            if (pMode==INTP_EXECUTE || pMode==INTP_EXECUTE_BLIND)                            
            {
                reg8_1=get8bitsRegisterAddress(op1);
                bit=reg8_1->getBit(BIT_0);
                
                reg8_1->setValue(((reg8_1->getValue()>>1) & FIRST_LOWEST_BYTE) | (mRegisterPack.regF.getBit(BITPOS_FLAG_CARRY)<<7));
                mRegisterPack.regF.setCarryFlag((bool) bit);

                /* Modify flags here    */
                S_IS(SIGN(newVal));
                Z_IS(ZERO(newVal));
                H_RESET;
                PV_IS(EVEN(newVal));
                N_RESET;

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

        case CODE_CB_SLAR:                                         /* This is a SLA r  */
            ret=bitToRegister(op1, sop1);

            sprintf(mInstruction, "SLA %s", sop1);

            if (pMode==INTP_EXECUTE || pMode==INTP_EXECUTE_BLIND)                            
            {
                reg8_1=get8bitsRegisterAddress(op1);

                mRegisterPack.regF.setCarryFlag((bool) reg8_1->getBit(BIT_7));
                reg8_1->setValue(reg8_1->getValue()<<1);
                newVal=reg8_1->getValue();

                /* Modify flags here    */
                S_IS(SIGN(newVal));
                Z_IS(ZERO(newVal));
                H_RESET;
                PV_IS(EVEN(newVal));
                N_RESET;

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

        case CODE_CB_SLAHL:                                         /* This is a SLA (HL)  */
            sprintf(mInstruction, "SLA (HL)");

            if (pMode==INTP_EXECUTE || pMode==INTP_EXECUTE_BLIND)                            
            {
                /* Retrieve the byte and shift it  */
                newVal=mMemory->get8bitsValue(mRegisterPack.regHL.getValue());
                mRegisterPack.regF.setCarryFlag((bool) BIT(newVal, BIT_7));
                newVal=newVal<<1;
                mMemory->set8bitsValue(mRegisterPack.regHL.getValue(), newVal);            

                /* Modify flags here    */
                S_IS(SIGN(newVal));
                Z_IS(ZERO(newVal));
                H_RESET;
                PV_IS(EVEN(newVal));
                N_RESET;

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

        case CODE_ED_RLD:                                         /* This is a RLD  */
            sprintf(mInstruction, "RLD");

            if (pMode==INTP_EXECUTE || pMode==INTP_EXECUTE_BLIND)                            
            { 
                val=mRegisterPack.regA.getValue() & 0x0F;
                newVal=mMemory->get8bitsValue(mRegisterPack.regHL.getValue());
                mRegisterPack.regA.setValue((mRegisterPack.regA.getValue() & 0xF0) + ((newVal & 0xF0) >> 4));
                newVal=((newVal<<4) & 0xF0) + (val);
                mMemory->set8bitsValue(mRegisterPack.regHL.getValue(), newVal);

                /* Modify flags here    */
                S_IS(SIGN(mRegisterPack.regA.getValue()));
                Z_IS(ZERO(mRegisterPack.regA.getValue()));
                H_RESET;
                PV_IS(EVEN(mRegisterPack.regA.getValue()));
                N_RESET;

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

        case CODE_ED_RRD:                                         /* This is a RRD  */
            sprintf(mInstruction, "RRD");

            if (pMode==INTP_EXECUTE || pMode==INTP_EXECUTE_BLIND)                            
            { 
                val=mRegisterPack.regA.getValue() & 0x0F;
                newVal=mMemory->get8bitsValue(mRegisterPack.regHL.getValue());
                mRegisterPack.regA.setValue((mRegisterPack.regA.getValue() & 0xF0) + (newVal & 0x0F));
                newVal=((newVal & 0xF0) >>4 ) + (val << 4);
                mMemory->set8bitsValue(mRegisterPack.regHL.getValue(), newVal);

                /* Modify flags here    */
                S_IS(SIGN(mRegisterPack.regA.getValue()));
                Z_IS(ZERO(mRegisterPack.regA.getValue()));
                H_RESET;
                PV_IS(EVEN(mRegisterPack.regA.getValue()));
                N_RESET;

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

        case CODE_DDCB_SLAIXD:                                           /* This is a SLA (IX+d)        */
            if (SIGN(op1))                                               /* Check if op1 is negative    */
            {
                op1=~op1+1;
                address=mRegisterPack.regIX.getValue()-op1;
                sprintf(mInstruction, "SLA (IX-#%02X)", op1);
            }
            else
            {
                address=mRegisterPack.regIX.getValue()+op1;
                sprintf(mInstruction, "SLA (IX+#%02X)", op1);
            }

            if (pMode==INTP_EXECUTE || pMode==INTP_EXECUTE_BLIND)                            
            {
                /* Retrieve the byte and shift it  */
                newVal=mMemory->get8bitsValue(address);
                mRegisterPack.regF.setCarryFlag((bool) BIT(newVal, BIT_7));
                newVal=newVal<<1;
                mMemory->set8bitsValue(address, newVal);            

                /* Modify flags here    */
                S_IS(SIGN(newVal));
                Z_IS(ZERO(newVal));
                H_RESET;
                PV_IS(EVEN(newVal));
                N_RESET;

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

        case CODE_FDCB_SLAIYD:                                           /* This is a SLA (IY+d)        */
            if (SIGN(op1))                                               /* Check if op1 is negative    */
            {
                op1=~op1+1;
                address=mRegisterPack.regIY.getValue()-op1;
                sprintf(mInstruction, "SLA (IY-#%02X)", op1);
            }
            else
            {
                address=mRegisterPack.regIY.getValue()+op1;
                sprintf(mInstruction, "SLA (IY+#%02X)", op1);
            }

            if (pMode==INTP_EXECUTE || pMode==INTP_EXECUTE_BLIND)                            
            {
                /* Retrieve the byte and shift it  */
                newVal=mMemory->get8bitsValue(address);
                mRegisterPack.regF.setCarryFlag((bool) BIT(newVal, BIT_7));
                newVal=newVal<<1;
                mMemory->set8bitsValue(address, newVal);            

                /* Modify flags here    */
                S_IS(SIGN(newVal));
                Z_IS(ZERO(newVal));
                H_RESET;
                PV_IS(EVEN(newVal));
                N_RESET;

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

        case CODE_CB_SRAR:                                         /* This is a SRA r  */
            ret=bitToRegister(op1, sop1);

            sprintf(mInstruction, "SRA %s", sop1);

            if (pMode==INTP_EXECUTE || pMode==INTP_EXECUTE_BLIND)                            
            {
                reg8_1=get8bitsRegisterAddress(op1);

                mRegisterPack.regF.setCarryFlag((bool) reg8_1->getBit(BIT_0));
                newVal=reg8_1->getValue()>>1;
                PUSHBIT(newVal, reg8_1->getBit(BIT_7), BIT_7);
                reg8_1->setValue(newVal);
               
                /* Modify flags here    */
                S_IS(SIGN(newVal));
                Z_IS(ZERO(newVal));
                H_RESET;
                PV_IS(EVEN(newVal));
                N_RESET;

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

        case CODE_CB_SRAHL:                                         /* This is a SRA (HL)  */
            sprintf(mInstruction, "SRA (HL)");

            if (pMode==INTP_EXECUTE || pMode==INTP_EXECUTE_BLIND)                            
            {
                /* Retrieve the byte and shift it  */
                newVal=(mMemory->get8bitsValue(mRegisterPack.regHL.getValue()));
                mRegisterPack.regF.setCarryFlag((bool) BIT(newVal, BIT_0));
                newVal=newVal>>1;
                PUSHBIT(newVal, BIT(newVal, BIT_6), BIT_7);
                mMemory->set8bitsValue(mRegisterPack.regHL.getValue(), newVal);        

                /* Modify flags here    */
                S_IS(SIGN(newVal));
                Z_IS(ZERO(newVal));
                H_RESET;
                PV_IS(EVEN(newVal));
                N_RESET;

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

        case CODE_DDCB_SRAIXD:                                           /* This is a SRA (IX+d)        */
            if (SIGN(op1))                                               /* Check if op1 is negative    */
            {
                op1=~op1+1;
                address=mRegisterPack.regIX.getValue()-op1;
                sprintf(mInstruction, "SRA (IX-#%02X)", op1);
            }
            else
            {
                address=mRegisterPack.regIX.getValue()+op1;
                sprintf(mInstruction, "SRA (IX+#%02X)", op1);
            }

            if (pMode==INTP_EXECUTE || pMode==INTP_EXECUTE_BLIND)                            
            {
                /* Retrieve the byte and shift it  */
                newVal=mMemory->get8bitsValue(address);
                mRegisterPack.regF.setCarryFlag((bool) BIT(newVal, BIT_0));
                newVal=newVal>>1;
                PUSHBIT(newVal, BIT(newVal, BIT_6), BIT_7);
                mMemory->set8bitsValue(address, newVal);           

                /* Modify flags here    */
                S_IS(SIGN(newVal));
                Z_IS(ZERO(newVal));
                H_RESET;
                PV_IS(EVEN(newVal));
                N_RESET;

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

        case CODE_FDCB_SRAIYD:                                           /* This is a SRA (IY+d)        */
            if (SIGN(op1))                                               /* Check if op1 is negative    */
            {
                op1=~op1+1;
                address=mRegisterPack.regIY.getValue()-op1;
                sprintf(mInstruction, "SRA (IY-#%02X)", op1);
            }
            else
            {
                address=mRegisterPack.regIY.getValue()+op1;
                sprintf(mInstruction, "SRA (IY+#%02X)", op1);
            }

            if (pMode==INTP_EXECUTE || pMode==INTP_EXECUTE_BLIND)                            
            {
                /* Retrieve the byte and shift it  */
                newVal=mMemory->get8bitsValue(address);
                mRegisterPack.regF.setCarryFlag((bool) BIT(newVal, BIT_0));
                newVal=newVal>>1;
                PUSHBIT(newVal, BIT(newVal, BIT_6), BIT_7);
                mMemory->set8bitsValue(address, newVal);           

                /* Modify flags here    */
                S_IS(SIGN(newVal));
                Z_IS(ZERO(newVal));
                H_RESET;
                PV_IS(EVEN(newVal));
                N_RESET;

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

        case CODE_CB_SRLR:                                         /* This is a SRL r  */
            ret=bitToRegister(op1, sop1);

            sprintf(mInstruction, "SRL %s", sop1);

            if (pMode==INTP_EXECUTE || pMode==INTP_EXECUTE_BLIND)                            
            {
                reg8_1=get8bitsRegisterAddress(op1);

                mRegisterPack.regF.setCarryFlag((bool) reg8_1->getBit(BIT_0));
                newVal=reg8_1->getValue()>>1;
                reg8_1->setValue(newVal);
               
                /* Modify flags here    */
                S_IS(SIGN(newVal));
                Z_IS(ZERO(newVal));
                H_RESET;
                PV_IS(EVEN(newVal));
                N_RESET;

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

        case CODE_CB_SRLHL:                                         /* This is a SRL (HL)  */
            sprintf(mInstruction, "SRL (HL)");

            if (pMode==INTP_EXECUTE || pMode==INTP_EXECUTE_BLIND)                            
            {
                /* Retrieve the byte and shift it  */
                newVal=(mMemory->get8bitsValue(mRegisterPack.regHL.getValue()));
                mRegisterPack.regF.setCarryFlag((bool) BIT(newVal, BIT_0));
                newVal=newVal>>1;
                mMemory->set8bitsValue(mRegisterPack.regHL.getValue(), newVal);        

                /* Modify flags here    */
                S_IS(SIGN(newVal));
                Z_IS(ZERO(newVal));
                H_RESET;
                PV_IS(EVEN(newVal));
                N_RESET;

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

        case CODE_DDCB_SRLIXD:                                           /* This is a SRL (IX+d)        */
            if (SIGN(op1))                                               /* Check if op1 is negative    */
            {
                op1=~op1+1;
                address=mRegisterPack.regIX.getValue()-op1;
                sprintf(mInstruction, "SRL (IX-#%02X)", op1);
            }
            else
            {
                address=mRegisterPack.regIX.getValue()+op1;
                sprintf(mInstruction, "SRL (IX+#%02X)", op1);
            }

            if (pMode==INTP_EXECUTE || pMode==INTP_EXECUTE_BLIND)                            
            {
                /* Retrieve the byte and shift it  */
                newVal=mMemory->get8bitsValue(address);
                mRegisterPack.regF.setCarryFlag((bool) BIT(newVal, BIT_0));
                newVal=newVal>>1;
                mMemory->set8bitsValue(address, newVal);           

                /* Modify flags here    */
                S_IS(SIGN(newVal));
                Z_IS(ZERO(newVal));
                H_RESET;
                PV_IS(EVEN(newVal));
                N_RESET;

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

        case CODE_FDCB_SRLIYD:                                           /* This is a SRL (IY+d)        */
            if (SIGN(op1))                                               /* Check if op1 is negative    */
            {
                op1=~op1+1;
                address=mRegisterPack.regIY.getValue()-op1;
                sprintf(mInstruction, "SRL (IY-#%02X)", op1);
            }
            else
            {
                address=mRegisterPack.regIY.getValue()+op1;
                sprintf(mInstruction, "SRL (IY+#%02X)", op1);
            }

            if (pMode==INTP_EXECUTE || pMode==INTP_EXECUTE_BLIND)                            
            {
                /* Retrieve the byte and shift it  */
                newVal=mMemory->get8bitsValue(address);
                mRegisterPack.regF.setCarryFlag((bool) BIT(newVal, BIT_0));
                newVal=newVal>>1;
                PUSHBIT(newVal, BIT(newVal, BIT_6), BIT_7);
                mMemory->set8bitsValue(address, newVal);           

                /* Modify flags here    */
                S_IS(SIGN(newVal));
                Z_IS(ZERO(newVal));
                H_RESET;
                PV_IS(EVEN(newVal));
                N_RESET;

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

        case CODE_CB_RRHL:                                         /* This is a RR (HL)  */
            ret=bitToRegister(op1, sop1);

            sprintf(mInstruction, "RR (HL)");

            if (pMode==INTP_EXECUTE || pMode==INTP_EXECUTE_BLIND)                            
            {
                /* Retrieve the byte and rotate it  */
                val=mMemory->get8bitsValue(mRegisterPack.regHL.getValue());
                bit=EXTRACT(val, 0, 1);
                
                newVal=((val>>1) & FIRST_LOWEST_BYTE) | (mRegisterPack.regF.getBit(BITPOS_FLAG_CARRY)<<7);

                mMemory->set8bitsValue(mRegisterPack.regHL.getValue(), newVal);            
                mRegisterPack.regF.setCarryFlag((bool) bit);

                /* Modify flags here    */
                S_IS(SIGN(newVal));
                Z_IS(ZERO(newVal));
                H_RESET;
                PV_IS(EVEN(newVal));
                N_RESET;

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

        case CODE_DDCB_RRIXD:                                           /* This is a RR (IX+d)      */
            if (SIGN(op1))                                              /* Check if op1 is negative */
            {
                op1=~op1+1;
                address=mRegisterPack.regIX.getValue()-op1;
                sprintf(mInstruction, "RR (IX-#%02X)", op1);
            }
            else
            {
                address=mRegisterPack.regIX.getValue()+op1;
                sprintf(mInstruction, "RR (IX+#%02X)", op1);
            }

            if (pMode==INTP_EXECUTE || pMode==INTP_EXECUTE_BLIND)                            
            {
                /* Retrieve the byte and rotate it  */
                val=mMemory->get8bitsValue(address);
                bit=EXTRACT(val, 0, 1);
                
                newVal=((val>>1) & FIRST_LOWEST_BYTE) | (mRegisterPack.regF.getBit(BITPOS_FLAG_CARRY)<<7);
                mMemory->set8bitsValue(address, newVal);            
                mRegisterPack.regF.setCarryFlag((bool) bit);

                /* Modify flags here    */
                S_IS(SIGN(newVal));
                Z_IS(ZERO(newVal));
                H_RESET;
                PV_IS(EVEN(newVal));
                N_RESET;

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

        case CODE_FDCB_RRIYD:                                           /* This is a RR (IY+d)  */
            if (SIGN(op1))                                              /* Check if op1 is negative */
            {
                op1=~op1+1;
                address=mRegisterPack.regIY.getValue()-op1;
                sprintf(mInstruction, "RR (IY-#%02X)", op1);
            }
            else
            {
                address=mRegisterPack.regIY.getValue()+op1;
                sprintf(mInstruction, "RR (IY+#%02X)", op1);
            }

            if (pMode==INTP_EXECUTE || pMode==INTP_EXECUTE_BLIND)                            
            {
                /* Retrieve the byte and rotate it  */
                val=mMemory->get8bitsValue(address);
                bit=EXTRACT(val, 0, 1);
                
                newVal=((val>>1) & FIRST_LOWEST_BYTE) | (mRegisterPack.regF.getBit(BITPOS_FLAG_CARRY)<<7);

                mMemory->set8bitsValue(address, newVal);            
                mRegisterPack.regF.setCarryFlag((bool) bit);

                /* Modify flags here    */
                S_IS(SIGN(newVal));
                Z_IS(ZERO(newVal));
                H_RESET;
                PV_IS(EVEN(newVal));
                N_RESET;

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

        case CODE_CPL:                                              /* This is a CPL  */
            sprintf(mInstruction, "CPL");

            if (pMode==INTP_EXECUTE || pMode==INTP_EXECUTE_BLIND)                            
            {
                reg8_1=get8bitsRegisterAddress(REGA);
                reg8_1->setValue(~reg8_1->getValue());

                /* Modify flags here    */
                H_SET;
                N_SET;

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

        case CODE_ED_NEG:                                              /* This is a NEG  */
            sprintf(mInstruction, "NEG");

            if (pMode==INTP_EXECUTE || pMode==INTP_EXECUTE_BLIND)                            
            {
                reg8_1=get8bitsRegisterAddress(REGA);
                val=reg8_1->getValue();
                reg8_1->setValue((~reg8_1->getValue())+1);

                /* Modify flags here    */
                S_IS(SIGN(reg8_1->getValue()));
                Z_IS(ZERO(reg8_1->getValue()));
                N_SET;

                H_IS(checkHalfBorrowOnSub8(0, val));

                /* PV setting   */
                if (val==0x80)
                {
                    PV_SET;
                }
                else
                {
                    PV_RESET;
                }

                /* C setting   */
                if (val!=0x00)
                {
                    C_SET;
                }
                else
                {
                    C_RESET;
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

        case CODE_ED_LDI:                                         /* This is a LDI  */
            sprintf(mInstruction, "LDI");

            if (pMode==INTP_EXECUTE || pMode==INTP_EXECUTE_BLIND)                            
            {
                /* Copy the memory case  */
                val=mMemory->get8bitsValue(mRegisterPack.regHL.getValue());
                mMemory->set8bitsValue(mRegisterPack.regDE.getValue(), val);
                mRegisterPack.regDE.setValue(mRegisterPack.regDE.getValue()+1);
                mRegisterPack.regHL.setValue(mRegisterPack.regHL.getValue()+1);
                mRegisterPack.regBC.setValue(mRegisterPack.regBC.getValue()-1);

                /* Modify flags here    */
                H_RESET;

                if ((mRegisterPack.regBC.getValue()-1) != 0) 
                {
                    PV_SET;
                }
                else
                {
                    PV_RESET;
                }
                
                N_RESET;

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

        case CODE_ED_LDIR:                                         /* This is a LDIR  */
            sprintf(mInstruction, "LDIR");

            if (pMode==INTP_EXECUTE || pMode==INTP_EXECUTE_BLIND)                            
            {
                do 
                {
                    /* Copy the memory block */
                    val=mMemory->get8bitsValue(mRegisterPack.regHL.getValue());
                    mMemory->set8bitsValue(mRegisterPack.regDE.getValue(), val);
                    mRegisterPack.regDE.setValue(mRegisterPack.regDE.getValue()+1);
                    mRegisterPack.regHL.setValue(mRegisterPack.regHL.getValue()+1);
                    mRegisterPack.regBC.setValue(mRegisterPack.regBC.getValue()-1);
                } 
                while (mRegisterPack.regBC.getValue());

                /* Modify flags here    */
                H_RESET;

                if ((mRegisterPack.regBC.getValue()-1) != 0) 
                {
                    PV_SET;
                }
                else
                {
                    PV_RESET;
                }
                
                N_RESET;

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

        case CODE_ED_LDD:                                         /* This is a LDD  */
            sprintf(mInstruction, "LDD");

            if (pMode==INTP_EXECUTE || pMode==INTP_EXECUTE_BLIND)                            
            {
                /* Copy the memory case */
                val=mMemory->get8bitsValue(mRegisterPack.regHL.getValue());
                mMemory->set8bitsValue(mRegisterPack.regDE.getValue(), val);
                mRegisterPack.regDE.setValue(mRegisterPack.regDE.getValue()-1);
                mRegisterPack.regHL.setValue(mRegisterPack.regHL.getValue()-1);
                mRegisterPack.regBC.setValue(mRegisterPack.regBC.getValue()-1);

                /* Modify flags here    */
                H_RESET;

                if ((mRegisterPack.regBC.getValue()-1) != 0) 
                {
                    PV_SET;
                }
                else
                {
                    PV_RESET;
                }
                
                N_RESET;

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

        case CODE_ED_LDDR:                                         /* This is a LDDR  */
            sprintf(mInstruction, "LDDR");

            if (pMode==INTP_EXECUTE || pMode==INTP_EXECUTE_BLIND)                            
            {
                do 
                {
                    /* Copy the memory block */
                    val=mMemory->get8bitsValue(mRegisterPack.regHL.getValue());
                    mMemory->set8bitsValue(mRegisterPack.regDE.getValue(), val);
                    mRegisterPack.regDE.setValue(mRegisterPack.regDE.getValue()-1);
                    mRegisterPack.regHL.setValue(mRegisterPack.regHL.getValue()-1);
                    mRegisterPack.regBC.setValue(mRegisterPack.regBC.getValue()-1);
                } 
                while (mRegisterPack.regBC.getValue());

                /* Modify flags here    */
                H_RESET;

                if ((mRegisterPack.regBC.getValue()-1) != 0) 
                {
                    PV_SET;
                }
                else
                {
                    PV_RESET;
                }
                
                N_RESET;

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

        case CODE_ED_CPI:                                         /* This is a CPI  */
            sprintf(mInstruction, "CPI");

            if (pMode==INTP_EXECUTE || pMode==INTP_EXECUTE_BLIND)                            
            {
                /* Copy the memory case  */
                val=mRegisterPack.regA.getValue() - mMemory->get8bitsValue(mRegisterPack.regHL.getValue());
                mRegisterPack.regHL.setValue(mRegisterPack.regHL.getValue()+1);
                mRegisterPack.regBC.setValue(mRegisterPack.regBC.getValue()-1);

                /* Modify flags here    */
                S_IS(SIGN(val));
                Z_IS(ZERO(val));

                H_IS(checkHalfBorrowOnSub8(mRegisterPack.regA.getValue(), mMemory->get8bitsValue(mRegisterPack.regHL.getValue())));

                if ((mRegisterPack.regBC.getValue()-1) != 0) 
                {
                    PV_SET;
                }
                else
                {
                    PV_RESET;
                }
                
                N_RESET;

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

        case CODE_ED_CPIR:                                         /* This is a CPIR  */
            sprintf(mInstruction, "CPIR");

            if (pMode==INTP_EXECUTE || pMode==INTP_EXECUTE_BLIND)                            
            {
                /* Copy the memory case  */
                do {
                    val=mRegisterPack.regA.getValue() - mMemory->get8bitsValue(mRegisterPack.regHL.getValue());
                    mRegisterPack.regHL.setValue(mRegisterPack.regHL.getValue()+1);
                    mRegisterPack.regBC.setValue(mRegisterPack.regBC.getValue()-1);

                    /* Modify flags here    */
                    S_IS(SIGN(val));
                    Z_IS(ZERO(val));

                    H_IS(checkHalfBorrowOnSub8(mRegisterPack.regA.getValue(), mMemory->get8bitsValue(mRegisterPack.regHL.getValue())));

                    if ((mRegisterPack.regBC.getValue()-1) != 0) 
                    {
                        PV_SET;
                    }
                    else
                    {
                        PV_RESET;
                    }
                    
                    N_RESET;
                } 
                while (mRegisterPack.regBC.getValue() && !mRegisterPack.regF.getZeroFlag());

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

        case CODE_ED_CPD:                                         /* This is a CPD  */
            sprintf(mInstruction, "CPD");

            if (pMode==INTP_EXECUTE || pMode==INTP_EXECUTE_BLIND)                            
            {
                /* Copy the memory case  */
                val=mRegisterPack.regA.getValue() - mMemory->get8bitsValue(mRegisterPack.regHL.getValue());
                mRegisterPack.regHL.setValue(mRegisterPack.regHL.getValue()-1);
                mRegisterPack.regBC.setValue(mRegisterPack.regBC.getValue()-1);

                /* Modify flags here    */
                S_IS(SIGN(val));
                Z_IS(ZERO(val));

                H_IS(checkHalfBorrowOnSub8(mRegisterPack.regA.getValue(), mMemory->get8bitsValue(mRegisterPack.regHL.getValue())));

                if ((mRegisterPack.regBC.getValue()-1) != 0) 
                {
                    PV_SET;
                }
                else
                {
                    PV_RESET;
                }
                
                N_RESET;

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

        case CODE_ED_CPDR:                                         /* This is a CPDR  */
            sprintf(mInstruction, "CPDR");

            if (pMode==INTP_EXECUTE || pMode==INTP_EXECUTE_BLIND)                            
            {
                /* Copy the memory case  */
                do {
                    val=mRegisterPack.regA.getValue() - mMemory->get8bitsValue(mRegisterPack.regHL.getValue());
                    mRegisterPack.regHL.setValue(mRegisterPack.regHL.getValue()-1);
                    mRegisterPack.regBC.setValue(mRegisterPack.regBC.getValue()-1);

                    /* Modify flags here    */
                    S_IS(SIGN(val));
                    Z_IS(ZERO(val));

                    H_IS(checkHalfBorrowOnSub8(mRegisterPack.regA.getValue(), mMemory->get8bitsValue(mRegisterPack.regHL.getValue())));

                    if ((mRegisterPack.regBC.getValue()-1) != 0) 
                    {
                        PV_SET;
                    }
                    else
                    {
                        PV_RESET;
                    }
                    
                    N_RESET;
                } 
                while (mRegisterPack.regBC.getValue() && !mRegisterPack.regF.getZeroFlag());

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

        case CODE_EXSPHL:                                         /* This is a EX (SP),HL  */
            sprintf(mInstruction, "EX (SP),HL");

            if (pMode==INTP_EXECUTE || pMode==INTP_EXECUTE_BLIND)                            
            {
                op16=mMemory->getAddress(mRegisterPack.regSP.getValue());
                mMemory->setAddress(mRegisterPack.regSP.getValue(), mRegisterPack.regHL.getValue());
                mRegisterPack.regHL.setValue(op16);

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

        case CODE_EXDEHL:                                         /* This is a EX DE,HL  */
            sprintf(mInstruction, "EX DE,HL");

            if (pMode==INTP_EXECUTE || pMode==INTP_EXECUTE_BLIND)                            
            {
                op16=mRegisterPack.regDE.getValue();
                mRegisterPack.regDE.setValue(mRegisterPack.regHL.getValue());
                mRegisterPack.regHL.setValue(op16);

                /* No flag changing here    */

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

        case CODE_DD_EXSPIX:                                         /* This is a EX (SP),IX  */
            sprintf(mInstruction, "EX (SP),IX");

            if (pMode==INTP_EXECUTE || pMode==INTP_EXECUTE_BLIND)                            
            {
                op16=mMemory->getAddress(mRegisterPack.regSP.getValue());
                mMemory->setAddress(mRegisterPack.regSP.getValue(), mRegisterPack.regIX.getValue());
                mRegisterPack.regIX.setValue(op16);

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

        case CODE_FD_EXSPIY:                                         /* This is a EX (SP),IY  */
            sprintf(mInstruction, "EX (SP),IY");

            if (pMode==INTP_EXECUTE || pMode==INTP_EXECUTE_BLIND)                            
            {
                op16=mMemory->getAddress(mRegisterPack.regSP.getValue());
                mMemory->setAddress(mRegisterPack.regSP.getValue(), mRegisterPack.regIY.getValue());
                mRegisterPack.regIY.setValue(op16);

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

        case CODE_ADDAN:                             /* This is a ADD A,n    */
            sprintf(mInstruction, "ADD A,#%02X", op1);
            
            if (pMode==INTP_EXECUTE || pMode==INTP_EXECUTE_BLIND)
            {
                reg8_1=get8bitsRegisterAddress(REGA);

                /* Modify flags here before operation   */
                /* Is there an Half Carry ?             */
                H_IS(checkHalfCarryOnAdd8(reg8_1->getValue(), op1));
                
                /* Is there a Carry ?                   */
                C_IS(checkCarryOnAdd8(reg8_1->getValue(), op1));
                
                /* IS there an overflow ?               */
                PV_IS(checkOverflowOnAdd8(reg8_1->getValue(), op1));

                reg8_1->setValue(reg8_1->getValue() + op1);

                /* Modify flags here after operation    */
                S_IS(SIGN(reg8_1->getValue()));
                Z_IS(ZERO(reg8_1->getValue()));
                N_RESET;

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

        case CODE_ADCAN:                             /* This is a ADC A,n    */
            sprintf(mInstruction, "ADC A,#%02X", op1);
            
            if (pMode==INTP_EXECUTE || pMode==INTP_EXECUTE_BLIND)
            {
                reg8_1=get8bitsRegisterAddress(REGA);
                val=(mRegisterPack.regF.getCarryFlag()?1:0);

                /* Modify flags here before operation   */
                /* Is there an Half Carry ?             */
                H_IS(checkHalfCarryOnAdd8(reg8_1->getValue(), op1+val));
                
                /* Is there a Carry ?                   */
                C_IS(checkCarryOnAdd8(reg8_1->getValue(), op1+val));
                
                /* IS there an overflow ?               */
                PV_IS(checkOverflowOnAdd8(reg8_1->getValue(), op1+val));

                reg8_1->setValue(reg8_1->getValue() + op1+val);

                /* Modify flags here after operation    */
                S_IS(SIGN(reg8_1->getValue()));
                Z_IS(ZERO(reg8_1->getValue()));
                N_RESET;

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

        case CODE_SUBAR:                                         /* This is a SUB A,r  */
            ret=bitToRegister(op1, sop1);
            /* to be done   */
            sprintf(mInstruction, "SUB A,%s", sop1);

            if (pMode==INTP_EXECUTE || pMode==INTP_EXECUTE_BLIND)                            
            {
                reg8_1=get8bitsRegisterAddress(REGA);
                reg8_2=get8bitsRegisterAddress(op1);

                /* Modify flags here before operation   */
                /* Is there an Half Carry ?             */
                H_IS(checkHalfBorrowOnSub8(reg8_1->getValue(), reg8_2->getValue()));
                
                /* Is there a Carry ?                   */
                C_IS(checkBorrowOnSub8(reg8_1->getValue(), reg8_2->getValue()));
                
                /* IS there an overflow ?               */
                PV_IS(checkOverflowOnSub8(reg8_1->getValue(), reg8_2->getValue()));

                reg8_1->setValue(reg8_1->getValue() - reg8_2->getValue());

                /* Modify flags here after operation    */
                S_IS(SIGN(reg8_1->getValue()));
                Z_IS(ZERO(reg8_1->getValue()));
                N_SET;

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

        case CODE_SUBAN:                             /* This is a SUB A,n    */
            sprintf(mInstruction, "SUB A,#%02X", op1);
            
            if (pMode==INTP_EXECUTE || pMode==INTP_EXECUTE_BLIND)
            {
                reg8_1=get8bitsRegisterAddress(REGA);

                /* Modify flags here before operation   */
                /* Is there an Half Carry ?             */
                H_IS(checkHalfBorrowOnSub8(reg8_1->getValue(), op1));
                
                /* Is there a Carry ?                   */
                C_IS(checkBorrowOnSub8(reg8_1->getValue(), op1));
                
                /* IS there an overflow ?               */
                PV_IS(checkOverflowOnSub8(reg8_1->getValue(), op1));

                reg8_1->setValue(reg8_1->getValue() - op1);

                /* Modify flags here after operation    */
                S_IS(SIGN(reg8_1->getValue()));
                Z_IS(ZERO(reg8_1->getValue()));
                N_SET;

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

        case CODE_SUBAHL:                                         /* This is a SUB A,(HL)  */
            sprintf(mInstruction, "SUB A,(HL)");

            if (pMode==INTP_EXECUTE || pMode==INTP_EXECUTE_BLIND)                            
            {
                reg8_1=get8bitsRegisterAddress(REGA);
                reg16_1=get16bitsRegisterAddress(REGHL);
                val=mMemory->get8bitsValue(reg16_1->getValue());

                /* Modify flags here before operation   */
                /* Is there an Half Carry ?             */
                H_IS(checkHalfBorrowOnSub8(reg8_1->getValue(), val));
                
                /* Is there a Carry ?                   */
                C_IS(checkBorrowOnSub8(reg8_1->getValue(), val));
                
                /* IS there an overflow ?               */
                PV_IS(checkOverflowOnSub8(reg8_1->getValue(), val));

                reg8_1->setValue(reg8_1->getValue() - val);

                /* Modify flags here after operation    */
                S_IS(SIGN(reg8_1->getValue()));
                Z_IS(ZERO(reg8_1->getValue()));
                N_SET;

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

        case CODE_DD_SUBAIXD:                                           /* This is a SUB A,(IX+d)  */
            if (SIGN(op1))                                              /* Check if op1 is negative */
            {
                op1=~op1+1;
                address=mRegisterPack.regIX.getValue()-op1;
                sprintf(mInstruction, "SUB A,(IX-#%02X)", op1);
            }
            else
            {
                address=mRegisterPack.regIX.getValue()+op1;
                sprintf(mInstruction, "SUB A,(IX+#%02X)", op1);
            }

            if (pMode==INTP_EXECUTE || pMode==INTP_EXECUTE_BLIND)                            
            {
                reg8_1=get8bitsRegisterAddress(REGA);
                val=mMemory->get8bitsValue(address);

                /* Modify flags here before operation   */
                /* Is there an Half Carry ?             */
                H_IS(checkHalfBorrowOnSub8(reg8_1->getValue(), val));
                
                /* Is there a Carry ?                   */
                C_IS(checkBorrowOnSub8(reg8_1->getValue(), val));
                
                /* IS there an overflow ?               */
                PV_IS(checkOverflowOnSub8(reg8_1->getValue(), val));

                reg8_1->setValue(reg8_1->getValue() - val);

                /* Modify flags here after operation    */
                S_IS(SIGN(reg8_1->getValue()));
                Z_IS(ZERO(reg8_1->getValue()));
                N_SET;

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

        case CODE_FD_SUBAIYD:                                           /* This is a SUB A,(IY+d)  */
            if (SIGN(op1))                                              /* Check if op1 is negative */
            {
                op1=~op1+1;
                address=mRegisterPack.regIY.getValue()-op1;
                sprintf(mInstruction, "SUB A,(IY-#%02X)", op1);
            }
            else
            {
                address=mRegisterPack.regIY.getValue()+op1;
                sprintf(mInstruction, "SUB A,(IY+#%02X)", op1);
            }

            if (pMode==INTP_EXECUTE || pMode==INTP_EXECUTE_BLIND)                            
            {
                reg8_1=get8bitsRegisterAddress(REGA);
                val=mMemory->get8bitsValue(address);

                /* Modify flags here before operation   */
                /* Is there an Half Carry ?             */
                H_IS(checkHalfBorrowOnSub8(reg8_1->getValue(), val));
                
                /* Is there a Carry ?                   */
                C_IS(checkBorrowOnSub8(reg8_1->getValue(), val));
                
                /* IS there an overflow ?               */
                PV_IS(checkOverflowOnSub8(reg8_1->getValue(), val));

                reg8_1->setValue(reg8_1->getValue() - val);

                /* Modify flags here after operation    */
                S_IS(SIGN(reg8_1->getValue()));
                Z_IS(ZERO(reg8_1->getValue()));
                N_SET;

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

        case CODE_SBCAR:                                         /* This is a SBC A,r  */
            ret=bitToRegister(op1, sop1);
            /* to be done   */
            sprintf(mInstruction, "SBC A,%s", sop1);

            if (pMode==INTP_EXECUTE || pMode==INTP_EXECUTE_BLIND)                            
            {
                reg8_1=get8bitsRegisterAddress(REGA);
                reg8_2=get8bitsRegisterAddress(op1);
                carry=(mRegisterPack.regF.getCarryFlag()?1:0);

                /* Modify flags here before operation   */
                /* Is there an Half Carry ?             */
                H_IS(checkHalfBorrowOnSub8(reg8_1->getValue(), reg8_2->getValue() + carry));
                
                /* Is there a Carry ?                   */
                C_IS(checkBorrowOnSub8(reg8_1->getValue(), reg8_2->getValue() + carry));
                
                /* IS there an overflow ?               */
                PV_IS(checkOverflowOnSub8(reg8_1->getValue(), reg8_2->getValue() + carry));

                reg8_1->setValue(reg8_1->getValue() - reg8_2->getValue() -  carry);

                /* Modify flags here after operation    */
                S_IS(SIGN(reg8_1->getValue()));
                Z_IS(ZERO(reg8_1->getValue()));
                N_SET;

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

        case CODE_SBCAN:                             /* This is a SBC A,n    */
            sprintf(mInstruction, "SBC A,#%02X", op1);
            
            if (pMode==INTP_EXECUTE || pMode==INTP_EXECUTE_BLIND)
            {
                reg8_1=get8bitsRegisterAddress(REGA);
                carry=(mRegisterPack.regF.getCarryFlag()?1:0);

                /* Modify flags here before operation   */
                /* Is there an Half Carry ?             */
                H_IS(checkHalfBorrowOnSub8(reg8_1->getValue(), op1 + carry));
                
                /* Is there a Carry ?                   */
                C_IS(checkBorrowOnSub8(reg8_1->getValue(), op1 + carry));
                
                /* IS there an overflow ?               */
                PV_IS(checkOverflowOnSub8(reg8_1->getValue(), op1 + carry));

                reg8_1->setValue(reg8_1->getValue() - op1 - carry);

                /* Modify flags here after operation    */
                S_IS(SIGN(reg8_1->getValue()));
                Z_IS(ZERO(reg8_1->getValue()));
                N_SET;

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

        case CODE_SBCAHL:                                         /* This is a SBC A,(HL)  */
            sprintf(mInstruction, "SBC A,(HL)");

            if (pMode==INTP_EXECUTE || pMode==INTP_EXECUTE_BLIND)                            
            {
                reg8_1=get8bitsRegisterAddress(REGA);
                reg16_1=get16bitsRegisterAddress(REGHL);
                val=mMemory->get8bitsValue(reg16_1->getValue());
                carry=(mRegisterPack.regF.getCarryFlag()?1:0);

                /* Modify flags here before operation   */
                /* Is there an Half Carry ?             */
                H_IS(checkHalfBorrowOnSub8(reg8_1->getValue(), val + carry));
                
                /* Is there a Carry ?                   */
                C_IS(checkBorrowOnSub8(reg8_1->getValue(), val + carry));
                
                /* IS there an overflow ?               */
                PV_IS(checkOverflowOnSub8(reg8_1->getValue(), val + carry));

                reg8_1->setValue(reg8_1->getValue() - val - carry);

                /* Modify flags here after operation    */
                S_IS(SIGN(reg8_1->getValue()));
                Z_IS(ZERO(reg8_1->getValue()));
                N_SET;

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

        case CODE_DD_SBCAIXD:                                           /* This is a SBC A,(IX+d)  */
            if (SIGN(op1))                                              /* Check if op1 is negative */
            {
                op1=~op1+1;
                address=mRegisterPack.regIX.getValue()-op1;
                sprintf(mInstruction, "SBC A,(IX-#%02X)", op1);
            }
            else
            {
                address=mRegisterPack.regIX.getValue()+op1;
                sprintf(mInstruction, "SBC A,(IX+#%02X)", op1);
            }

            if (pMode==INTP_EXECUTE || pMode==INTP_EXECUTE_BLIND)                            
            {
                reg8_1=get8bitsRegisterAddress(REGA);
                val=mMemory->get8bitsValue(address);
                carry=(mRegisterPack.regF.getCarryFlag()?1:0);

                /* Modify flags here before operation   */
                /* Is there an Half Carry ?             */
                H_IS(checkHalfBorrowOnSub8(reg8_1->getValue(), val + carry));
                
                /* Is there a Carry ?                   */
                C_IS(checkBorrowOnSub8(reg8_1->getValue(), val + carry));
                
                /* IS there an overflow ?               */
                PV_IS(checkOverflowOnSub8(reg8_1->getValue(), val + carry));

                reg8_1->setValue(reg8_1->getValue() - val - carry);

                /* Modify flags here after operation    */
                S_IS(SIGN(reg8_1->getValue()));
                Z_IS(ZERO(reg8_1->getValue()));
                N_SET;

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

        case CODE_FD_SBCAIYD:                                           /* This is a SBC A,(IY+d)  */
            if (SIGN(op1))                                              /* Check if op1 is negative */
            {
                op1=~op1+1;
                address=mRegisterPack.regIY.getValue()-op1;
                sprintf(mInstruction, "SBC A,(IY-#%02X)", op1);
            }
            else
            {
                address=mRegisterPack.regIY.getValue()+op1;
                sprintf(mInstruction, "SBC A,(IY+#%02X)", op1);
            }

            if (pMode==INTP_EXECUTE || pMode==INTP_EXECUTE_BLIND)                            
            {
                reg8_1=get8bitsRegisterAddress(REGA);
                val=mMemory->get8bitsValue(address);
                carry=(mRegisterPack.regF.getCarryFlag()?1:0);

                /* Modify flags here before operation   */
                /* Is there an Half Carry ?             */
                H_IS(checkHalfBorrowOnSub8(reg8_1->getValue(), val + carry));
                
                /* Is there a Carry ?                   */
                C_IS(checkBorrowOnSub8(reg8_1->getValue(), val + carry));
                
                /* IS there an overflow ?               */
                PV_IS(checkOverflowOnSub8(reg8_1->getValue(), val + carry));

                reg8_1->setValue(reg8_1->getValue() - val - carry);

                /* Modify flags here after operation    */
                S_IS(SIGN(reg8_1->getValue()));
                Z_IS(ZERO(reg8_1->getValue()));
                N_SET;

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

        case CODE_CCF:                                              /* This is a CCF  */
            sprintf(mInstruction, "CCF");

            if (pMode==INTP_EXECUTE || pMode==INTP_EXECUTE_BLIND)                            
            {
                /* Modify flags here    */
                H_IS(mRegisterPack.regF.getCarryFlag());
                N_RESET;
                C_IS(!mRegisterPack.regF.getCarryFlag());

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

        case CODE_SCF:                                              /* This is a SCF  */
            sprintf(mInstruction, "SCF");

            if (pMode==INTP_EXECUTE || pMode==INTP_EXECUTE_BLIND)                            
            {
                /* Modify flags here    */
                H_RESET;
                N_RESET;
                C_SET;

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

        case CODE_DAA:                                              /* This is a DAA  */
            sprintf(mInstruction, "DAA");

            val=mRegisterPack.regA.getValue();

            if (pMode==INTP_EXECUTE || pMode==INTP_EXECUTE_BLIND)                            
            {
                if (N_GET)                  /* Check if the last operation was an addition or a subtraction */
                {
                                            /* This was a subtraction           */
                    if (C_GET)              
                    {                       /* Carry is set                             */
                        if (H_GET)
                        {                   /* Carry is set, Half Carry is set          */
                             /* case 12          */
                            if ((UPPER(val)>=VAL_6) && (LOWER(val)>=VAL_6))                                 
                            {
                                mRegisterPack.regA.setValue(val+0x9A);
                                C_SET;
                            }
                        }
                        else                /* Carry is set, Half Carry is not set      */
                        {
                             /* case 11          */
                            if ((UPPER(val)>=VAL_7) && (LOWER(val)<=VAL_9))                                 
                            {
                                mRegisterPack.regA.setValue(val+0xA0);
                                C_SET;
                            }
                        }
                    }
                    else                    /* Carry is not set                         */
                    {
                        if (H_GET)
                        {                   /* Carry is not set, Half Carry is set      */
                            /* case 10          */
                            if ((UPPER(val)<=VAL_8) && (LOWER(val)>=VAL_6))                                 
                            {
                                mRegisterPack.regA.setValue(val+0xFA);
                                C_RESET;
                            }
                        }
                        else                /* Carry is not set, Half Carry is not set  */
                        {
                            /* case 9           */
                            /* nothing to do    */
                            C_RESET;
                        }
                    }
                }
                else
                {
                                            /* This was an addition                     */
                    if (C_GET)              
                    {                       /* Carry is set                             */
                        if (H_GET)
                        {                   /* Carry is set, Half Carry is set          */
                            /* case 8           */
                            if ((UPPER(val)<=VAL_3) && (LOWER(val)<=VAL_3))                                 
                            {
                                mRegisterPack.regA.setValue(val+0x66);
                                C_SET;
                            }
                        }
                        else                /* Carry is set, Half Carry is not set      */
                        {
                            /* case 6           */
                            if ((UPPER(val)<=VAL_2) && (LOWER(val)<=VAL_9))                                 
                            {
                                mRegisterPack.regA.setValue(val+0x60);
                                C_SET;
                            }

                            /* case 7           */
                            if ((UPPER(val)<=VAL_2) && (LOWER(val)>=VAL_A))                                 
                            {
                                mRegisterPack.regA.setValue(val+0x66);
                                C_SET;
                            }
                        }
                    }
                    else                    /* Carry is not set                         */
                    {
                        if (H_GET)
                        {                   /* Carry is not set, Half Carry is set      */
                            /* case 2           */
                            if ((UPPER(val)<=VAL_9) && (LOWER(val)<=VAL_3))                                 
                            {
                                mRegisterPack.regA.setValue(val+0x6);
                                C_RESET;
                            }

                            /* case 5           */
                            if ((UPPER(val)>=VAL_A) && (LOWER(val)<=VAL_3))                                 
                            {
                                mRegisterPack.regA.setValue(val+0x66);
                                C_SET;
                            }
                        }
                        else                /* Carry is not set, Half Carry is not set  */
                        {
                            /* case 0           */
                            /* nothing to do    */
                            C_RESET;

                            /* case 1           */
                            if ((UPPER(val)<=VAL_8) && (LOWER(val)>=VAL_A))     
                            {
                                mRegisterPack.regA.setValue(val+0x6);
                                C_RESET;
                            }

                            /* case 3           */
                            if ((UPPER(val)>=VAL_A) && (LOWER(val)<=VAL_9))     
                            {
                                mRegisterPack.regA.setValue(val+0x60);
                                C_SET;
                            }

                            /* case 4           */
                            if ((UPPER(val)>=VAL_9) && (LOWER(val)>=VAL_A))     
                            {
                                mRegisterPack.regA.setValue(val+0x66);
                                C_SET;
                            }
                        }
                    }
                }

                val=mRegisterPack.regA.getValue();

                /* Modify flags here    */
                /* To be done           */
                S_IS(SIGN(val));
                Z_IS(ZERO(val));
                PV_IS(EVEN(newVal));

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

        case CODE_ED_LDAI:                             /* This is a LD A,I    */
            sprintf(mInstruction, "LD A,I");
            
            if (pMode==INTP_EXECUTE || pMode==INTP_EXECUTE_BLIND)
            {
                /* Load I register  */
                mRegisterPack.regA.setValue(mRegisterPack.regI.getValue());

                // /* Modify flags here after operation    */
                S_IS(SIGN(mRegisterPack.regI.getValue()));
                Z_IS(ZERO(mRegisterPack.regI.getValue()));
                H_RESET;
                N_RESET;

                /* To do, special affectation   */
                // PV_IS()  -> P/V contains contents of IFF2
                //          -> If an interrupt occurs during execution of this instruction, the Parity flag contains a 0

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

        case CODE_ED_LDAR:                             /* This is a LD A,R    */
            sprintf(mInstruction, "LD A,R");
            
            if (pMode==INTP_EXECUTE || pMode==INTP_EXECUTE_BLIND)
            {
                /* Load I register  */
                mRegisterPack.regA.setValue(mRegisterPack.regR.getValue());

                /* Modify flags here after operation    */
                S_IS(SIGN(mRegisterPack.regR.getValue()));
                Z_IS(ZERO(mRegisterPack.regR.getValue()));
                H_RESET;
                N_RESET;

                /* To do, special affectation   */
                // PV_IS()  -> P/V contains content of IFF2
                //          -> If an interrupt occurs during execution of this instruction, the Parity flag contains a 0

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
        
        case CODE_ED_LDIA:                             /* This is a LD I,A    */
            sprintf(mInstruction, "LD I,A");
            
            if (pMode==INTP_EXECUTE || pMode==INTP_EXECUTE_BLIND)
            {
                /* Load I register  */
                mRegisterPack.regI.setValue(mRegisterPack.regA.getValue());

                /* Modify flags here after operation    */
                /* Nothing here                         */

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

        case CODE_ED_LDRA:                             /* This is a LD R,A    */
            sprintf(mInstruction, "LD R,A");
            
            if (pMode==INTP_EXECUTE || pMode==INTP_EXECUTE_BLIND)
            {
                /* Load I register  */
                mRegisterPack.regR.setValue(mRegisterPack.regA.getValue());

                /* Modify flags here after operation    */
                /* Nothing here                         */

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

        case CODE_DI:                             /* This is a DI    */
            sprintf(mInstruction, "DI");
            
            if (pMode==INTP_EXECUTE || pMode==INTP_EXECUTE_BLIND)
            {
                /* Load I register  */
                mRegisterPack.iff1=false;
                mRegisterPack.iff2=false;

                /* Modify flags here after operation    */
                /* Nothing here                         */

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

        case CODE_EI:                             /* This is a EI    */
            sprintf(mInstruction, "EI");
            
            if (pMode==INTP_EXECUTE || pMode==INTP_EXECUTE_BLIND)
            {
                /* Load I register  */
                mRegisterPack.iff1=true;
                mRegisterPack.iff2=true;

                /* Modify flags here after operation    */
                /* Nothing here                         */

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

#ifdef DEBUG_DISPLAY_COLOR_CHANGED 
    printf("ColorFlagChanged=%s\n", byteToBinary(mRegisterPack.regF.getColorChangedFlag()));
#endif

    /* bottom 2*/
    /*************************************************************************************************************************/

    return retInterpret;
}

/* Check if it will be an half carry on an 8-bit addition.    */
bool Z80Machine::checkHalfCarryOnAdd8(uint8_t pB1, uint8_t pB2)
{
    if ((pB1 & 0x0F) + (pB2 & 0x0F)>0x0F)
    {
        return true;
    }
    else
    {
        return false;
    }
}


/* Check if it will be a carry on an 8-bit addition.          */
bool Z80Machine::checkCarryOnAdd8(uint8_t pB1, uint8_t pB2)
{
    if (pB1 + pB2 > 0xFF)
    {
        return true;
    }
    else
    {
        return false;
    }
}

/* Check if it will be an half carry on an 16-bit addition.    */
bool Z80Machine::checkHalfCarryOnAdd16(uint16_t pW1, uint16_t pW2)
{
    if ((pW1 & 0x0FFF) + (pW2 & 0x0FFF)>0x0FFF)
    {
        return true;
    }
    else
    {
        return false;
    }
}


/* Check if it will be a carry on an 16-bit addition.          */
bool Z80Machine::checkCarryOnAdd16(uint16_t pW1, uint16_t pW2)
{
    if (pW1 + pW2 > 0xFFFF)
    {
        return true;
    }
    else
    {
        return false;
    }
}


/* Check if it will be an overflow on an addition.      */
bool Z80Machine::checkOverflowOnAdd8(uint8_t pB1, uint8_t pB2)
{
    if ((SIGN(pB1)==SIGN(pB2)) && (SIGN(pB1) != SIGN(pB1+pB2)))
    {
        return true;
    }
    else
    {
        return false;
    }
}


/* Check if it will be an overflow on an substraction.      */
bool Z80Machine::checkOverflowOnSub8(uint8_t pB1, uint8_t pB2)
{
    if ((SIGN(pB1)!=SIGN(pB2)) && (SIGN(pB1) != SIGN(pB1-pB2)))
    {
        return true;
    }
    else
    {
        return false;
    }
}

/* Check if it will be an half borrow on an 8-bit substraction  */
bool Z80Machine::checkHalfBorrowOnSub8(uint8_t pB1, uint8_t pB2)
{
    if ((pB1 & 0x0F) < (pB2 & 0x0F))
    {
        return true;
    }
    else
    {
        return false;
    }
}

/* Check if it will be an half borrow on an 8-bit substraction  */
bool Z80Machine::checkHalfBorrowOnSub16(uint16_t pW1, uint16_t pW2)
{
    if ((pW1 & 0x0FFF) < (pW2 & 0x0FFF))
    {
        return true;
    }
    else
    {
        return false;
    }
}

/* Check if it will be a borrow on an 8-bit substraction */
bool Z80Machine::checkBorrowOnSub8(uint8_t pB1, uint8_t pB2)
{
    if (pB1 < pB2)
    {
        return true;
    }
    else
    {
        return false;
    }
}


/* Check if it will be a borrow on an 16-bit substraction   */
bool Z80Machine::checkBorrowOnSub16(uint16_t pW1, uint16_t pW2)
{
    if (pW1 < pW2)
    {
        return true;
    }
    else
    {
        return false;
    }
}


/* Used to cut the instruction  */
int8_t Z80Machine::cutInstruction(char *pInstruction, char *pInst, char *pOp1, char *pOp2)
{
    int8_t nbOfComp=0;
    int8_t retCheck=0;
    uint8_t len=0;
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
        //uint8_t pos=charPos-pInst;
        //printf("pos=%d\n", pos);
        strcpy(pOp1, charPos+1);
        pInst[charPos-pInst]='\0';
        //pInst[pos]='\0';
        //printf("inst=%s\n", pInst);
        
        /* Is there a ',' into the instruction ?  */
        if (!(charPos=strchr(pOp1, ',')))
        {
            nbOfComp=2;
        }
        else
        {                                       /* Extract op1 and op2      */
            len=strlen(charPos);
            strcpy(pOp2, charPos+1);
            pOp2[len-1]='\0';
            pOp1[charPos-pOp1]='\0';
            
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
    uint32_t retCode=NOT_DECODED;
    char str_inst[MAX_LEN];
    char str_op1[MAX_OP_LENGTH];
    char str_op2[MAX_OP_LENGTH];
    char *str_ptr=NULL;
    uint8_t nbOfComponents=0;
    uint8_t val;
    uint8_t lenEff=0;
    int8_t retCheck=0;
    uint16_t word=0;
    bool isIX=false;
    bool isIY=false;

    *pLen=0;                             /* Init the length counter  */
    nbOfComponents=cutInstruction(pInstruction, str_inst, str_op1, str_op2);

#ifdef DEBUG_DISPLAY_CUTI_DATA
    printf(">>> <%s> <%s> <%s>\n", str_inst, str_op1, str_op2);
    printf("comp=%d\n", nbOfComponents);
#endif

    /* bottom 3 */

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

            if (!strcmp(str_inst, "EI"))                          /* A EI is present     */
            {
                retCode=CODE_EI;
                *pLen=ONE_BYTE;
            }
            
            if (!strcmp(str_inst, "DI"))                          /* A DI is present     */
            {
                retCode=CODE_DI;
                *pLen=ONE_BYTE;
            }

            if (!strcmp(str_inst, "CPL"))                          /* A CPL is present     */
            {
                retCode=CODE_CPL;
                *pLen=ONE_BYTE;
            }

            if (!strcmp(str_inst, "NEG"))                          /* A NEG is present     */
            {
                retCode=CODE_ED_NEG;
                *pLen=TWO_BYTES;
            }

            if (!strcmp(str_inst, "CCF"))                          /* A CCF is present     */
            {
                retCode=CODE_CCF;
                *pLen=ONE_BYTE;
            }

            if (!strcmp(str_inst, "RET"))                          /* A RET is present     */
            {
                retCode=CODE_RET;
                *pLen=ONE_BYTE;
            }

            if (!strcmp(str_inst, "SCF"))                          /* A SCF is present     */
            {
                retCode=CODE_SCF;
                *pLen=ONE_BYTE;
            }

            if (!strcmp(str_inst, "RLCA"))                          /* A RLCA is present     */
            {
                retCode=CODE_RLCA;
                *pLen=ONE_BYTE;
            }

            if (!strcmp(str_inst, "RRCA"))                          /* A RRCA is present     */
            {
                retCode=CODE_RRCA;
                *pLen=ONE_BYTE;
            }

            if (!strcmp(str_inst, "RLA"))                           /* A RLA is present     */
            {
                retCode=CODE_RLA;
                *pLen=ONE_BYTE;
            }

            if (!strcmp(str_inst, "RRA"))                           /* A RRA is present     */
            {
                retCode=CODE_RRA;
                *pLen=ONE_BYTE;
            }

            if (!strcmp(str_inst, "RLD"))                          /* A RLD is present     */
            {
                retCode=CODE_ED_RLD;
                *pLen=TWO_BYTES;
            }

            if (!strcmp(str_inst, "RRD"))                          /* A RRD is present     */
            {
                retCode=CODE_ED_RRD;
                *pLen=TWO_BYTES;
            }

            if (!strcmp(str_inst, "EXX"))                          /* A EXX is present     */
            {
                retCode=CODE_EXX;
                *pLen=ONE_BYTE;
            }

            if (!strcmp(str_inst, "LDI"))                          /* A LDI is present     */
            {
                retCode=CODE_ED_LDI;
                *pLen=TWO_BYTES;
            }

            if (!strcmp(str_inst, "LDIR"))                          /* A LDIR is present     */
            {
                retCode=CODE_ED_LDIR;
                *pLen=TWO_BYTES;
            }

            if (!strcmp(str_inst, "LDD"))                          /* A LDD is present     */
            {
                retCode=CODE_ED_LDD;
                *pLen=TWO_BYTES;
            }

            if (!strcmp(str_inst, "LDDR"))                          /* A LDDR is present     */
            {
                retCode=CODE_ED_LDDR;
                *pLen=TWO_BYTES;
            }

            if (!strcmp(str_inst, "CPI"))                          /* A CPI is present     */
            {
                retCode=CODE_ED_CPI;
                *pLen=TWO_BYTES;
            }

            if (!strcmp(str_inst, "CPIR"))                          /* A CPIR is present     */
            {
                retCode=CODE_ED_CPIR;
                *pLen=TWO_BYTES;
            }

            if (!strcmp(str_inst, "CPD"))                          /* A CPD is present     */
            {
                retCode=CODE_ED_CPD;
                *pLen=TWO_BYTES;
            }

            if (!strcmp(str_inst, "CPDR"))                          /* A CPDR is present     */
            {
                retCode=CODE_ED_CPDR;
                *pLen=TWO_BYTES;
            }

            if (!strcmp(str_inst, "DAA"))                           /* A DAA is present     */
            {
                retCode=CODE_DAA;
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

                /* Check if it is a RLC (IX+d) or RLC (IY+d) instruction  */
                if (((strlen(str_op1)==7) || (strlen(str_op1)==8)) && (strstr(str_op1, "IX") || strstr(str_op1, "IY")) && strchr(str_op1, '(') && strchr(str_op1, ')') && strchr(str_op1, '+'))
                {
                    if (strstr(str_op1, "IX"))
                    {
                        retCode=CODE_DDCB_RLCIXD;
                    }

                    if (strstr(str_op1, "IY"))
                    {
                        retCode=CODE_FDCB_RLCIYD;
                    }

                    /* Clean the (IX+d) or (IY+d) for Op1 */
                    retCheck=clean_ixn(str_op1);
                    retCode=retCode+(toValue(str_op1+1, pLen, &lenEff)<<SIZE_1_BYTE);         /* Prepare the RLC (IX+d) or the RLC (IY+d)  */

                    *pLen=FOUR_BYTES;
                }
            }

            if (!strcmp(str_inst, "RRC"))                           /* A RRC instruction is present         */
            {
                if (strlen(str_op1)==1)                             /* Check if it is a RRC r instruction   */
                {
                    retCode=CODE_CB_RRCR;                           /* Prepare the RRC r                    */
                    *pLen=TWO_BYTES;

                    retCheck=clean_r(str_op1);
                    
                    PUSHBIT(retCode, registerToBit(str_op1), 0);    /* Add the register as bits             */
                }

                if (!strcmp(str_op1, "(HL)"))                       /* Check if it is a RRC (HL) instruction*/
                {
                    retCode=CODE_CB_RRCHL;                           /* Prepare the RRC (HL))                    */
                    *pLen=TWO_BYTES;
                }

                /* Check if it is a RRC (IX+d) or RRC (IY+d) instruction  */
                if (((strlen(str_op1)==7) || (strlen(str_op1)==8)) && (strstr(str_op1, "IX") || strstr(str_op1, "IY")) && strchr(str_op1, '(') && strchr(str_op1, ')') && strchr(str_op1, '+'))
                {
                    if (strstr(str_op1, "IX"))
                    {
                        retCode=CODE_DDCB_RRCIXD;
                    }

                    if (strstr(str_op1, "IY"))
                    {
                        retCode=CODE_FDCB_RRCIYD;
                    }

                    /* Clean the (IX+d) or (IY+d) for Op1 */
                    retCheck=clean_ixn(str_op1);
                    retCode=retCode+(toValue(str_op1+1, pLen, &lenEff)<<SIZE_1_BYTE);         /* Prepare the RLC (IX+d) or the RLC (IY+d)  */

                    *pLen=FOUR_BYTES;
                }
                
            }

            if (!strcmp(str_inst, "RL"))                            /* A RL instruction is present         */
            {
                if (strlen(str_op1)==1)                             /* Check if it is a RLC r instruction   */
                {
                    retCode=CODE_CB_RLR;                            /* Prepare the RLC r                    */
                    *pLen=TWO_BYTES;

                    retCheck=clean_r(str_op1);
                    
                    PUSHBIT(retCode, registerToBit(str_op1), 0);    /* Add the register as bits             */
                }

                if (!strcmp(str_op1, "(HL)"))                       /* Check if it is a RL (HL) instruction*/
                {
                    retCode=CODE_CB_RLHL;                           /* Prepare the RL (HL)                    */
                    *pLen=TWO_BYTES;
                }


                /* Check if it is a RL (IX+d) or RL (IY+d) instruction  */
                if (((strlen(str_op1)==7) || (strlen(str_op1)==8)) && (strstr(str_op1, "IX") || strstr(str_op1, "IY")) && strchr(str_op1, '(') && strchr(str_op1, ')') && strchr(str_op1, '+'))
                {
                    if (strstr(str_op1, "IX"))
                    {
                        retCode=CODE_DDCB_RLIXD;
                    }

                    if (strstr(str_op1, "IY"))
                    {
                        retCode=CODE_FDCB_RLIYD;
                    }

                    /* Clean the (IX+d) or (IY+d) for Op1 */
                    retCheck=clean_ixn(str_op1);
                    retCode=retCode+(toValue(str_op1+1, pLen, &lenEff)<<SIZE_1_BYTE);         /* Prepare the RLC (IX+d) or the RLC (IY+d)  */

                    *pLen=FOUR_BYTES;
                }
            }

            if (!strcmp(str_inst, "RR"))                            /* A RR instruction is present         */
            {
                if (strlen(str_op1)==1)                             /* Check if it is a RR r instruction   */
                {
                    retCode=CODE_CB_RRR;                            /* Prepare the RR r                    */
                    *pLen=TWO_BYTES;

                    retCheck=clean_r(str_op1);
                    
                    PUSHBIT(retCode, registerToBit(str_op1), 0);    /* Add the register as bits             */
                }

                if (!strcmp(str_op1, "(HL)"))                       /* Check if it is a RR (HL) instruction*/
                {
                    retCode=CODE_CB_RRHL;                           /* Prepare the RR (HL)                    */
                    *pLen=TWO_BYTES;
                }

                /* Check if it is a RR (IX+d) or RR (IY+d) instruction  */
                if (((strlen(str_op1)==7) || (strlen(str_op1)==8)) && (strstr(str_op1, "IX") || strstr(str_op1, "IY")) && strchr(str_op1, '(') && strchr(str_op1, ')') && strchr(str_op1, '+'))
                {
                    if (strstr(str_op1, "IX"))
                    {
                        retCode=CODE_DDCB_RRIXD;
                    }

                    if (strstr(str_op1, "IY"))
                    {
                        retCode=CODE_FDCB_RRIYD;
                    }

                    /* Clean the (IX+d) or (IY+d) for Op1 */
                    retCheck=clean_ixn(str_op1);
                    retCode=retCode+(toValue(str_op1+1, pLen, &lenEff)<<SIZE_1_BYTE);         /* Prepare the RLC (IX+d) or the RLC (IY+d)  */

                    *pLen=FOUR_BYTES;
                }
            }

            if (!strcmp(str_inst, "SLA"))                           /* A SLA instruction is present         */
            {
                if (strlen(str_op1)==1)                             /* Check if it is a SLA r instruction   */
                {
                    retCode=CODE_CB_SLAR;                           /* Prepare the SLA r                    */
                    *pLen=TWO_BYTES;

                    retCheck=clean_r(str_op1);
                    
                    PUSHBIT(retCode, registerToBit(str_op1), 0);    /* Add the register as bits             */
                }

                if (!strcmp(str_op1, "(HL)"))                       /* Check if it is a SLA (HL) instruction*/
                {
                    retCode=CODE_CB_SLAHL;                           /* Prepare the SLA (HL))                    */
                    *pLen=TWO_BYTES;
                }

                /* Check if it is a SLA (IX+d) or SLA (IY+d) instruction  */
                if (((strlen(str_op1)==7) || (strlen(str_op1)==8)) && (strstr(str_op1, "IX") || strstr(str_op1, "IY")) && strchr(str_op1, '(') && strchr(str_op1, ')') && strchr(str_op1, '+'))
                {
                    if (strstr(str_op1, "IX"))
                    {
                        retCode=CODE_DDCB_SLAIXD;
                    }

                    if (strstr(str_op1, "IY"))
                    {
                        retCode=CODE_FDCB_SLAIYD;
                    }

                    /* Clean the (IX+d) or (IY+d) for Op1 */
                    retCheck=clean_ixn(str_op1);
                    retCode=retCode+(toValue(str_op1+1, pLen, &lenEff)<<SIZE_1_BYTE);         /* Prepare the SLA (IX+d) or the SLA (IY+d)  */

                    *pLen=FOUR_BYTES;
                }            
            }

            if (!strcmp(str_inst, "SRA"))                           /* A SRA instruction is present         */
            {
                if (strlen(str_op1)==1)                             /* Check if it is a SRA r instruction   */
                {
                    retCode=CODE_CB_SRAR;                           /* Prepare the SRA r                    */
                    *pLen=TWO_BYTES;

                    retCheck=clean_r(str_op1);
                    
                    PUSHBIT(retCode, registerToBit(str_op1), 0);    /* Add the register as bits             */
                }

                if (!strcmp(str_op1, "(HL)"))                       /* Check if it is a SRA (HL) instruction*/
                {
                    retCode=CODE_CB_SRAHL;                           /* Prepare the SRA (HL))                    */
                    *pLen=TWO_BYTES;
                }

                /* Check if it is a SRA (IX+d) or SRA (IY+d) instruction  */
                if (((strlen(str_op1)==7) || (strlen(str_op1)==8)) && (strstr(str_op1, "IX") || strstr(str_op1, "IY")) && strchr(str_op1, '(') && strchr(str_op1, ')') && strchr(str_op1, '+'))
                {
                    if (strstr(str_op1, "IX"))
                    {
                        retCode=CODE_DDCB_SRAIXD;
                    }

                    if (strstr(str_op1, "IY"))
                    {
                        retCode=CODE_FDCB_SRAIYD;
                    }

                    /* Clean the (IX+d) or (IY+d) for Op1 */
                    retCheck=clean_ixn(str_op1);
                    retCode=retCode+(toValue(str_op1+1, pLen, &lenEff)<<SIZE_1_BYTE);         /* Prepare the SRA (IX+d) or the SRA (IY+d)  */

                    *pLen=FOUR_BYTES;
                }
                
            }

            if (!strcmp(str_inst, "SRL"))                           /* A SRL instruction is present         */
            {
                if (strlen(str_op1)==1)                             /* Check if it is a SRA r instruction   */
                {
                    retCode=CODE_CB_SRLR;                           /* Prepare the SRL r                    */
                    *pLen=TWO_BYTES;

                    retCheck=clean_r(str_op1);
                    
                    PUSHBIT(retCode, registerToBit(str_op1), 0);    /* Add the register as bits             */
                }

                if (!strcmp(str_op1, "(HL)"))                       /* Check if it is a SRL (HL) instruction*/
                {
                    retCode=CODE_CB_SRLHL;                          /* Prepare the SRL (HL))                    */
                    *pLen=TWO_BYTES;
                }

                /* Check if it is a SRL (IX+d) or SRL (IY+d) instruction  */
                if (((strlen(str_op1)==7) || (strlen(str_op1)==8)) && (strstr(str_op1, "IX") || strstr(str_op1, "IY")) && strchr(str_op1, '(') && strchr(str_op1, ')') && strchr(str_op1, '+'))
                {
                    if (strstr(str_op1, "IX"))
                    {
                        retCode=CODE_DDCB_SRLIXD;
                        printf("THIS IS CODE_DDCB_SRLIXD\n");
                    }

                    if (strstr(str_op1, "IY"))
                    {
                        retCode=CODE_FDCB_SRLIYD;
                        printf("THIS IS CODE_FDCB_SRLIYD\n");
                    }

                    /* Clean the (IX+d) or (IY+d) for Op1 */
                    retCheck=clean_ixn(str_op1);
                    retCode=retCode+(toValue(str_op1+1, pLen, &lenEff)<<SIZE_1_BYTE);         /* Prepare the SRL (IX+d) or the SRA (IY+d)  */

                    *pLen=FOUR_BYTES;
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
                    
                    PUSHBIT(retCode, registerToBit(str_op1), 4);    /* Add the register as bits                 */
                }

                if (!strcmp(str_op1, "(HL)"))                        /* Check if it is a INC (HL) instruction    */
                {
                    retCode=CODE_INCHL;                             /* Prepare the INC (HL)                     */
                    *pLen=ONE_BYTE;
                }

                if (!strcmp(str_op1, "IX"))                         /* Check if it is a INC IX instruction      */
                {
                    retCode=CODE_DD_INCIX;                          /* Prepare the INC IX                       */
                    *pLen=TWO_BYTES;
                }

                if (!strcmp(str_op1, "IY"))                         /* Check if it is a INC IY instruction      */
                {
                    printf("CODE_DD_INCIY\n");
                    retCode=CODE_FD_INCIY;                          /* Prepare the INC IY                       */
                    *pLen=TWO_BYTES;
                }

                /* Check if it is a INC (IX+d) instruction    */
                if (((strlen(str_op1)==7) || (strlen(str_op1)==8)) && (strstr(str_op1, "IX") || strstr(str_op1, "IY")))     
                {
                    printf("CODE_DD_INCIYD\n");
                    if (strstr(str_op1, "IX"))
                    {
                        retCode=CODE_DD_INCIXD;
                    }

                    if (strstr(str_op1, "IY"))
                    {
                        retCode=CODE_FD_INCIYD;
                    }

                    /* Clean the (IX+d) or (IY+d) for Op1 */
                    retCheck=clean_ixn(str_op1);

                    retCode=(retCode<<8)+toValue(str_op1+1, pLen, &lenEff);
                    *pLen=THREE_BYTES;
                    str_op1[0]='\0';
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

                if (strlen(str_op1)==2)                             /* Check if it is a DEC rr instruction   */
                {
                    retCode=CODE_DECRR;                              /* Prepare the DEC rr                   */
                    *pLen=ONE_BYTE;

                    retCheck=clean_r(str_op1);
                    
                    PUSHBIT(retCode, registerToBit(str_op1), 4);    /* Add the register as bits             */
                }

                if (!strcmp(str_op1, "(HL)"))                        /* Check if it is a DEC (HL) instruction       */
                {
                    retCode=CODE_DECHL;                             /* Prepare the DEC (HL)                         */
                    *pLen=ONE_BYTE;
                }

                if (!strcmp(str_op1, "IX"))                         /* Check if it is a DEC IX instruction      */
                {
                    retCode=CODE_DD_DECIX;                          /* Prepare the DEC IX                       */
                    *pLen=TWO_BYTES;
                }

                if (!strcmp(str_op1, "IY"))                         /* Check if it is a DEC IY instruction      */
                {
                    retCode=CODE_FD_DECIY;                          /* Prepare the DEC IY                       */
                    *pLen=TWO_BYTES;
                }

                /* Check if it is a DEC (IX+d) instruction    */
                if (((strlen(str_op1)==7) || (strlen(str_op1)==8)) && (strstr(str_op1, "IX") || strstr(str_op1, "IY")))    
                {
                    if (strstr(str_op1, "IX"))
                    {
                        retCode=CODE_DD_DECIXD;
                    }

                    if (strstr(str_op1, "IY"))
                    {
                        retCode=CODE_FD_DECIYD;
                    }

                    /* Clean the (IX+d) or (IY+d) for Op1 */
                    retCheck=clean_ixn(str_op1);

                    retCode=(retCode<<8)+toValue(str_op1+1, pLen, &lenEff);
                    *pLen=THREE_BYTES;
                    str_op1[0]='\0';
                }
            }

            if (!strcmp(str_inst, "AND"))                            /* A AND instruction is present  */
            {
                /* Check if it is a AND r instruction    */
                if (strlen(str_op1)==1)       
                {
                    retCode=CODE_ANDR;                              /* Prepare the AND r  */
                    *pLen=ONE_BYTE;

                    retCheck=clean_r(str_op1);
                    
                    PUSHBIT(retCode, registerToBit(str_op1), 0);    /* Add the register as bits   */
                }

                /* Check if it is a AND (IX+d) or AND (IY+d) instruction    */
                if (((strlen(str_op1)==7) || (strlen(str_op1)==8)) && (strstr(str_op1, "IX") || strstr(str_op1, "IY")))     
                {
                    if (strstr(str_op1, "IX"))
                    {
                        retCode=CODE_DD_ANDIXD;
                    }

                    if (strstr(str_op1, "IY"))
                    {
                        retCode=CODE_FD_ANDIYD;
                    }

                    /* Clean the (IX+d) or (IY+d) for Op1 */
                    retCheck=clean_ixn(str_op1);

                    retCode=(retCode<<8)+toValue(str_op1+1, pLen, &lenEff);
                    *pLen=THREE_BYTES;
                    str_op1[0]='\0';
                }

                /* Check if it is a AND n instruction    */
                if (((strlen(str_op1)==2) || (strlen(str_op1)==3)) && strchr(str_op1, '#'))       
                {
                    retCode=CODE_ANDN;                              /* Prepare the AND n  */
                    
                    retCheck=clean_n(str_op1);
                    retCode=(retCode << SIZE_1_BYTE)+toValue(str_op1+1, pLen, &lenEff);
                    *pLen=TWO_BYTES;
                }

                /* Check if it is a AND (HL) instruction    */
                if (!strcmp(str_op1, "(HL)"))       
                {
                    retCode=CODE_ANDHL;                              /* Prepare the AND (HL)  */
                    *pLen=ONE_BYTE;
                }
            }

            if (!strcmp(str_inst, "OR"))                            /* A OR instruction is present  */
            {
                /* Check if it is a OR r instruction    */
                if (strlen(str_op1)==1)       
                {
                    retCode=CODE_ORR;                              /* Prepare the OR r  */
                    *pLen=ONE_BYTE;

                    retCheck=clean_r(str_op1);
                    
                    PUSHBIT(retCode, registerToBit(str_op1), 0);    /* Add the register as bits   */
                }

                /* Check if it is a OR (IX+d) or OR (IY+d) instruction    */
                if (((strlen(str_op1)==7) || (strlen(str_op1)==8)) && (strstr(str_op1, "IX") || strstr(str_op1, "IY")))     
                {
                    if (strstr(str_op1, "IX"))
                    {
                        retCode=CODE_DD_ORIXD;
                    }

                    if (strstr(str_op1, "IY"))
                    {
                        retCode=CODE_FD_ORIYD;
                    }

                    /* Clean the (IX+d) or (IY+d) for Op1 */
                    retCheck=clean_ixn(str_op1);

                    retCode=(retCode<<8)+toValue(str_op1+1, pLen, &lenEff);
                    *pLen=THREE_BYTES;
                    str_op1[0]='\0';
                }

                /* Check if it is a AND n instruction    */
                if (((strlen(str_op1)==2) || (strlen(str_op1)==3)) && strchr(str_op1, '#'))       
                {
                    retCode=CODE_ORN;                              /* Prepare the OR n  */
                    
                    retCheck=clean_n(str_op1);
                    retCode=(retCode << SIZE_1_BYTE)+toValue(str_op1+1, pLen, &lenEff);
                    *pLen=TWO_BYTES;
                }

                /* Check if it is a AND (HL) instruction    */
                if (!strcmp(str_op1, "(HL)"))       
                {
                    retCode=CODE_ORHL;                              /* Prepare the OR (HL)  */
                    *pLen=ONE_BYTE;
                }
            }

            if (!strcmp(str_inst, "XOR"))                            /* A XOR instruction is present  */
            {
                /* Check if it is a XOR A,r instruction    */
                if (strlen(str_op1)==1)       
                {
                    retCode=CODE_XORR;                              /* Prepare the XOR r  */
                    *pLen=ONE_BYTE;

                    retCheck=clean_r(str_op1);
                    
                    PUSHBIT(retCode, registerToBit(str_op1), 0);    /* Add the register as bits   */
                }

                /* Check if it is a AND (IX+d) or AND (IY+d) instruction    */
                if (((strlen(str_op1)==7) || (strlen(str_op1)==8)) && (strstr(str_op1, "IX") || strstr(str_op1, "IY")))     
                {
                    if (strstr(str_op1, "IX"))
                    {
                        retCode=CODE_DD_XORIXD;
                    }

                    if (strstr(str_op1, "IY"))
                    {
                        retCode=CODE_FD_XORIYD;
                    }

                    /* Clean the (IX+d) or (IY+d) for Op1 */
                    retCheck=clean_ixn(str_op1);

                    retCode=(retCode<<8)+toValue(str_op1+1, pLen, &lenEff);
                    *pLen=THREE_BYTES;
                    str_op1[0]='\0';
                }

                /* Check if it is a AND n instruction    */
                if (((strlen(str_op1)==2) || (strlen(str_op1)==3)) && strchr(str_op1, '#'))       
                {
                    retCode=CODE_XORN;                              /* Prepare the XOR n  */
                    
                    retCheck=clean_n(str_op1);
                    retCode=(retCode << SIZE_1_BYTE)+toValue(str_op1+1, pLen, &lenEff);
                    *pLen=TWO_BYTES;
                }

                /* Check if it is a XOR (HL) instruction    */
                if (!strcmp(str_op1, "(HL)"))       
                {
                    retCode=CODE_XORHL;                              /* Prepare the XOR (HL)  */
                    *pLen=ONE_BYTE;
                }
            }

            if (!strcmp(str_inst, "CP"))                            /* A CP instruction is present  */
            {
                /* Check if it is a CP A,r instruction    */
                if (strlen(str_op1)==1)       
                {
                    retCode=CODE_CPR;                              /* Prepare the CP r  */
                    *pLen=ONE_BYTE;

                    retCheck=clean_r(str_op1);
                    
                    PUSHBIT(retCode, registerToBit(str_op1), 0);    /* Add the register as bits   */
                }

                /* Check if it is a CP (IX+d) or CP (IY+d) instruction    */
                if (((strlen(str_op1)==7) || (strlen(str_op1)==8)) && (strstr(str_op1, "IX") || strstr(str_op1, "IY")))     
                {
                    if (strstr(str_op1, "IX"))
                    {
                        retCode=CODE_DD_CPIXD;
                    }

                    if (strstr(str_op1, "IY"))
                    {
                        retCode=CODE_FD_CPIYD;
                    }

                    /* Clean the (IX+d) or (IY+d) for Op1 */
                    retCheck=clean_ixn(str_op1);

                    retCode=(retCode<<8)+toValue(str_op1+1, pLen, &lenEff);
                    *pLen=THREE_BYTES;
                    str_op1[0]='\0';
                }

                /* Check if it is a CP n instruction    */
                if (((strlen(str_op1)==2) || (strlen(str_op1)==3)) && strchr(str_op1, '#'))       
                {
                    retCode=CODE_CPN;                              /* Prepare the CP n  */
                    
                    retCheck=clean_n(str_op1);
                    retCode=(retCode << SIZE_1_BYTE)+toValue(str_op1+1, pLen, &lenEff);
                    *pLen=TWO_BYTES;
                }

                /* Check if it is a CP (HL) instruction    */
                if (!strcmp(str_op1, "(HL)"))       
                {
                    retCode=CODE_CPHL;                              /* Prepare the CP (HL)  */
                    *pLen=ONE_BYTE;
                }
            }

            if (!strcmp(str_inst, "POP"))                          /* A POP instruction is present         */
            {
                /* Check if it is a POP qq instruction   */
                if (strlen(str_op1)==2 && (!strcmp(str_op1, "BC") || !strcmp(str_op1, "DE") || !strcmp(str_op1, "HL") || !strcmp(str_op1, "AF")))                             
                {
                    retCode=CODE_POPQQ;                              /* Prepare the POP qq                    */
                    *pLen=ONE_BYTE;

                    retCheck=clean_rr(str_op1);

                    if (!strcmp(str_op1, "AF"))
                    {
                        strcpy(str_op1, "SP");
                    }

                    PUSHBIT(retCode, registerToBit(str_op1), 4);        /* Add the register as bits             */
                }

                /* Check if it is a POP IX instruction   */
                if (!strcmp(str_op1, "IX"))                             
                {
                    retCode=CODE_DD_POPIX;                              /* Prepare the POP IX                    */
                    *pLen=TWO_BYTES;
                }

                /* Check if it is a POP IY instruction   */
                if (!strcmp(str_op1, "IY"))                             
                {
                    retCode=CODE_FD_POPIY;                              /* Prepare the POP IY                    */
                    *pLen=TWO_BYTES;
                }
            }

            if (!strcmp(str_inst, "PUSH"))                          /* A PUSH instruction is present         */
            {
                /* Check if it is a PUSH qq instruction   */
                if (strlen(str_op1)==2 && (!strcmp(str_op1, "BC") || !strcmp(str_op1, "DE") || !strcmp(str_op1, "HL") || !strcmp(str_op1, "AF")))                             
                {
                    retCode=CODE_PUSHQQ;                              /* Prepare the PUSH qq                    */
                    *pLen=ONE_BYTE;

                    retCheck=clean_rr(str_op1);

                    if (!strcmp(str_op1, "AF"))
                    {
                        strcpy(str_op1, "SP");
                    }

                    PUSHBIT(retCode, registerToBit(str_op1), 4);        /* Add the register as bits             */
                }

                /* Check if it is a PUSH IX instruction   */
                if (!strcmp(str_op1, "IX"))                             
                {
                    retCode=CODE_DD_PUSHIX;                              /* Prepare the PUSH IX                    */
                    *pLen=TWO_BYTES;
                }

                /* Check if it is a PUSH IY instruction   */
                if (!strcmp(str_op1, "IY"))                             
                {
                    retCode=CODE_FD_PUSHIY;                              /* Prepare the PUSH IY                    */
                    *pLen=TWO_BYTES;
                }
            }

            if (!strcmp(str_inst, "RST"))                           /* A RST instruction is present         */
            {
                /* Check if it is a RST p instruction   */
                if (!strcmp(str_op1, "#00") || !strcmp(str_op1, "#08") || !strcmp(str_op1, "#10") || !strcmp(str_op1, "#18") || !strcmp(str_op1, "#20") || !strcmp(str_op1, "#28") || !strcmp(str_op1, "#30") || !strcmp(str_op1, "#38"))     
                {
                    retCode=CODE_RSTP;                              /* Prepare the RST p                    */
                    *pLen=ONE_BYTE;

                    /* Extract the p and push it    */
                    val=((str_op1[1]-'0')*0x10 + (str_op1[2]-'0'))/7;
                    PUSHBIT(retCode, val, 3);    /* Add the p-value as bits             */                
                }
            }

            if (!strcmp(str_inst, "CALL"))                              /* A CALL instruction is present                    */
            {
                *pLen=THREE_BYTES;                                      /* By default, a CALL is 3 bytes in label detection */

                /* Check if it is a CALL nn instruction   */
                if ((strlen(str_op1)>=4 && strlen(str_op1)<=7) && strchr(str_op1, '#'))
                {
                    retCode=CODE_CALLNN;                                /* Prepare the CALL nn                    */
                    
                    retCheck=clean_nn(str_op1);                         /* Clean the (nn) operand   */
                    word=toValue(str_op1+1, pLen, &lenEff);

                    retCode=(retCode<<SIZE_2_BYTES) + ((word & FIRST_LOWEST_BYTE) << SIZE_1_BYTE) + ((word & SECOND_LOWEST_BYTE)>>SIZE_1_BYTE);     /* Add the nn into the code */      

                    *pLen=THREE_BYTES;
                }
            }

            if (!strcmp(str_inst, "RET"))                              /* A RET instruction is present         */
            {
                /* Check if it is a RET cc instruction   */
                if ((strlen(str_op1)==1 || strlen(str_op1)==2))
                {
                    retCode=CODE_RETCC;                                 /* Prepare the RET cc                    */
                    
                    // To be done.
                    retCheck=clean_cc(str_op1);                         /* Clean the cc operand     */

                    PUSHBIT(retCode, conditionToBit(str_op1), 3);
                    *pLen=ONE_BYTE;
                }
            }

            if (!strcmp(str_inst, "JP"))                                /* A JP instruction is present                      */
            {
                *pLen=THREE_BYTES;                                      /* By default, a JP is 3 bytes in label detection */

                /* Check if it is a JP nn instruction   */
                if ((strlen(str_op1)>=2 && strlen(str_op1)<=7) && strchr(str_op1, '#'))
                {
                    retCode=CODE_JPNN;                                  /* Prepare the JP nn                 */
                    
                    retCheck=clean_nn(str_op1);                         /* Clean the (nn) operand   */
                    word=toValue(str_op1+1, pLen, &lenEff);

                    retCode=(retCode<<SIZE_2_BYTES) + ((word & FIRST_LOWEST_BYTE) << SIZE_1_BYTE) + ((word & SECOND_LOWEST_BYTE)>>SIZE_1_BYTE);     /* Add the nn into the code */      

                    *pLen=THREE_BYTES;
                }

                /* Check if it is a JP (HL) instruction   */
                if (!strcmp(str_op1, "(HL)"))
                {
                    retCode=CODE_JPHL;                                  /* Prepare the JP (HL)                 */
                    
                    *pLen=ONE_BYTE;
                }

                /* Check if it is a JP (IX) instruction   */
                if (!strcmp(str_op1, "(IX)"))
                {
                    retCode=CODE_DD_JPIX;                               /* Prepare the JP (IX)                 */
                    
                    *pLen=TWO_BYTES;
                }

                /* Check if it is a JP (IX) instruction   */
                if (!strcmp(str_op1, "(IY)"))
                {
                    retCode=CODE_FD_JPIY;                               /* Prepare the JP (IY)                 */
                    
                    *pLen=TWO_BYTES;
                }



            }

            if (!strcmp(str_inst, "JR"))                                /* A JR instruction is present          */
            {
                *pLen=TWO_BYTES;                                        /* By default, a JR is 2 bytes in label detection */
                
                /* Check if it is a JR e instruction   */
                if ((strlen(str_op1)>=3 && strlen(str_op1)<=5) && !strchr(str_op1, '#') && strchr(str_op1, '$') && (strchr(str_op1, '+') || strchr(str_op1, '-')))
                {
                    retCode=CODE_JRE;                                   /* Prepare the JR e                     */
                    
                    retCheck=clean_e(str_op1);                          /* Clean the e operand                  */

                    if (str_op1[1]=='+')
                    {
                        word=toDecValue(str_op1+2, pLen, &lenEff);
                    }
                    else
                    {
                        word=((~toDecValue(str_op1+2, pLen, &lenEff))+1) & FIRST_LOWEST_BYTE;
                    }

                    retCode=(retCode<<SIZE_1_BYTE) + ((word - 2) & FIRST_LOWEST_BYTE); /* Calc the (e-2) operand */      

                    *pLen=TWO_BYTES;
                }
            }

            if (!strcmp(str_inst, "DJNZ"))                                /* A DJNZ instruction is present          */
            {
                *pLen=TWO_BYTES;                                      /* By default, a DJNZ is 2 bytes in label detection */

                /* Check if it is a DJNZ e instruction   */
                if ((strlen(str_op1)>=3 && strlen(str_op1)<=5) && !strchr(str_op1, '#') && strchr(str_op1, '$') && (strchr(str_op1, '+') || strchr(str_op1, '-') ))
                {
                    retCode=CODE_DJNZ;                                   /* Prepare the JR e                     */
                    
                    retCheck=clean_e(str_op1);                          /* Clean the e operand                  */

                    if (str_op1[1]=='+')
                    {
                        word=toDecValue(str_op1+2, pLen, &lenEff);
                    }
                    else
                    {
                        word=((~toDecValue(str_op1+2, pLen, &lenEff))+1) & FIRST_LOWEST_BYTE;
                    }

                    retCode=(retCode<<SIZE_1_BYTE) + ((word -2) & FIRST_LOWEST_BYTE); /* Calc the (e-2) operand */      

                    *pLen=TWO_BYTES;
                }
            }
            break;    
    
        case 3:
            if (!strcmp(str_inst, "LD"))                            /* A LD instruction is present  */
            {
                /* Check if it is a LD, r,r' instruction but not LD with R or I register    */
                if (strlen(str_op1)==1 && strlen(str_op2)==1 && strcmp(str_op1, "I") && strcmp(str_op1, "R")&& strcmp(str_op2, "I") && strcmp(str_op2, "R"))       
                {
                    retCode=CODE_LDRR;                              /* Prepare the LD r,r'  */
                    *pLen=ONE_BYTE;

                    retCheck=clean_r(str_op1);
                    retCheck=clean_r(str_op2);
                    
                    PUSHBIT(retCode, registerToBit(str_op1), 3);    /* Add the first register as bits   */
                    PUSHBIT(retCode, registerToBit(str_op2), 0);    /* Add the first register as bits   */
                }

                /* Check if it is a LD r,n instruction    */
                if (strlen(str_op1)==1 && ((strlen(str_op2)==2) || (strlen(str_op2)==3)) && strchr(str_op2, '#'))       
                {
                    /* Clean the n for Op2 and r for Op1 */
                    retCheck=clean_r(str_op1);
                    retCheck=clean_n(str_op2);

                    retCode=CODE_LDRN;
                    
                    PUSHBIT(retCode, registerToBit(str_op1), 3);                          /* Add the first register as bits   */

                    retCode=(retCode<<SIZE_1_BYTE)+toValue(str_op2+1, pLen, &lenEff);     /* Prepare the LD r,n   */
                    *pLen=TWO_BYTES;
                }

                /* Check if it is a LD rr,nn, LD IX,nn or a LD IY,nn instruction    */
                if (strlen(str_op1)==2 && ((strlen(str_op2)==2) || (strlen(str_op2)==3)|| (strlen(str_op2)==4)|| (strlen(str_op2)==5)) && !strchr(str_op2, '(') && !strchr(str_op2, ')'))       
                {
                    /* Clean the n for Op2 and r for Op1 */
                    retCheck=clean_nn(str_op2);

                    word=toValue(str_op2+1, pLen, &lenEff);

                    if (!strcmp(str_op1, "IX"))
                    {
                        retCode=CODE_DD_LDIXNN;
                        *pLen=FOUR_BYTES;
                    }

                    if (!strcmp(str_op1, "IY"))
                    {
                        retCode=CODE_FD_LDIYNN;
                        *pLen=FOUR_BYTES;
                    }

                    if ((!strcmp(str_op1, "BC") || !strcmp(str_op1, "DE") || !strcmp(str_op1, "HL") || !strcmp(str_op1, "SP")) && strchr(str_op2, '#'))
                    {
                        retCode=CODE_LDRRNN;

                        PUSHBIT(retCode, registerToBit(str_op1), 4);                /* Add the first register as bits   */
                        *pLen=THREE_BYTES;
                    }

                    retCode=(retCode<<SIZE_2_BYTES) + ((word & FIRST_LOWEST_BYTE) << SIZE_1_BYTE) + ((word & SECOND_LOWEST_BYTE)>>SIZE_1_BYTE);     /* Prepare the LD rr,nn   */              
                }

                /* Check if it is a LD SP,HL    */
                if (!strcmp(str_op1, "SP") && !strcmp(str_op2, "HL"))
                {
                    retCode=CODE_LDSPHL;
                    *pLen=ONE_BYTE;
                }

                /* Check if it is a LD SP,IX    */
                if (!strcmp(str_op1, "SP") && !strcmp(str_op2, "IX"))
                {
                    retCode=CODE_DD_LDSPIX;
                    *pLen=TWO_BYTES;
                }

                /* Check if it is a LD SP,IY    */
                if (!strcmp(str_op1, "SP") && !strcmp(str_op2, "IY"))
                {
                    retCode=CODE_FD_LDSPIY;
                    *pLen=TWO_BYTES;
                }

                /* Check if it is a LD A,I    */
                if (!strcmp(str_op1, "A") && !strcmp(str_op2, "I"))
                {
                    retCode=CODE_ED_LDAI;
                    *pLen=TWO_BYTES;
                }

                /* Check if it is a LD A,R    */
                if (!strcmp(str_op1, "A") && !strcmp(str_op2, "R"))
                {
                    retCode=CODE_ED_LDAR;
                    *pLen=TWO_BYTES;
                }

                /* Check if it is a LD I,A    */
                if (!strcmp(str_op1, "I") && !strcmp(str_op2, "A"))
                {
                    retCode=CODE_ED_LDIA;
                    *pLen=TWO_BYTES;
                }

                /* Check if it is a LD R,A    */
                if (!strcmp(str_op1, "R") && !strcmp(str_op2, "A"))
                {
                    retCode=CODE_ED_LDRA;
                    *pLen=TWO_BYTES;
                }

                /* Check if it is a LD rr,(nn), LD IX,(nn) or a LD IY,(nn) instruction    */
                if (strlen(str_op1)==2 && (strlen(str_op2)>=4 && strlen(str_op2)<=7) && strchr(str_op2, '(') && strchr(str_op2, ')') && strchr(str_op2, '#'))
                {
                    /* Clean the n for Op2 and r for Op1 */
                    retCheck=clean_rr(str_op1);
                    retCheck=clean_inn(str_op2);                /* Clean the (nn) operand ande remove '(' and ')'  */
                    word=toValue(str_op2+1, pLen, &lenEff);

                    if (!strcmp(str_op1, "IX"))
                    {
                        retCode=CODE_DD_LDIXANN;
                        *pLen=FOUR_BYTES;
                    }

                    if (!strcmp(str_op1, "IY"))
                    {
                        retCode=CODE_FD_LDIYANN;
                        *pLen=FOUR_BYTES;
                    }

                    if (!strcmp(str_op1, "BC") || !strcmp(str_op1, "DE") || !strcmp(str_op1, "HL") || !strcmp(str_op1, "SP"))
                    {
                        retCode=CODE_ED_LDDDNN;
                        PUSHBIT(retCode, registerToBit(str_op1), 4);                /* Add the first register as bits   */
                        *pLen=FOUR_BYTES;
                    }

                    retCode=(retCode<<SIZE_2_BYTES) + ((word & FIRST_LOWEST_BYTE) << SIZE_1_BYTE) + ((word & SECOND_LOWEST_BYTE)>>SIZE_1_BYTE);     /* Prepare the LD rr,nn   */              
                }

                /* Check if it is a LD r,(IX+d) instruction    */
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

                /* Check if it is a LD (IX+d),r or LD (IY+d),r instruction   */
                if (strlen(str_op2)==1 && ((strlen(str_op1)==7) || (strlen(str_op1)==8)) && (strstr(str_op1, "IX") || strstr(str_op1, "IY")))       
                {
                    /* Clean the  r for Op2 */
                    retCheck=clean_r(str_op2);

                    if (strstr(str_op1, "IX"))
                    {
                        retCode=CODE_DD_LDIXDR;
                    }

                    if (strstr(str_op1, "IY"))
                    {
                        retCode=CODE_FD_LDIYDR;
                    }


                    /* Clean the (IX+d) or (IY+d) for Op1 */
                    retCheck=clean_ixn(str_op1);

                    PUSHBIT(retCode, registerToBit(str_op2), 0);                /* Add the register as bits   */
                    retCode=(retCode<<8)+toValue(str_op1+1, pLen, &lenEff);     /* Prepare the LD r,(IX+d) or the LD r,(IY+d)  */

                    *pLen=THREE_BYTES;
                }

                /* Check if it is a LD (IX+d),n or LD (IY+d),n instruction   */
                if (((strlen(str_op2)==2) || (strlen(str_op2)==3)) && strchr(str_op2, '#') && ((strlen(str_op1)==7) || (strlen(str_op1)==8)) && (strstr(str_op1, "IX") || strstr(str_op1, "IY")))       
                {
                    /* Clean the  n for Op2 */
                    retCheck=clean_n(str_op2);

                    if (strstr(str_op1, "IX"))
                    {
                        retCode=CODE_DD_LDIXDN;
                    }

                    if (strstr(str_op1, "IY"))
                    {
                        retCode=CODE_FD_LDIYDN;
                    }

                    /* Clean the (IX+d) or (IY+d) for Op1 */
                    retCheck=clean_ixn(str_op1);

                    //PUSHBIT(retCode, registerToBit(str_op2), 0);                /* Add the register as bits   */
                    retCode=(retCode<<8)+toValue(str_op1+1, pLen, &lenEff);
                    retCode=(retCode<<8)+toValue(str_op2+1, pLen, &lenEff);     /* Prepare the LD r,(IX+d) or the LD r,(IY+d)  */

                    *pLen=FOUR_BYTES;
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

                /* Check if it is a LD (nn),rr, LD (nn),IX or LD (nn),IY instruction    */
                if (strlen(str_op2)==2 && strchr(str_op1, '#') && strchr(str_op1, '(') && strchr(str_op1, ')') && strlen(str_op1)>=4 && strlen(str_op1)<=7 )    
                {
                    if (!strcmp(str_op2, "IX"))
                    {
                        retCode=CODE_DD_LDNNIX;
                    }

                    if (!strcmp(str_op2, "IY"))
                    {
                        retCode=CODE_FD_LDNNIY;
                    }

                    if (!strcmp(str_op2, "BC") || !strcmp(str_op2, "DE") || !strcmp(str_op2, "HL") || !strcmp(str_op2, "SP"))
                    {
                        retCode=CODE_ED_LDNNRR;
                        PUSHBIT(retCode, registerToBit(str_op2), 4);                /* Add the first register as bits   */
                    }

                    retCheck=clean_inn(str_op1);                /* Clean the (nn) operand ande remove '(' and ')'  */
                    word=toValue(str_op1+1, pLen, &lenEff);

                    retCode=(retCode<<SIZE_2_BYTES) + ((word & FIRST_LOWEST_BYTE) << SIZE_1_BYTE) + ((word & SECOND_LOWEST_BYTE)>>SIZE_1_BYTE);

                    *pLen=FOUR_BYTES;
                }

                /* Check if it is a LD (HL),n instruction    */
                if (!strcmp(str_op1,"(HL)") && ((strlen(str_op2)==2) || (strlen(str_op2)==3)) && strchr(str_op2, '#'))       
                {
                    retCode=CODE_LDHLN;
                    /* Clean the n for Op2 */
                    retCheck=clean_n(str_op2);

                    retCode=(retCode<<SIZE_1_BYTE)+toValue(str_op2+1, pLen, &lenEff);
                    *pLen=TWO_BYTES;
                }

                /* Check if it is a LD (HL),r instruction    */
                if (!strcmp(str_op1,"(HL)") && (strlen(str_op2)==1))       
                {
                    retCode=CODE_LDHLR;
                    /* Clean the n for Op2 */
                    retCheck=clean_r(str_op2);

                    PUSHBIT(retCode, registerToBit(str_op2), 0);
                    *pLen=ONE_BYTE;
                }
            }

            if (!strcmp(str_inst, "ADD"))                            /* A ADD instruction is present  */
            {
                /* Check if it is a ADD A,r instruction    */
                if (!strcmp(str_op1, "A") && (strlen(str_op2)==1))       
                {
                    retCode=CODE_ADDAR;                              /* Prepare the ADD r,r'  */
                    *pLen=ONE_BYTE;

                    retCheck=clean_r(str_op1);
                    retCheck=clean_r(str_op2);
                    
                    PUSHBIT(retCode, registerToBit(str_op2), 0);    /* Add the second register as bits   */
                }

                /* Check if it is a ADD A,(IX+d) instruction    */
                if (!strcmp(str_op1, "A") && ((strlen(str_op2)==7) || (strlen(str_op2)==8)) && (strstr(str_op2, "IX") || strstr(str_op2, "IY")))       
                {
                    if (strstr(str_op2, "IX"))
                    {
                        retCode=CODE_DD_ADDAIXD;
                    }

                    if (strstr(str_op2, "IY"))
                    {
                        retCode=CODE_FD_ADDAIYD;
                    }

                    /* Clean the (IX+d) or (IY+d) for Op1 */
                    retCheck=clean_ixn(str_op2);

                    retCode=(retCode<<8)+toValue(str_op2+1, pLen, &lenEff);
                    *pLen=THREE_BYTES;
                    str_op2[0]='\0';
                }

                /* Check if it is a ADD A,n instruction    */
                if (!strcmp(str_op1, "A") && ((strlen(str_op2)==2) || (strlen(str_op2)==3)) && strchr(str_op2, '#'))       
                {
                    retCode=CODE_ADDAN;                              /* Prepare the ADD A,n  */
                    
                    retCheck=clean_n(str_op2);
                    retCode=(retCode << SIZE_1_BYTE)+toValue(str_op2+1, pLen, &lenEff);
                    //PUSHBIT(retCode, registerToBit(str_op2), 0);    /* Add the n    */
                    *pLen=TWO_BYTES;
                }

                /* Check if it is a ADD A,(HL) instruction    */
                if (!strcmp(str_op1, "A") && !strcmp(str_op2, "(HL)"))       
                {
                    retCode=CODE_ADDAHL;                              /* Prepare the ADD A,(HL)  */
                    *pLen=ONE_BYTE;
                }

                /* Check if it is a ADD HL,rr instruction    */
                if (!strcmp(str_op1, "HL") && strlen(str_op2)==2 && !strchr(str_op2, '#'))       
                {
                    retCode=CODE_ADDHLRR;                                       /* Prepare the ADD HL,rr  */
                    PUSHBIT(retCode, registerToBit(str_op2), 4);                /* Add the first register as bits   */
                    *pLen=ONE_BYTE;
                }

                /* Check if it is a ADD IX,pp instruction    */
                if (!strcmp(str_op1, "IX") && strlen(str_op2)==2 && !strchr(str_op2, '#'))       
                {
                    retCode=CODE_DD_ADDIXPP;     
                                                                                /* Prepare the ADD IY,qq            */
                    PUSHBIT(retCode, registerToBit(str_op2), 4);                /* Add the first register as bits   */
                    *pLen=TWO_BYTES;
                }

                /* Check if it is a ADD IY,qq instruction    */
                if (!strcmp(str_op1, "IY") && strlen(str_op2)==2 && !strchr(str_op2, '#'))       
                {
                    retCode=CODE_FD_ADDIYQQ;     
                                                                                /* Prepare the ADD IY,qq            */
                    PUSHBIT(retCode, registerToBit(str_op2), 4);                /* Add the first register as bits   */
                    *pLen=TWO_BYTES;
                }
            }

            if (!strcmp(str_inst, "ADC"))                            /* A ADC instruction is present  */
            {
                /* Check if it is a ADC A,r' instruction    */
                if (!strcmp(str_op1, "A") && strlen(str_op2)==1)       
                {
                    retCode=CODE_ADCAR;                              /* Prepare the ADC A,r  */
                    *pLen=ONE_BYTE;

                    retCheck=clean_r(str_op2);
                    
                    PUSHBIT(retCode, registerToBit(str_op2), 0);    /* Add the second register as bits   */
                }

                /* Check if it is a ADC A,(HL) instruction    */
                if (!strcmp(str_op1, "A") && !strcmp(str_op2, "(HL)"))       
                {
                    retCode=CODE_ADCAHL;                              /* Prepare the ADC r,(HL)  */
                    *pLen=ONE_BYTE;
                }

                /* Check if it is a ADC A,(IX+d) instruction    */
                if (!strcmp(str_op1, "A") && ((strlen(str_op2)==7) || (strlen(str_op2)==8)) && (strstr(str_op2, "IX") || strstr(str_op2, "IY")))       
                {
                    if (strstr(str_op2, "IX"))
                    {
                        retCode=CODE_DD_ADCAIXD;
                    }

                    if (strstr(str_op2, "IY"))
                    {
                        retCode=CODE_FD_ADCAIYD;
                    }

                    /* Clean the (IX+d) or (IY+d) for Op1 */
                    retCheck=clean_ixn(str_op2);

                    retCode=(retCode<<8)+toValue(str_op2+1, pLen, &lenEff);
                    *pLen=THREE_BYTES;
                    str_op2[0]='\0';
                }

                /* Check if it is a ADC A,n instruction    */
                if (!strcmp(str_op1, "A") && ((strlen(str_op2)==2) || (strlen(str_op2)==3)) && strchr(str_op2, '#'))       
                {
                    retCode=CODE_ADCAN;                              /* Prepare the ADD A,n  */
                    
                    retCheck=clean_n(str_op2);
                    retCode=(retCode << SIZE_1_BYTE)+toValue(str_op2+1, pLen, &lenEff);
                    *pLen=TWO_BYTES;
                }

                /* Check if it is a ADC HL,rr instruction    */
                if (!strcmp(str_op1, "HL") && strlen(str_op2)==2 && !strchr(str_op2, '#'))       
                {
                    retCode=CODE_ED_ADCHLRR;                            /* Prepare the ADC HL,rr  */
                    PUSHBIT(retCode, registerToBit(str_op2), 4);        /* Add the first register as bits   */
                    *pLen=TWO_BYTES;
                }
            }

            if (!strcmp(str_inst, "SUB"))                            /* A SUB instruction is present  */
            {
                /* Check if it is a SUB A,r instruction    */
                if (!strcmp(str_op1, "A") && (strlen(str_op2)==1))       
                {
                    retCode=CODE_SUBAR;                              /* Prepare the SUB r,r'  */
                    *pLen=ONE_BYTE;

                    retCheck=clean_r(str_op1);
                    retCheck=clean_r(str_op2);
                    
                    PUSHBIT(retCode, registerToBit(str_op2), 0);    /* Add the second register as bits   */
                }

                /* Check if it is a SUB A,(IX+d) instruction    */
                if (!strcmp(str_op1, "A") && ((strlen(str_op2)==7) || (strlen(str_op2)==8)) && (strstr(str_op2, "IX") || strstr(str_op2, "IY")))       
                {
                    if (strstr(str_op2, "IX"))
                    {
                        retCode=CODE_DD_SUBAIXD;
                    }

                    if (strstr(str_op2, "IY"))
                    {
                        retCode=CODE_FD_SUBAIYD;
                    }

                    /* Clean the (IX+d) or (IY+d) for Op1 */
                    retCheck=clean_ixn(str_op2);

                    retCode=(retCode<<8)+toValue(str_op2+1, pLen, &lenEff);
                    *pLen=THREE_BYTES;
                    str_op2[0]='\0';
                }

                /* Check if it is a SUB A,n instruction    */
                if (!strcmp(str_op1, "A") && ((strlen(str_op2)==2) || (strlen(str_op2)==3)) && strchr(str_op2, '#'))       
                {
                    retCode=CODE_SUBAN;                              /* Prepare the SUB A,n  */
                    
                    retCheck=clean_n(str_op2);
                    retCode=(retCode << SIZE_1_BYTE)+toValue(str_op2+1, pLen, &lenEff);
                    //PUSHBIT(retCode, registerToBit(str_op2), 0);    /* Add the n    */
                    *pLen=TWO_BYTES;
                }

                /* Check if it is a SUB A,(HL) instruction    */
                if (!strcmp(str_op1, "A") && !strcmp(str_op2, "(HL)"))       
                {
                    retCode=CODE_SUBAHL;                              /* Prepare the SUB A,(HL)  */
                    *pLen=ONE_BYTE;
                } 
            }

            if (!strcmp(str_inst, "SBC"))                            /* A SBC instruction is present  */
            {
                /* Check if it is a SBC A,r instruction    */
                if (!strcmp(str_op1, "A") && (strlen(str_op2)==1))       
                {
                    retCode=CODE_SBCAR;                              /* Prepare the SBC r,r'  */
                    *pLen=ONE_BYTE;

                    retCheck=clean_r(str_op1);
                    retCheck=clean_r(str_op2);
                    
                    PUSHBIT(retCode, registerToBit(str_op2), 0);    /* Add the second register as bits   */
                }

                /* Check if it is a SBC A,(IX+d) instruction    */
                if (!strcmp(str_op1, "A") && ((strlen(str_op2)==7) || (strlen(str_op2)==8)) && (strstr(str_op2, "IX") || strstr(str_op2, "IY")))       
                {
                    if (strstr(str_op2, "IX"))
                    {
                        retCode=CODE_DD_SBCAIXD;
                    }

                    if (strstr(str_op2, "IY"))
                    {
                        retCode=CODE_FD_SBCAIYD;
                    }

                    /* Clean the (IX+d) or (IY+d) for Op1 */
                    retCheck=clean_ixn(str_op2);

                    retCode=(retCode<<8)+toValue(str_op2+1, pLen, &lenEff);
                    *pLen=THREE_BYTES;
                    str_op2[0]='\0';
                }

                /* Check if it is a SBC A,n instruction    */
                if (!strcmp(str_op1, "A") && ((strlen(str_op2)==2) || (strlen(str_op2)==3)) && strchr(str_op2, '#'))       
                {
                    retCode=CODE_SBCAN;                              /* Prepare the SBC A,n  */
                    
                    retCheck=clean_n(str_op2);
                    retCode=(retCode << SIZE_1_BYTE)+toValue(str_op2+1, pLen, &lenEff);
                    *pLen=TWO_BYTES;
                }

                /* Check if it is a SBC A,(HL) instruction    */
                if (!strcmp(str_op1, "A") && !strcmp(str_op2, "(HL)"))       
                {
                    retCode=CODE_SBCAHL;                              /* Prepare the SBC A,(HL)  */
                    *pLen=ONE_BYTE;
                }

                /* Check if it is a SBC HL,rr instruction    */
                if (!strcmp(str_op1, "HL") && strlen(str_op2)==2 && !strchr(str_op2, '#'))       
                {
                    retCode=CODE_ED_SBCHLRR;                            /* Prepare the SBC HL,rr  */
                    PUSHBIT(retCode, registerToBit(str_op2), 4);        /* Add the first register as bits   */
                    *pLen=TWO_BYTES;
                }
            }

            if (!strcmp(str_inst, "EX"))                                        /* A EX is present     */
            {
                if (!strcmp(str_op1, "AF") && !strcmp(str_op2,"AF'"))           /* A EX AF,AF' is present     */
                {
                    retCode=CODE_EXAFAF;
                    *pLen=ONE_BYTE;
                }

                if (!strcmp(str_op1, "(SP)") && !strcmp(str_op2,"HL"))           /* A EX (SP),HL is present     */
                {
                    retCode=CODE_EXSPHL;
                    *pLen=ONE_BYTE;
                }

                if (!strcmp(str_op1, "(SP)") && !strcmp(str_op2,"IX"))           /* A EX (SP),IX is present     */
                {
                    retCode=CODE_DD_EXSPIX;
                    *pLen=TWO_BYTES;
                }

                if (!strcmp(str_op1, "(SP)") && !strcmp(str_op2,"IY"))           /* A EX (SP),IY is present     */
                {
                    retCode=CODE_FD_EXSPIY;
                    *pLen=TWO_BYTES;
                }

                if (!strcmp(str_op1, "DE") && !strcmp(str_op2,"HL"))           /* A EX DE,HL is present     */
                {
                    retCode=CODE_EXDEHL;
                    *pLen=ONE_BYTE;
                }
            }
           
            if (!strcmp(str_inst, "BIT"))                               /* A BIT instruction is present  */
            {
                /* Check if it is a BIT b,r instruction    */
                if (strlen(str_op1)==1 && strlen(str_op2)==1)       
                {
                    retCode=CODE_CB_BITBR;                              /* Prepare the BIT n,r  */
                    //printf("rc=%s reg=%d bit=%d\n", byteToBinary(bb), registerToBit(str_op2), numberToBit(str_op1));
                    PUSHBIT(retCode, registerToBit(str_op2), 0);        /* Add the register as bits     */
                    PUSHBIT(retCode, numberToBit(str_op1), 3);          /* Add the bit number as bits   */

                    *pLen=TWO_BYTES;
                }

                /* Check if it is a BIT b,(HL) instruction    */
                if (strlen(str_op1)==1 && !strcmp(str_op2, "(HL)"))       
                {
                    retCode=CODE_CB_BITBHL;                               /* Prepare the BIT n,(HL) */
                    //PUSHBIT(retCode, registerToBit(str_op2), 0);        /* Add the register as bits     */
                    PUSHBIT(retCode, numberToBit(str_op1), 3);            /* Add the bit number as bits   */
                    //
                    *pLen=TWO_BYTES;
                }

                /* Check if it is a BIT b,(IX+d) or BIT b,(IY+d) instruction  */
                if ((strlen(str_op1)==1) && (((strlen(str_op2)==7) || (strlen(str_op2)==8)) && (strstr(str_op2, "IX") || strstr(str_op2, "IY")) && strchr(str_op2, '(') && strchr(str_op2, ')') && strchr(str_op2, '+')))
                {
                    if (strstr(str_op2, "IX"))
                    {
                        retCode=CODE_DDCB_BITBIXD;
                    }

                    if (strstr(str_op2, "IY"))
                    {
                        retCode=CODE_FDCB_BITBIYD;
                    }

                    /* Clean the (IX+d) or (IY+d) for Op1 */
                    retCheck=clean_ixn(str_op2);
                    
                    retCode=retCode+(toValue(str_op2+1, pLen, &lenEff)<<SIZE_1_BYTE);         /* Prepare the BIT b,(IX+d) or the RLC (IY+d)  */
                    PUSHBIT(retCode, toValue(str_op1, pLen, &lenEff), 3);

                    *pLen=FOUR_BYTES;
                }
            }

            if (!strcmp(str_inst, "RES"))                               /* A RES instruction is present  */
            {
                /* Check if it is a RES n,r instruction    */
                if (strlen(str_op1)==1 && strlen(str_op2)==1)       
                {
                    retCode=CODE_CB_RESBR;                              /* Prepare the RES n,r  */
                    PUSHBIT(retCode, registerToBit(str_op2), 0);        /* Add the register as bits     */
                    PUSHBIT(retCode, numberToBit(str_op1), 3);          /* Add the bit number as bits   */

                    *pLen=TWO_BYTES;
                }

                /* Check if it is a RES n,(HL) instruction    */
                if (strlen(str_op1)==1 && !strcmp(str_op2, "(HL)"))       
                {
                    retCode=CODE_CB_RESBHL;                               /* Prepare the RES n,(HL) */
                    PUSHBIT(retCode, numberToBit(str_op1), 3);            /* Add the bit number as bits   */
                    
                    *pLen=TWO_BYTES;
                }

                /* Check if it is a RES b,(IX+d) or RES b,(IY+d) instruction  */
                if ((strlen(str_op1)==1) && (((strlen(str_op2)==7) || (strlen(str_op2)==8)) && (strstr(str_op2, "IX") || strstr(str_op2, "IY")) && strchr(str_op2, '(') && strchr(str_op2, ')') && strchr(str_op2, '+')))
                {
                    if (strstr(str_op2, "IX"))
                    {
                        retCode=CODE_DDCB_RESBIXD;
                    }

                    if (strstr(str_op2, "IY"))
                    {
                        retCode=CODE_FDCB_RESBIYD;
                    }

                    /* Clean the (IX+d) or (IY+d) for Op1 */
                    retCheck=clean_ixn(str_op2);
                    
                    retCode=retCode+(toValue(str_op2+1, pLen, &lenEff)<<SIZE_1_BYTE);         /* Prepare the RES b,(IX+d) or the RLC (IY+d)  */
                    PUSHBIT(retCode, toValue(str_op1, pLen, &lenEff), 3);

                    *pLen=FOUR_BYTES;
                }
            }

            if (!strcmp(str_inst, "SET"))                               /* A SET instruction is present  */
            {
                /* Check if it is a SET n,r instruction    */
                if (strlen(str_op1)==1 && strlen(str_op2)==1)       
                {
                    retCode=CODE_CB_SETBR;                              /* Prepare the SET n,r  */
                    PUSHBIT(retCode, registerToBit(str_op2), 0);        /* Add the register as bits     */
                    PUSHBIT(retCode, numberToBit(str_op1), 3);          /* Add the bit number as bits   */

                    *pLen=TWO_BYTES;
                }

                /* Check if it is a SET n,(HL) instruction    */
                if (strlen(str_op1)==1 && !strcmp(str_op2, "(HL)"))       
                {
                    retCode=CODE_CB_SETBHL;                               /* Prepare the SET n,(HL) */
                    PUSHBIT(retCode, numberToBit(str_op1), 3);            /* Add the bit number as bits   */
                    
                    *pLen=TWO_BYTES;
                }

                /* Check if it is a SET b,(IX+d) or SET b,(IY+d) instruction  */
                if ((strlen(str_op1)==1) && (((strlen(str_op2)==7) || (strlen(str_op2)==8)) && (strstr(str_op2, "IX") || strstr(str_op2, "IY")) && strchr(str_op2, '(') && strchr(str_op2, ')') && strchr(str_op2, '+')))
                {
                    if (strstr(str_op2, "IX"))
                    {
                        retCode=CODE_DDCB_SETBIXD;
                    }

                    if (strstr(str_op2, "IY"))
                    {
                        retCode=CODE_FDCB_SETBIYD;
                    }

                    /* Clean the (IX+d) or (IY+d) for Op1 */
                    retCheck=clean_ixn(str_op2);
                    
                    retCode=retCode+(toValue(str_op2+1, pLen, &lenEff)<<SIZE_1_BYTE);         /* Prepare the SET b,(IX+d) or the RLC (IY+d)  */
                    PUSHBIT(retCode, toValue(str_op1, pLen, &lenEff), 3);

                    *pLen=FOUR_BYTES;
                }
            }

            if (!strcmp(str_inst, "CALL"))                              /* A CALL instruction is present         */
            {
                /* Check if it is a CALL cc,nn instruction   */
                if ((strlen(str_op1)==1 || strlen(str_op1)==2) && (strlen(str_op2)>=4 && strlen(str_op2)<=7) && strchr(str_op2, '#'))
                {
                    retCode=CODE_CALLCCNN;                              /* Prepare the CALL cc,nn                    */
                    
                    // To be done.
                    retCheck=clean_cc(str_op1);                         /* Clean the cc operand     */
                    retCheck=clean_nn(str_op2);                         /* Clean the (nn) operand   */
                    word=toValue(str_op2+1, pLen, &lenEff);

                    PUSHBIT(retCode, conditionToBit(str_op1), 3);
                    retCode=(retCode<<SIZE_2_BYTES) + ((word & FIRST_LOWEST_BYTE) << SIZE_1_BYTE) + ((word & SECOND_LOWEST_BYTE)>>SIZE_1_BYTE);     /* Add the nn into the code */      
                    *pLen=THREE_BYTES;
                }
            }

            if (!strcmp(str_inst, "JP"))                                /* A JP instruction is present                      */
            {
                *pLen=THREE_BYTES;                                      /* By default, a JP is 3 bytes in label detection */

                if ((strlen(str_op1)==1 || strlen(str_op1)==2) && (strlen(str_op2)>=4 && strlen(str_op2)<=7) && strchr(str_op2, '#'))
                {
                    retCode=CODE_JPCCNN;                                /* Prepare the JP cc,nn                    */
                    
                    // To be done.
                    retCheck=clean_cc(str_op1);                         /* Clean the cc operand     */
                    retCheck=clean_nn(str_op2);                         /* Clean the (nn) operand   */
                    word=toValue(str_op2+1, pLen, &lenEff);

                    PUSHBIT(retCode, conditionToBit(str_op1), 3);
                    retCode=(retCode<<SIZE_2_BYTES) + ((word & FIRST_LOWEST_BYTE) << SIZE_1_BYTE) + ((word & SECOND_LOWEST_BYTE)>>SIZE_1_BYTE);     /* Add the nn into the code */      
                    *pLen=THREE_BYTES;
                }
            }

            if (!strcmp(str_inst, "JR"))                                /* A JR instruction is present                      */
            {
                *pLen=TWO_BYTES;                                      /* By default, a JR is 3 bytes in label detection */

                if (!strcmp(str_op1, "C") && (strlen(str_op2)>=3 && strlen(str_op2)<=5) && !strchr(str_op2, '#') && strchr(str_op2, '$') && (strchr(str_op2, '+') || strchr(str_op2, '-')))
                {
                    retCode=CODE_JRCE;                                  /* Prepare the JR C,e                   */
                    
                    retCheck=clean_cc(str_op1);                         /* Clean the cc operand                 */
                    retCheck=clean_e(str_op2);                          /* Clean the e operand                  */

                    if (str_op2[1]=='+')
                    {
                        word=toDecValue(str_op2+2, pLen, &lenEff);
                    }
                    else
                    {
                        word=((~toDecValue(str_op2+2, pLen, &lenEff))+1) & FIRST_LOWEST_BYTE;
                    }

                    retCode=(retCode<<SIZE_1_BYTE) + ((word - 2) & FIRST_LOWEST_BYTE); /* Calc the (e-2) operand */     

                    *pLen=TWO_BYTES;
                }

                if (!strcmp(str_op1, "NC") && (strlen(str_op2)>=3 && strlen(str_op2)<=5) && !strchr(str_op2, '#') && strchr(str_op2, '$') && (strchr(str_op2, '+') || strchr(str_op2, '-')))
                {
                    retCode=CODE_JRNCE;                                  /* Prepare the JR NC,e                 */
                    
                    retCheck=clean_cc(str_op1);                         /* Clean the cc operand                 */
                    retCheck=clean_e(str_op2);                          /* Clean the e operand                  */

                    if (str_op2[1]=='+')
                    {
                        word=toDecValue(str_op2+2, pLen, &lenEff);
                    }
                    else
                    {
                        word=((~toDecValue(str_op2+2, pLen, &lenEff))+1) & FIRST_LOWEST_BYTE;
                    }

                    retCode=(retCode<<SIZE_1_BYTE) + ((word - 2) & FIRST_LOWEST_BYTE); /* Calc the (e-2) operand */  

                    *pLen=TWO_BYTES;
                }

                if (!strcmp(str_op1, "Z") && (strlen(str_op2)>=3 && strlen(str_op2)<=5) && !strchr(str_op2, '#') && strchr(str_op2, '$') && (strchr(str_op2, '+') || strchr(str_op2, '-')))
                {
                    retCode=CODE_JRZE;                                  /* Prepare the JR Z,e                   */

                    retCheck=clean_cc(str_op1);                         /* Clean the cc operand                 */
                    retCheck=clean_e(str_op2);                          /* Clean the e operand                  */

                    if (str_op2[1]=='+')
                    {
                        word=toDecValue(str_op2+2, pLen, &lenEff);
                    }
                    else
                    {
                        word=((~toDecValue(str_op2+2, pLen, &lenEff))+1) & FIRST_LOWEST_BYTE;
                    }

                    retCode=(retCode<<SIZE_1_BYTE) + ((word - 2) & FIRST_LOWEST_BYTE);   /* Calc the (e-2) operand */  

                    *pLen=TWO_BYTES;
                }

                if (!strcmp(str_op1, "NZ") && (strlen(str_op2)>=3 && strlen(str_op2)<=5) && !strchr(str_op2, '#') && strchr(str_op2, '$') && (strchr(str_op2, '+') || strchr(str_op2, '-')))
                {
                    retCode=CODE_JRNZE;                                 /* Prepare the JR NZ,e                  */

                    retCheck=clean_cc(str_op1);                         /* Clean the cc operand                 */
                    retCheck=clean_e(str_op2);                          /* Clean the e operand                  */

                    if (str_op2[1]=='+')
                    {
                        word=toDecValue(str_op2+2, pLen, &lenEff);
                    }
                    else
                    {
                        word=((~toDecValue(str_op2+2, pLen, &lenEff))+1) & FIRST_LOWEST_BYTE;
                    }

                    retCode=(retCode<<SIZE_1_BYTE) + ((word - 2) & FIRST_LOWEST_BYTE);       /* Calc the (e-2) operand */  

                    *pLen=TWO_BYTES;
                }
            }

            break;           
    }

#ifdef DEBUG_DISPLAY_FINDCODE_DATA
    printf("rc=<%04X>\n", retCode);
    printf("ln=%d\n", *pLen);
#endif
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
    uint8_t ret=0;
    uint8_t lenEff=0;
    int32_t valDec=0; 
    uint32_t value=0;
    uint32_t machineCode=0;
    uint32_t codeInHexa=0;
    char instruction[MAX_INSTR_LENGTH];
    char filename[MAX_LEN];
    char *entryOp=NULL;

    if (mCommandIsEntered)
    {
        // printf(">>> analyse [%s] <<<\n", mEntry);

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
                            printf("l <file>    load a file with codes or instructions.\n");
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
                            printf("l <file>    load a file with codes or instructions.\n");
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

                    case CMD_ASSEMBLYCODE:                                              /* The 'a' command          */
                        mEntry+=2;
                        codeInHexa=toValue(mEntry, &lenValue, &lenEff);                 /* Transform the instruction into real number  */
                        interpretCode(codeInHexa, lenValue, INTP_DISPLAY);
                        
                        break;
                    
                    case CMD_MACHINECODE:                                               /* The 'c' command          */
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
                        resetAllChangedRegister();

                        if (mExecMode)
                        {
                            /* Retrieve the instruction         */
                            machineCode=getNextInstruction(instruction, &lenValue);

                            /* Execute the instruction          */
                            ret=interpretCode(machineCode, lenValue, INTP_EXECUTE_BLIND);
                            
                            /* Go to next instruction, change PC value (only if needded)          */
                            if (ret != NO_PC_CHANGE) 
                            {
                                mRegisterPack.regPC.setValue(mRegisterPack.regPC.getValue() + (lenValue/2));
                            }
                        }
                        break;

                    case CMD_LOAD_CODE:
                        if (!(entryOp=strchr(mEntry, ' ')))
                        {
                            printf("\nPlease, give me something to load !!\n");
                        }
                        else
                        {
                            if (strlen(mEntry)<3)
                            {
                                printf("\nPlease, give me something to load !!\n");
                            }
                            else
                            {
                                strcpy(filename, ASM_FILE_LOCATION);
                                strcat(filename, mEntry+2);
                                printf("\nTry to load %s ...", filename);
                                loadCode(filename);
                            }
                        }        
                        break;
                }
                break;

            case CODE:
                resetAllChangedRegister();

                codeInHexa=toValue(mEntry, &lenValue, &lenEff);                     /* Transform the instruction into real number  */
                interpretCode(codeInHexa, lenValue, INTP_EXECUTE);
                break;

            case INSTRUCTION:
                resetAllChangedRegister();
                
                machineCode=findMachineCode(mEntry, &lenValue);
                interpretCode(machineCode, lenValue, INTP_EXECUTE);
                break;
        }
    }
    
    return retValue;
}
