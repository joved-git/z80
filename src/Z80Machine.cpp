#include "../inc/Z80Machine.h"

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

     /* Initialize registers to 0 exept SP  */
    mRegisterPack.regA.setValue(0x00);
    mRegisterPack.regB.setValue(0x00);
	mRegisterPack.regC.setValue(0x00);
	mRegisterPack.regD.setValue(0x00);
	mRegisterPack.regE.setValue(0x00);
	mRegisterPack.regL.setValue(0x00);
	mRegisterPack.regF.setValue(0b00000000);
    mRegisterPack.regPC.setValue(0x0000);
    mRegisterPack.regSP.setValue(INITIAL_STACK_POINTER);

     /* Initialize some registers (for test)   */
    mRegisterPack.regB.setValue(0x01);
	mRegisterPack.regC.setValue(0xAA);
	mRegisterPack.regD.setValue(0x0C);
	mRegisterPack.regE.setValue(0x10);
	mRegisterPack.regL.setValue(0xC8);
	mRegisterPack.regF.setValue(0b01101111);
    mRegisterPack.regPC.setValue(0x1234);
    mRegisterPack.regHL.setValue(0xFE14);

/*
    printf("\n\n--- set to tf f ttt\n");
    mRegisterPack.regF.setSignFlag(true);
    mRegisterPack.regF.setZeroFlag(false);
    mRegisterPack.regF.setHalfCarryFlag(false),
    mRegisterPack.regF.setParityOverflowFlag(true),
    mRegisterPack.regF.setAddSubtractFlag(true),
    mRegisterPack.regF.setCarryFlag(true);
    printf("Flags=%d%dx%dx%d%d%d\n", 
        mRegisterPack.regF.getSignFlag(),
        mRegisterPack.regF.getZeroFlag(),
        mRegisterPack.regF.getHalfCarryFlag(),
        mRegisterPack.regF.getParityOverflowFlag(),
        mRegisterPack.regF.getAddSubtractFlag(),
        mRegisterPack.regF.getCarryFlag()
    );

    printf("--- set to ft t fff\n");
    mRegisterPack.regF.setSignFlag(false);
    mRegisterPack.regF.setZeroFlag(true);
    mRegisterPack.regF.setHalfCarryFlag(true),
    mRegisterPack.regF.setParityOverflowFlag(false),
    mRegisterPack.regF.setAddSubtractFlag(false),
    mRegisterPack.regF.setCarryFlag(false);
    printf("Flags=%d%dx%dx%d%d%d\n", 
        mRegisterPack.regF.getSignFlag(),
        mRegisterPack.regF.getZeroFlag(),
        mRegisterPack.regF.getHalfCarryFlag(),
        mRegisterPack.regF.getParityOverflowFlag(),
        mRegisterPack.regF.getAddSubtractFlag(),
        mRegisterPack.regF.getCarryFlag()
    );

    printf("--- set to tf f ttt\n");
    mRegisterPack.regF.setSignFlag(true);
    mRegisterPack.regF.setZeroFlag(false);
    mRegisterPack.regF.setHalfCarryFlag(false),
    mRegisterPack.regF.setParityOverflowFlag(true),
    mRegisterPack.regF.setAddSubtractFlag(true),
    mRegisterPack.regF.setCarryFlag(true);
    printf("Flags=%d%dx%dx%d%d%d\n", 
        mRegisterPack.regF.getSignFlag(),
        mRegisterPack.regF.getZeroFlag(),
        mRegisterPack.regF.getHalfCarryFlag(),
        mRegisterPack.regF.getParityOverflowFlag(),
        mRegisterPack.regF.getAddSubtractFlag(),
        mRegisterPack.regF.getCarryFlag()
    );
*/


    /* Creating the memory  */
    mMemory=new(Memory);
}

/* The destructor  */
Z80Machine::~Z80Machine()
{
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

    reg=pRegister[0];              /* The register as a character  */
    
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

        case REGHL:
            strcpy(pRetChar, STRING_REGHL);
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

    while (i<strlen(pCode))
    {
        if (pCode[i]<'0' || pCode[i]>'F' || (pCode[i]>'9') && pCode[i]<'A')
        {
            *pLenEffective=i;
            i=10;
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
    }

    return(regReturn);
}

/* Interpret the machine code   */
uint8_t Z80Machine::interpretCode(uint32_t codeInHexa, uint8_t len, uint8_t pMode)
{
    //uint32_t codeInHexa;
    //uint8_t len=0;
    uint8_t lenEff=0;
    uint8_t op1, op2;
    uint8_t instruction=CODE_NO_INSTRUCTION;
    Register_8bits *reg8_1=NULL;
    Register_8bits *reg8_2=NULL;
    Register_16bits *reg16_1=NULL;
    uint16_t address=0x0000;

    uint8_t ret;
    char sop1[MAX_OP_LENGTH], sop2[MAX_OP_LENGTH];

    //printf("code=<%s>\n", pCode);
    //codeInHexa=toValue(pCode, &len, &lenEff);                     /* Transform the instruction into real number  */
    
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
        op1=EXTRACT(codeInHexa, 3, 3);
        op2=EXTRACT(codeInHexa, 0, 3);
    }

    /* This is a LD r,(HL)  */
    if ((codeInHexa & MASK_LDRHL)==CODE_LDRHL && len == ONE_BYTE)
    {
        instruction=CODE_LDRHL;
        
        /* Extract the value of the register (in bits)    */
        op1=EXTRACT(codeInHexa, 3, 3);
        op2=EXTRACT(codeInHexa, 0, 3);
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
        op2=codeInHexa & 0xFF;
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

     /* This is a LD A,nn  */
    if ((codeInHexa>>SIZE_2_BYTES & MASK_LDANN)==CODE_LDANN && len == THREE_BYTES)
    {
        instruction=CODE_LDANN;
        
        /* Extract the value of the operand #2 (n)    */
        op1=codeInHexa & 0x0000FF;
        op2=(codeInHexa & 0x00FF00) >> SIZE_1_BYTE;
    }

    /*************************************************************************************************************************/

    switch (instruction)
    {
        case CODE_NOP:                              /* This is a NOP    */
            if (pMode==INTP_EXECUTE)
            {
                printf("\nNOP was executed\n");
            }
            
            if (pMode==INTP_DISPLAY)
            {
                printf("\n[00] is NOP\n");
            }
            break;
        
        case CODE_HALT:                              /* This is a HALT    */
            if (pMode==INTP_EXECUTE)
            {
                printf("\nHALT was executed\n");
                /* Don't forget to program the HALT command */
            }
            
            if (pMode==INTP_DISPLAY)
            {
                printf("\n[%02X] is HALT\n", codeInHexa);
            }
            break;

        case CODE_LDRR:                             /* This is a LD r,r'    */
            if (pMode==INTP_EXECUTE)
            {
                ret=bitToRegister(op1, sop1);
                ret=bitToRegister(op2, sop2);
                printf("LD %s,%s was executed\n", sop1, sop2);

                reg8_1=get8bitsRegisterAddress(op1);
                reg8_2=get8bitsRegisterAddress(op2);

                reg8_1->setValue(reg8_2->getValue());
            }
            
            if (pMode==INTP_DISPLAY)
            {
                ret=bitToRegister(op1, sop1);
                ret=bitToRegister(op2, sop2);

                printf("\n[%02X] is LD %s,%s\n", codeInHexa, sop1, sop2);
            }
            break;

        case CODE_LDRN:                             /* This is a LD r,n    */
            if (pMode==INTP_EXECUTE)
            {
                ret=bitToRegister(op1, sop1);
                printf("LD %s,#%02X was executed\n", sop1, op2);

                reg8_1=get8bitsRegisterAddress(op1);
                
                reg8_1->setValue(op2);
            }
            
            if (pMode==INTP_DISPLAY)
            {
                ret=bitToRegister(op1, sop1);

                printf("\n[%04X] is LD %s,#%02X\n", codeInHexa, sop1, op2);
            }
            break;
        
        case CODE_LDRHL:                            /* This is a LD r,(HL)    */
            if (pMode==INTP_EXECUTE)
            {
                ret=bitToRegister(op1, sop1);
                ret=bitToRegister(op2, sop2);
                printf("LD %s,%s was executed\n", sop1, sop2);

                reg8_1=get8bitsRegisterAddress(op1);
                reg16_1=get16bitsRegisterAddress(REGHL);
                
                reg8_1->setValue(mMemory->get8bitsValue(reg16_1->getValue()));
            }
            
            if (pMode==INTP_DISPLAY)
            {
                ret=bitToRegister(op1, sop1);
                ret=bitToRegister(op2, sop2);

                printf("\n[%02X] is LD %s,%s\n", codeInHexa, sop1, sop2);
            }
            break;

        case CODE_LDHLR:                            /* This is a LD (HL),r    */   
            if (pMode==INTP_EXECUTE)
            {
                ret=bitToRegister(op1, sop1);
                ret=bitToRegister(op2, sop2);
                printf("LD %s,%s was executed\n", sop1, sop2);

                reg8_1=get8bitsRegisterAddress(op2);
                reg16_1=get16bitsRegisterAddress(REGHL);

                mMemory->set8bitsValue(reg16_1->getValue(), reg8_1->getValue());
            }
            
            if (pMode==INTP_DISPLAY)
            {
                ret=bitToRegister(op1, sop1);
                ret=bitToRegister(op2, sop2);

                printf("\n[%02X] is LD %s,%s\n", codeInHexa, sop1, sop2);
            }
            break;

        case CODE_LDHLN:                            /* This is a LD (HL),n    */   
            if (pMode==INTP_EXECUTE)
            {
                ret=bitToRegister(REGHL, sop1);
                printf("LD %s,#%02X was executed\n", sop1, op2);

                reg16_1=get16bitsRegisterAddress(REGHL);

                mMemory->set8bitsValue(reg16_1->getValue(), op2);
            }
            
            if (pMode==INTP_DISPLAY)
            {
                ret=bitToRegister(REGHL, sop1);
                
                printf("\n[%02X] is LD %s,#%02X\n", codeInHexa, sop1, op2);
            }
            break;

        case CODE_LDABC:                            /* This is a LD A,(BC)    */   
            if (pMode==INTP_EXECUTE)
            {
                printf("LD A,(BC) was executed\n");

                reg8_1=get8bitsRegisterAddress(REGA);
                reg16_1=get16bitsRegisterAddress(REGBC);

                reg8_1->setValue(mMemory->get8bitsValue(reg16_1->getValue()));
            }
            
            if (pMode==INTP_DISPLAY)
            {
                ret=bitToRegister(op1, sop1);
                ret=bitToRegister(op2, sop2);

                printf("\n[%02X] is LD A,(BC)\n", codeInHexa);
            }
            break;

        case CODE_LDADE:                            /* This is a LD A,(BC)    */   
            if (pMode==INTP_EXECUTE)
            {
                printf("LD A,(DE) was executed\n");

                reg8_1=get8bitsRegisterAddress(REGA);
                reg16_1=get16bitsRegisterAddress(REGDE);

                reg8_1->setValue(mMemory->get8bitsValue(reg16_1->getValue()));
            }
            
            if (pMode==INTP_DISPLAY)
            {
                ret=bitToRegister(op1, sop1);
                ret=bitToRegister(op2, sop2);

                printf("\n[%02X] is LD A,(DE)\n", codeInHexa);
            }
            break;
        
        case CODE_LDANN:                                    /* This is a LD A,(nn)    */   
            address=op1*0x100+op2;
            
            if (pMode==INTP_EXECUTE)
            {
                printf("LD A,(#%04X) was executed\n", address);

                reg8_1=get8bitsRegisterAddress(REGA);
                
                reg8_1->setValue(mMemory->get8bitsValue(address));
            }
            
            if (pMode==INTP_DISPLAY)
            {
                printf("\n[%06X] is LD A,(#%04X)\n", codeInHexa, address);
            }
    }
    
    return 0;
}

/* Used to cut the instrcution  */
uint8_t Z80Machine::cutInstruction(char *pInstruction, char *pInst, char *pOp1, char *pOp2)
{
    uint8_t nbObComp=0;
    char *charPos;

    pOp1[0]='\0';
    pOp2[0]='\0';

    strcpy(pInst, pInstruction);                        /* Init the return intruction string    */

    /* Is there a space into the instruction ?  */
    if (!(charPos=strchr(pInst, ' ')))
    {
        pOp1[0]='\0';
        pOp2[0]='\0';

        nbObComp=1;
    }
    else 
    {
        strcpy(pOp1, charPos+1);
        pInst[charPos-pInst]='\0';
        
        /* Is there a ',' into the instruction ?  */
        if (!(charPos=strchr(pOp1, ',')))
        {
            nbObComp=2;
        }
        else
        {
            pOp1[charPos-pOp1]='\0';
            strcpy(pOp2, charPos+1);
            nbObComp=3;
        }
    }

    return nbObComp;
}

/* Find machine code    */
uint32_t Z80Machine::findMachineCode(char *pInstruction, uint8_t *pLen)
{
    uint32_t retCode=0xFFFFFFFF;
    char str_inst[MAX_OP_LENGTH];
    char str_op1[MAX_OP_LENGTH];
    char str_op2[MAX_OP_LENGTH];
    char *str_ptr=NULL;
    uint8_t nbOfComponents=0;

    nbOfComponents=cutInstruction(pInstruction, str_inst, str_op1, str_op2);

    //printf(">>> %s %s,%s\n", str_inst, str_op1, str_op2);
    //printf("comp=%d\n", nbOfComponents);

    switch (nbOfComponents)
    {
        case 1:                                 /* Only one component in the instruction    */
            if (!strcmp(str_inst, "NOP"))
            {
                retCode=CODE_NOP;
                *pLen=ONE_BYTE;
            }
            break;

        case 2:
            break;    
    
        case 3:

            if (!strcmp(str_inst, "LD"))                            /* A LD instruction is present  */
            {
                if (strlen(str_op1)==1 && strlen(str_op2)==1)       /* Check if it is a LD, r,r' instruction    */
                {
                    retCode=CODE_LDRR;                              /* Prepare the LD r,r'  */
                    *pLen=ONE_BYTE;
                    
                    PUSHBIT(retCode, registerToBit(str_op1), 3);    /* Add the first register as bits   */
                    PUSHBIT(retCode, registerToBit(str_op2), 0);    /* Add the first register as bits   */
                }
            }
            break;
            
    }
    return retCode;


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
                    printf("\n");
                    printf("a <code>    translate <code> to assembly langage.\n");
                    printf("              Example: cb22 gives SLA D\n");
                    printf("c <cmd>     translate <cmd> to machine code.\n");
                    printf("              Example: ld c,b gives 0x41\n");
                    printf("r           display main registers.\n");
                    printf("R           display all registers.\n");
                    printf("m <addr>    dump 16 bytes memory from <addr>.\n");
                    printf("x <dec>     convert <dec> to hexa.\n");
                    printf("d <hex>     convert <hex> to decimal.\n");
                    printf("b <hex>     convert <hex> to binary.\n");
                    printf("q           quit me.\n");
                    printf("\n");
                    printf("<cmd>       execute the command.\n");
                    printf("<code>      execute the code.\n");
                    break;
          
                case CMD_EXAMPLE:
                    printf("\n");
                    printf("0x36nn - LD (HL),n\n");
                    printf("00x46  - LD B,(HL)\n"); 
                    printf("00x4E  - LD C,(HL)\n"); 
                    printf("00x70  - LD (HL), B\n");
                    printf("00x71  - LD (HL), C\n\n");

                    break;
                /* Display registers	*/
                case CMD_REGISTER:
                    //printf("--- Registers ---\n\n");
                    printf("\n");
                    printf("B:  [%02X]      C: [%02X]\n", mRegisterPack.regB.getValue(), mRegisterPack.regC.getValue());
                    printf("D:  [%02X]      E: [%02X]\n", mRegisterPack.regD.getValue(), mRegisterPack.regE.getValue());
                    printf("H:  [%02X]      L: [%02X]\n", mRegisterPack.regH.getValue(), mRegisterPack.regL.getValue());
                    printf("A:  [%02X]      F: [%02X] [%s]\n", mRegisterPack.regA.getValue(), mRegisterPack.regF.getValue(), 
                        byteToBinary(mRegisterPack.regF.getValue()));

                    printf("\n");
                    printf("BC: [%04X]    DE [%04X]\n", mRegisterPack.regBC.getValue(), mRegisterPack.regDE.getValue());
                    printf("HL: [%04X]    AF [%04X]\n", mRegisterPack.regHL.getValue(), mRegisterPack.regAF.getValue());

                    printf("\n");
                    printf("PC: [%04X]    SP [%04X]\n", mRegisterPack.regPC.getValue(), mRegisterPack.regSP.getValue());
                   
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
                    mEntry+=2;

                    value=toValue(mEntry, &lenValue, &lenEff);

                    if (lenEff<lenValue)
                    {
                        printf("\nNot a valid address\n"); 
                    }
                    else
                    {
                        dumpMemory(value);
                    }

                    break;
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
