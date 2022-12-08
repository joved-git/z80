#ifndef LABELDATASET_H
#define LABELDATASET_H

#include <inttypes.h>
//#include <stdio.h>                              /* printf, scanf, NULL    */
//#include <stdlib.h>                             /* malloc, free, rand     */
#include <string.h>                             /* strcmp, ...            */
#include <string>
#include <list>

#include "../inc/Label.h"

class LabelDataset
{
public:
    LabelDataset();
    ~LabelDataset();
    void add(Label);
    uint16_t findAddress(char *);

private:
    std::list<Label> mListLabel;
};

#endif