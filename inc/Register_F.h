#ifndef REGISTER_F_H
#define REGISTER_F_H

#include <inttypes.h>
#include <stdio.h>

#include "../inc/Register_8bits.h"

class Register_F: public Register_8bits
{
public:
    Register_F();
    ~Register_F();

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

    /* Get and set for color changing   */
    uint8_t getColorChangedFlag();
    void resetColorChangedFlag();

private:
    uint8_t mColorFlagsChanged;
};
#endif