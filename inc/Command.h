#ifndef COMMAND_H
#define COMMAND_H

#include <inttypes.h>

//#include <iostream>
#include <stdio.h>
#include <string.h>

#include "../inc/common_values.h"

class Command 
{
public:
    Command();
    ~Command();

    /// @brief  This method is used to enter the command that will be analysed.
    /// @param  The command that was entered in command line.  
    void setEntry(char *);

    /// @brief  This method analyses the is used to enter the command that will be analysed.
    /// @param  The command that was entered in command line. 
    bool analyse();

private:
    char *mEntry;
    bool mCommandIsEntered;

    typeOfEntry findEntryType(char *);
};	

#endif
