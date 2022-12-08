#include "../inc/LabelDataset.h"

LabelDataset::LabelDataset()
{

}

LabelDataset::~LabelDataset()
{

}

void LabelDataset::add(Label pLabel)
{
    mListLabel.insert(mListLabel.begin(), pLabel);
}

uint16_t LabelDataset::findAddress(char *plabel)
{
    uint16_t retAddress=0x0000;

    for (auto it=mListLabel.begin(); it!=mListLabel.end(); it++)
    {
        if (!strcmp(plabel, it->getName().c_str()))
        {
            retAddress=it->getAddress();
        }
    }

    return retAddress;
}