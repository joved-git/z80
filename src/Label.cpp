#include "../inc/Label.h"

Label::Label(std::string pName, uint8_t pAddress)
{
    mName=pName;
    mAddress=pAddress;
}

Label::~ Label()
{
}