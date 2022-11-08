#ifndef COMMAND_H
#define COMMAND_H

#include <inttypes.h>

#include "../inc/common_values.h"

class Command 
{
public:
    Command();
    ~Command();

    /// @brief 
    /// @param  
    void setEntry(char *);

private:
    char *mEntry;
    
};	

#endif
