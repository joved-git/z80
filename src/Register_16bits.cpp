#include "../inc/Register_16bits.h"

Register_16bits::Register_16bits()
{
    mValue=0;
    mHasJustChanged=false;
}

Register_16bits::~Register_16bits()
{

}

void Register_16bits::set16bitsRegisterType(bool mType)
{
    mFullRegister=mType;
}

void Register_16bits::setHighLowRegister(Register_8bits *pHighReg, Register_8bits *pLowReg)
{
    mValueH=pHighReg;  
    mValueL=pLowReg;

    pHighReg->setRelated16bitsRegister(this);
    pLowReg->setRelated16bitsRegister(this);
}

void Register_16bits::setHasJustChanged(bool pVal)
{
    mHasJustChanged=pVal;
}

void Register_16bits::resetChanged()
{
    mHasJustChanged=false;
}

uint16_t Register_16bits::getValue()
{
    if (mFullRegister) 
    {
        return mValue;
    }
    else
    {
        return (mValueH->getValue()*0x100+mValueL->getValue());
    }
}

void Register_16bits::setValue(uint16_t pValue)
{
if (mFullRegister) 
    {
        if (pValue!=mValue) 
        {
            mHasJustChanged=true;
        }

        mValue=pValue;
        
    }
    else 
    {
        /* xxxjoexxx check here if values are changing  */
        if ((pValue / 0x100) != mValueH->getValue() || (pValue % 0x100) != mValueL->getValue()) 
        {
            mHasJustChanged=true;
        }

        mValueH->setValue(pValue / 0x100);
        mValueL->setValue(pValue % 0x100);
    }
}

bool Register_16bits::hasJustChanged()
{
    return mHasJustChanged;
}
