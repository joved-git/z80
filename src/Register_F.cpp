#include "../inc/Register_F.h"

Register_F::Register_F(): Register_8bits()
{
    mColorFlagsChanged=0;
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
        PUSHBIT(mColorFlagsChanged, (EXTRACT(mValue, BITPOS_FLAG_SIGN, 1) ^ EXTRACT(oldVal, BITPOS_FLAG_SIGN, 1)), BITPOS_FLAG_SIGN);
        //mColorFlagsChanged=EXTRACT(mValue, BITPOS_FLAG_SIGN, 1) ^ EXTRACT(oldVal, BITPOS_FLAG_SIGN, 1);
    }
}

void Register_F::setZeroFlag(bool pFlagValue)
{
    uint8_t oldVal=mValue;

    mValue=(pFlagValue?mValue|MASK_FLAG_ZERO:mValue&~MASK_FLAG_ZERO);

    if (oldVal!=mValue)
    {
        mHasJustChanged=true;
        // uint8_t b3=EXTRACT(mValue, BITPOS_FLAG_ZERO, 1) ^ EXTRACT(oldVal, BITPOS_FLAG_ZERO, 1);
        // printf("b1=%d b2=%d b3=%d\n", EXTRACT(mValue, BITPOS_FLAG_ZERO, 1), EXTRACT(oldVal, BITPOS_FLAG_ZERO, 1), b3);
        // PUSHBIT(mColorFlagsChanged, EXTRACT(mValue, BITPOS_FLAG_ZERO, 1) ^ EXTRACT(oldVal, BITPOS_FLAG_ZERO, 1), BITPOS_FLAG_ZERO);
        PUSHBIT(mColorFlagsChanged, (EXTRACT(mValue, BITPOS_FLAG_ZERO, 1) ^ EXTRACT(oldVal, BITPOS_FLAG_ZERO, 1)), BITPOS_FLAG_ZERO);
        // printf("ColorFlagChanged=%02X\n", mColorFlagsChanged);
    }
}

void Register_F::setHalfCarryFlag(bool pFlagValue)
{
    uint8_t oldVal=mValue;

    mValue=(pFlagValue?mValue|MASK_FLAG_HALF_CARRY:mValue&~MASK_FLAG_HALF_CARRY);

    if (oldVal!=mValue)
    {
        mHasJustChanged=true;
        PUSHBIT(mColorFlagsChanged, (EXTRACT(mValue, BITPOS_FLAG_HALF_CARRY, 1) ^ EXTRACT(oldVal, BITPOS_FLAG_HALF_CARRY, 1)), BITPOS_FLAG_HALF_CARRY);
        //mColorFlagsChanged=EXTRACT(mValue, BITPOS_FLAG_HALF_CARRY, 1) ^ EXTRACT(oldVal, BITPOS_FLAG_HALF_CARRY, 1);
    }
}

void Register_F::setParityOverflowFlag(bool pFlagValue)
{
    uint8_t oldVal=mValue;

    mValue=(pFlagValue?mValue|MASK_FLAG_PARITY_OVERFLOW:mValue&~MASK_FLAG_PARITY_OVERFLOW);

    if (oldVal!=mValue)
    {
        mHasJustChanged=true;
        PUSHBIT(mColorFlagsChanged, (EXTRACT(mValue, BITPOS_FLAG_PARITY_OVERFLOW, 1) ^ EXTRACT(oldVal, BITPOS_FLAG_PARITY_OVERFLOW, 1)), BITPOS_FLAG_PARITY_OVERFLOW);
        //mColorFlagsChanged=EXTRACT(mValue, BITPOS_FLAG_PARITY_OVERFLOW, 1) ^ EXTRACT(oldVal, BITPOS_FLAG_PARITY_OVERFLOW, 1);
    }
}

void Register_F::setAddSubtractFlag(bool pFlagValue)
{
    uint8_t oldVal=mValue;

    mValue=(pFlagValue?mValue|MASK_FLAG_ADD_SUBTRACT:mValue&~MASK_FLAG_ADD_SUBTRACT);

    if (oldVal!=mValue)
    {
        mHasJustChanged=true;
        PUSHBIT(mColorFlagsChanged, (EXTRACT(mValue, BITPOS_FLAG_ADD_SUBTRACT, 1) ^ EXTRACT(oldVal, BITPOS_FLAG_ADD_SUBTRACT, 1)), BITPOS_FLAG_ADD_SUBTRACT);
        //mColorFlagsChanged=EXTRACT(mValue, BITPOS_FLAG_ADD_SUBTRACT, 1) ^ EXTRACT(oldVal, BITPOS_FLAG_ADD_SUBTRACT, 1);
    }
}

void Register_F::setCarryFlag(bool pFlagValue)
{
    uint8_t oldVal=mValue;

    mValue=(pFlagValue?mValue|MASK_FLAG_CARRY:mValue&~MASK_FLAG_CARRY);

    if (oldVal!=mValue)
    {
        mHasJustChanged=true;
        PUSHBIT(mColorFlagsChanged, (EXTRACT(mValue, BITPOS_FLAG_CARRY, 1) ^ EXTRACT(oldVal, BITPOS_FLAG_CARRY, 1)), BITPOS_FLAG_CARRY);
        //mColorFlagsChanged=EXTRACT(mValue, BITPOS_FLAG_CARRY, 1) ^ EXTRACT(oldVal, BITPOS_FLAG_CARRY, 1);
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

uint8_t Register_F::getColorChangedFlag()
{
    return mColorFlagsChanged;
}

void Register_F::resetColorChangedFlag()
{
    mColorFlagsChanged=0;
}


