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