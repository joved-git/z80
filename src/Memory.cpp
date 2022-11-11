#include "../inc/Memory.h"

    Memory::Memory()
    {
        /* Create the memory    */
        mMemory=(uint8_t *) malloc(MEM_SIZE*sizeof(uint8_t));

        if (!mMemory) 
        {
            printf("Impossible to create memory...\n");
        }
        else
        {
            /* Initialize memory with (MEM_BYTE_INIT)  */
            memInit(0x0000, MEM_SIZE, MEM_BYTE_INIT);
            //printf("Initialize memory with %02X\n", MEM_BYTE_INIT);
        }
    }

    Memory::~Memory()
    {
        /* Destroy the memory   */
        free(mMemory);
    }

    /* This method returns one byte from the memory.    */
    uint8_t Memory::get8bitsValue(uint16_t pAddress)
    {
        return mMemory[pAddress];
    }

    /* This method returns two byte from the memory.    */
    uint16_t Memory::get16BitsValue(uint16_t)
    {
        uint16_t bytes=0;

        return bytes;
    }

    /* This method stores a byte into memory.           */
    void Memory::set8bitsValue(uint16_t pAddress, uint8_t pValue)
    {
        mMemory[pAddress]=pValue;
    }

    /* This method stores two bytes into memory.        */
    void Memory::set16bitsValue(uint16_t pAddress, uint16_t pValue)
    {

    }

    /* This method initialize the memory.               */
    void Memory::memInit(uint16_t pStart, uint16_t pSize, uint8_t pValue)
    {
        for (uint16_t addr=pStart; addr<pStart+pSize; addr++)
        {
            set8bitsValue(addr, pValue);
        }
    }
