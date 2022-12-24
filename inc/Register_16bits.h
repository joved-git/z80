#ifndef REGISTER_16_BITS_H
#define REGISTER_16_BITS_H

#include <inttypes.h>
#include <stdio.h>

#include "../inc/common_values.h"
#include "../inc/Register_8bits.h"

class Register_8bits;

class Register_16bits 
{
public:
    Register_16bits();
    ~Register_16bits();

    uint16_t getValue();
    void setValue(uint16_t);
    void set16bitsRegisterType(bool);
    void setHighLowRegister(Register_8bits *, Register_8bits *);
    bool hasJustChanged();
    void resetChanged();
    void setHasJustChanged(bool);

private:
    uint16_t mValue;          
    Register_8bits *mValueH;  
    Register_8bits *mValueL;
    bool mFullRegister;
    bool mHasJustChanged;
};	

#endif
