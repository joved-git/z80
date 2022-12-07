#ifndef LABEL_H
#define LABEL_H

#include <inttypes.h>
#include <stdio.h>                              /* printf, scanf, NULL */
#include <stdlib.h>                             /* malloc, free, rand */
#include <string>

class Label
{
public:
    Label(std::string, uint8_t);
    ~Label();
    void setValues(std::string, uint8_t);

private:
    std::string mName;
    uint8_t mAddress;
};

#endif