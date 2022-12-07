#ifndef LABEL_H
#define LABEL_H

#include <inttypes.h>
#include <stdio.h>                              /* printf, scanf, NULL */
#include <stdlib.h>                             /* malloc, free, rand */
#include <string>

class Label
{
public:

    Label(std::string, uint16_t);
    Label();
    ~Label();
    void setValues(std::string, uint16_t);
    uint16_t getAddress();
    std::string getName();


private:
    std::string mName;
    uint16_t mAddress;
};

#endif