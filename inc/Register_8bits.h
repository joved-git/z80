#ifndef REGISTER_8_BITS_H
#define REGISTER_8_BITS_H

#include <inttypes.h>
#include <stdio.h>

#include "../inc/common_values.h"
#include "../inc/Register_16bits.h"

class Register_16bits;

class Register_8bits 
{
public:
    Register_8bits();
    ~Register_8bits();

    uint8_t getValue();
    void setValue(uint8_t);

    /* Some useful methods   */
    bool isZero();
    bool hasJustChanged();
    void resetChanged();
    void setRelated16bitsRegister(Register_16bits *);

    /* Get and set bits     */
    uint8_t getBit(uint8_t);
    void setBit(uint8_t);
    void resetBit(uint8_t);
    void setBitValue(uint8_t, uint8_t);

protected:
    uint8_t mValue;
    bool mHasJustChanged;

private: 
    Register_16bits *mRelated16bitsRegister;
};	

#endif
