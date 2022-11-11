#ifndef REGISTER_8_BITS_H
#define REGISTER_8_BITS_H

#include <inttypes.h>
#include <stdio.h>

#include "../inc/common_values.h"

class Register_8bits 
{
public:
    Register_8bits();
    ~Register_8bits();

    uint8_t getValue();
    void setValue(uint8_t);

    /* Set flags    */
    void setSignFlag(bool);
    void setZeroFlag(bool);
    void setHalfCarryFlag(bool);
    void setParityOverflowFlag(bool);
    void setAddSubtractFlag(bool);
    void setCarryFlag(bool);

    /* Get flags    */
    bool getSignFlag();
    bool getZeroFlag();
    bool getHalfCarryFlag();
    bool getParityOverflowFlag();
    bool getAddSubtractFlag();
    bool getCarryFlag();

    

private:
    uint8_t mValue;
};	

#endif
