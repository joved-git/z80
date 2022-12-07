#include "../inc/Label.h"

Label::Label()
{

}

Label::Label(std::string pName, uint16_t pAddress)
{
    mName=pName;
    mAddress=pAddress;
}

Label::~Label()
{
}

/// @brief 
/// @param pName 
/// @param pAddress 
void Label::setValues(std::string pName, uint16_t pAddress)
{
    mName=pName;
    mAddress=pAddress;    
}

uint16_t Label::getAddress()
{
    return mAddress;
}

std::string Label::getName()
{
    return mName;
}