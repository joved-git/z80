#ifndef COMMAND_H
#define COMMAND_H

#include <inttypes.h>

//#include <iostream>
#include <stdio.h>
#include <string.h>

#include "../inc/common_values.h"
#include "../inc/RegisterPack.h"

class Command 
{
public:
    Command();
    ~Command();

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
};	

#endif
