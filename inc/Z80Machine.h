#ifndef Z80MACHINE_H
#define Z80MACHINE_H

#include <inttypes.h>

//#include <iostream>
#include <stdio.h>
#include <string.h>

#include "../inc/common_values.h"
#include "../inc/RegisterPack.h"
#include "../inc/Memory.h"

class Z80Machine 
{
public:
    Z80Machine();
    ~Z80Machine();

    /// @brief  This method is used to enter the command that will be analysed.
    /// @param  The command that was entered in command line.  
    void setEntry(char *);

    /// @brief  This method analyses the is used to enter the command that will be analysed.
    /// @param  Nothing. 
    bool analyse();

private:
    char *mEntry;
    bool mCommandIsEntered;

    RegisterPack mRegisterPack;         /* The pack of all the registers    */

    Memory *mMemory;                    /* The memory of the Z80            */

    /// @brief  This method find the type of the entry command.
    /// @param  Nothing. 
    /// @return The type of the entre (NOTHING, COMMAND, INSTRUCTION OR CODE).
    typeOfEntry findEntryType();

    /// @brief  Verify is the string is an hexa number.
    /// @param  Nothing. 
    /// @return True if an entry is a code in hexa (up to 4 bytes).
    bool isACode();

    /// @brief  Convert the Uint8 value into a binary string.
    /// @param  The converted binary string. 
    /// @return The binary string converted.
    const char *byteToBinary(uint8_t);

    /// @brief  Convert the uint8_t value into the name of the register.
    /// @param  The converted register in a char * (ready to be printed). 
    /// @return The result of the command (see opcode.h).
    uint8_t bitToRegister(uint8_t, char *);

    /// @brief  Convert the name of the register into its binary value.
    /// @param  The register as a char *. 
    /// @return The binary code for the register (see opcode.h).
    uint8_t registerToBit(char *);

    /// @brief  Interpret the entry (hexa), tranlate it in machine code then display or execute it.
    /// @param  The entry code to be interpreted.
    /// @param  The length of the entry code (2 = 1 byte = ONE_BYTE).
    /// @param  The mode: INTP_DISPLAY or INTP_EXECUTE.
    /// @return Always 0 for now even if there is a uint8_t.
    uint8_t interpretCode(uint32_t, uint8_t, uint8_t);

    /// @brief  Find the machine code corresponding to the instruction.
    /// @param  The entry code to be interpreted.
    /// @param  In out, the length of the command.
    /// @return Always 0 for now even if there is a uint8_t.
    uint32_t findMachineCode(char *, uint8_t *);

    /// @brief  Convert the string into real value.
    /// @param  The string to convert.
    /// @param  In return, the length of the convertion (in digit, 2 digits = 1 byte).
    /// @param  In return, the effective length.
    /// @return The converted value.
    uint32_t toValue(char *, uint8_t *, uint8_t *);

    /// @brief  Convert the string into dec value.
    /// @param  The string to convert.
    /// @return The converted value.
    int32_t toDec(char *);

    /// @brief  Convert a string into uppercase.
    /// @param  The string to convert (as an input and an ouput).
    /// @return Nothing.
    void toUpper(char *);

    /// @brief  Return a pointer to the register defined by its binary code.
    /// @param  The binary value of the register.
    /// @return A pointer to a 8-bit Register (A, B, C, D, E, F, H, or L).
    Register_8bits *get8bitsRegisterAddress(uint8_t);

    /// @brief  Return a pointer to the register defined by its binary code.
    /// @param  The binary value of the register.
    /// @return A pointer to a 8-bit Register (AF, BC, DE, HL, PC or SP).
    Register_16bits *get16bitsRegisterAddress(uint8_t);

    /// @brief  This method dumps 16 bytes of memory from the given address.
    /// @param  The address of the first byte to be diplayed.
    void dumpMemory(uint16_t);

    /// @brief  This method cuts the instruction into an instruction and from 0 to 2 operand(s).
    /// @param  The instruction to cut.
    /// @param  The instruction.
    /// @param  The potential first operand.
    /// @param  The potential second operand.
    /// @return The number of components.
    int8_t cutInstruction(char *, char *, char *, char *);

    /// @brief  Clean a n operand.
    /// @param  The operand as a string.
    /// @return OK or error code (bas operand).
    int8_t clean_n(char *);

    /// @brief  Clean a nn operand.
    /// @param  The operand as a string.
    /// @return OK or error code (bas operand).
    int8_t clean_nn(char *);

    /// @brief  Clean a r operand.
    /// @param  The operand as a string.
    /// @return OK or error code (bas operand).
    int8_t clean_r(char *);

    /// @brief  Clean a (Ix+n) operand.
    /// @param  The operand as a string.
    /// @return OK or error code (bas operand).
    int8_t clean_ixn(char *);
};	

#endif
