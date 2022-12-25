#include "../inc/Register_F.h"

Register_F::Register_F(): Register_8bits()
{

}

Register_F::~Register_F()
{
    
}

/* Set flags    */
void Register_F::setSignFlag(bool pFlagValue)
{
    uint8_t oldVal=mValue;

    mValue=(pFlagValue?mValue|MASK_FLAG_SIGN:mValue&~MASK_FLAG_SIGN);

    if (oldVal!=mValue)
    {
        mHasJustChanged=true;
    }
}

void Register_F::setZeroFlag(bool pFlagValue)
{
    uint8_t oldVal=mValue;

    mValue=(pFlagValue?mValue|MASK_FLAG_ZERO:mValue&~MASK_FLAG_ZERO);

    if (oldVal!=mValue)
    {
        mHasJustChanged=true;
    }
}

void Register_F::setHalfCarryFlag(bool pFlagValue)
{
    uint8_t oldVal=mValue;

    mValue=(pFlagValue?mValue|MASK_FLAG_HALF_CARRY:mValue&~MASK_FLAG_HALF_CARRY);

    if (oldVal!=mValue)
    {
        mHasJustChanged=true;
    }
}

void Register_F::setParityOverflowFlag(bool pFlagValue)
{
    uint8_t oldVal=mValue;

    mValue=(pFlagValue?mValue|MASK_FLAG_PARITY_OVERFLOW:mValue&~MASK_FLAG_PARITY_OVERFLOW);

    if (oldVal!=mValue)
    {
        mHasJustChanged=true;
    }
}

void Register_F::setAddSubtractFlag(bool pFlagValue)
{
    uint8_t oldVal=mValue;

    mValue=(pFlagValue?mValue|MASK_FLAG_ADD_SUBTRACT:mValue&~MASK_FLAG_ADD_SUBTRACT);

    if (oldVal!=mValue)
    {
        mHasJustChanged=true;
    }
}

void Register_F::setCarryFlag(bool pFlagValue)
{
    uint8_t oldVal=mValue;

    mValue=(pFlagValue?mValue|MASK_FLAG_CARRY:mValue&~MASK_FLAG_CARRY);

    if (oldVal!=mValue)
    {
        mHasJustChanged=true;
    }
}


/* Get flags    */
bool Register_F::getSignFlag()
{
    return ((bool) (mValue & MASK_FLAG_SIGN));
}

bool Register_F::getZeroFlag()
{
    return ((bool) (mValue & MASK_FLAG_ZERO));
}

bool Register_F::getHalfCarryFlag()
{
    return ((bool) (mValue & MASK_FLAG_HALF_CARRY));
}

bool Register_F::getParityOverflowFlag()
{
    return ((bool) (mValue & MASK_FLAG_PARITY_OVERFLOW));
}

bool Register_F::getAddSubtractFlag()
{
    return ((bool) (mValue & MASK_FLAG_ADD_SUBTRACT));
}

bool Register_F::getCarryFlag()
{
    return ((bool) (mValue & MASK_FLAG_CARRY));
}
