#include "../inc/Register_8bits.h"

Register_8bits::Register_8bits()
{
    mValue=0;
}

Register_8bits::~Register_8bits()
{
    
}

void Register_8bits::setValue(uint8_t pValue)
{
    mValue=pValue;
}

uint8_t Register_8bits::getValue()
{
    return mValue;
}

 /* Set flags    */
void Register_8bits::setSignFlag(bool pFlagValue)
{
    mValue=(pFlagValue?mValue|FLAG_SIGN:mValue&~FLAG_SIGN);
}

void Register_8bits::setZeroFlag(bool pFlagValue)
{
    mValue=(pFlagValue?mValue|FLAG_ZERO:mValue&~FLAG_ZERO);
}

void Register_8bits::setHalfCarryFlag(bool pFlagValue)
{
    mValue=(pFlagValue?mValue|FLAG_HALF_CARRY:mValue&~FLAG_HALF_CARRY);
}

void Register_8bits::setParityOverflowFlag(bool pFlagValue)
{
    mValue=(pFlagValue?mValue|FLAG_PARITY_OVERFLOW:mValue&~FLAG_PARITY_OVERFLOW);
}

void Register_8bits::setAddSubtractFlag(bool pFlagValue)
{
    mValue=(pFlagValue?mValue|FLAG_ADD_SUBTRACT:mValue&~FLAG_ADD_SUBTRACT);
}

void Register_8bits::setCarryFlag(bool pFlagValue)
{
    mValue=(pFlagValue?mValue|FLAG_CARRY:mValue&~FLAG_CARRY);
}


/* Get flags    */
bool Register_8bits::getSignFlag()
{
    return ((bool) (mValue & FLAG_SIGN));
}

bool Register_8bits::getZeroFlag()
{
    return ((bool) (mValue & FLAG_ZERO));
}

bool Register_8bits::getHalfCarryFlag()
{
    return ((bool) (mValue & FLAG_HALF_CARRY));
}

bool Register_8bits::getParityOverflowFlag()
{
    return ((bool) (mValue & FLAG_PARITY_OVERFLOW));
}

bool Register_8bits::getAddSubtractFlag()
{
    return ((bool) (mValue & FLAG_ADD_SUBTRACT));
}

bool Register_8bits::getCarryFlag()
{
    return ((bool) (mValue & FLAG_CARRY));
}

/* Is the register to 0 */
bool Register_8bits::isZero()
{
    if (mValue==0)
    {
        return true;
    }
    else
    {
        return false;
    }
}

/* Get and set bits     */
uint8_t Register_8bits::getBit(uint8_t pBit)
{
    return EXTRACT(mValue, pBit, 1);
}

void Register_8bits::setBit(uint8_t pBit)
{
    PUSHBIT(mValue, 1, pBit);
}
    
    
void Register_8bits::resetBit(uint8_t pBit)
{
    PUSHBIT(mValue, 0, pBit);
}
