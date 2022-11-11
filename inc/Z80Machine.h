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

    /// @brief  Convert the Uint8 value into the name of the register.
    /// @param  The converted register in a char * (ready to be printed). 
    /// @return The result of the command (see opcode.h).
    uint8_t bitToRegister(uint8_t, char *);

    /// @brief  Interpret the entry (hexa), tranlate it in machine code then display or execute it.
    /// @param  The entry code to be interpreted.
    /// @param  The mode: INTP_DISPLAY or INTP_EXECUTE.
    /// @return Always 0 for now even if there is a uint8_t.
    uint8_t interpretCode(char *, uint8_t);

    /// @brief  Convert the string into hexa value.
    /// @param  The string to convert.
    /// @param  In return, the length of the convertio (in digit, 2 digits = 1 byte).
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
};	

#endif
