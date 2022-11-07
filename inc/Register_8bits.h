#ifndef REGISTER_8_BITS_H
#define REGISTER_8_BITS_H

#include <inttypes.h>

class Register_8bits 
{
public:
    Register_8bits();
    ~Register_8bits();

    uint8_t getValue();
    void setValue(uint8_t);

private:
    uint8_t mValue;
};	

#endif
