#include "../inc/Register_16bits.h"

Register_16bits::Register_16bits()
{
    mValue=0;
}

Register_16bits::~Register_16bits()
{
    
}

void Register_16bits::set16bitsRegisterType(bool mType)
{
    mFullRegister=mType;
}

void Register_16bits::setHightLowRegister(Register_8bits *pHighReg, Register_8bits *pLowReg)
{
    mValueH=pHighReg;  
    mValueL=pLowReg;
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
        mValue=pValue;
    }
    else 
    {
        mValueH->setValue(pValue / 0X100);
        mValueL->setValue(pValue % 0X100);
    }
}
