#include "../inc/Register_8bits.h"

Register_8bits::Register_8bits()
{
    mValue=0;
    mHasJustChanged=false;
    mRelated16bitsRegister=NULL;
}

Register_8bits::~Register_8bits()
{

}

void Register_8bits::setValue(uint8_t pValue)
{
    if (pValue != mValue)
    {
        mHasJustChanged=true;

        if (mRelated16bitsRegister) 
        {
            mRelated16bitsRegister->setHasJustChanged(true);
        }
    }

    mValue=pValue;
}

uint8_t Register_8bits::getValue()
{
    return mValue;
}


/* Set the related 16-bit register  */
void Register_8bits::setRelated16bitsRegister(Register_16bits *pReg)
{
    mRelated16bitsRegister=pReg;
}

#ifdef SIGN_IS_HERE
/* Set flags    */
void Register_8bits::setSignFlag(bool pFlagValue)
{
    uint8_t oldVal=mValue;

    mValue=(pFlagValue?mValue|MASK_FLAG_SIGN:mValue&~MASK_FLAG_SIGN);

    if (oldVal!=mValue)
    {
        mHasJustChanged=true;
    }
}

void Register_8bits::setZeroFlag(bool pFlagValue)
{
    uint8_t oldVal=mValue;

    mValue=(pFlagValue?mValue|MASK_FLAG_ZERO:mValue&~MASK_FLAG_ZERO);

    if (oldVal!=mValue)
    {
        mHasJustChanged=true;
    }
}

void Register_8bits::setHalfCarryFlag(bool pFlagValue)
{
    uint8_t oldVal=mValue;

    mValue=(pFlagValue?mValue|MASK_FLAG_HALF_CARRY:mValue&~MASK_FLAG_HALF_CARRY);

    if (oldVal!=mValue)
    {
        mHasJustChanged=true;
    }
}

void Register_8bits::setParityOverflowFlag(bool pFlagValue)
{
    uint8_t oldVal=mValue;

    mValue=(pFlagValue?mValue|MASK_FLAG_PARITY_OVERFLOW:mValue&~MASK_FLAG_PARITY_OVERFLOW);

    if (oldVal!=mValue)
    {
        mHasJustChanged=true;
    }
}

void Register_8bits::setAddSubtractFlag(bool pFlagValue)
{
    uint8_t oldVal=mValue;

    mValue=(pFlagValue?mValue|MASK_FLAG_ADD_SUBTRACT:mValue&~MASK_FLAG_ADD_SUBTRACT);

    if (oldVal!=mValue)
    {
        mHasJustChanged=true;
    }
}

void Register_8bits::setCarryFlag(bool pFlagValue)
{
    uint8_t oldVal=mValue;

    mValue=(pFlagValue?mValue|MASK_FLAG_CARRY:mValue&~MASK_FLAG_CARRY);

    if (oldVal!=mValue)
    {
        mHasJustChanged=true;
    }
}


/* Get flags    */
bool Register_8bits::getSignFlag()
{
    return ((bool) (mValue & MASK_FLAG_SIGN));
}

bool Register_8bits::getZeroFlag()
{
    return ((bool) (mValue & MASK_FLAG_ZERO));
}

bool Register_8bits::getHalfCarryFlag()
{
    return ((bool) (mValue & MASK_FLAG_HALF_CARRY));
}

bool Register_8bits::getParityOverflowFlag()
{
    return ((bool) (mValue & MASK_FLAG_PARITY_OVERFLOW));
}

bool Register_8bits::getAddSubtractFlag()
{
    return ((bool) (mValue & MASK_FLAG_ADD_SUBTRACT));
}

bool Register_8bits::getCarryFlag()
{
    return ((bool) (mValue & MASK_FLAG_CARRY));
}
#endif

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

void Register_8bits::setBitValue(uint8_t pBit, uint8_t pValue)
{
    PUSHBIT(mValue, pValue, pBit);
}

void Register_8bits::resetChanged()
{
    mHasJustChanged=false;
}

bool Register_8bits::hasJustChanged()
{
    return mHasJustChanged;
}