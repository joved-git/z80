#ifndef Z80MACHINE_H
#define Z80MACHINE_H

#include <inttypes.h>

//#include <iostream>
#include <stdio.h>
#include <string.h>

#include "../inc/common_values.h"
#include "../inc/RegisterPack.h"

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
    RegisterPack mRegisterPack;          /* The pack of all the registers    */

    /// @brief  This method find the type of the entry command.
    /// @param  Nothing. 
    typeOfEntry findEntryType();

    /// @brief  Verify is the string is an hexa number.
    /// @param  Nothing. 
    bool isACode();

    /// @brief  Convert the Uint8 value into a binary string.
    /// @param  The converted binary string. 
    const char *byteToBinary(uint8_t);

    /// @brief  Convert the Uint8 value into the name of the register.
    /// @param  The converted register in a char * (ready to be printed). 
    /// @return The result of the command (see opcode.h).
    uint8_t bitToRegister(uint8_t, char *);

    /// @brief  
    /// @param
    uint8_t interpretCode(char *, uint8_t);

    /// @brief  
    /// @param
    uint32_t toHexa(char *, uint8_t *);

    /// @brief  
    /// @param
    void toUpper(char *);

    /// @brief  
    /// @param
    void displayCode(char *);

    /// @brief  
    /// @param
    void executeCode(char *);
};	

#endif
