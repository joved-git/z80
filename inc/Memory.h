#ifndef MEMORY_H
#define MEMORY_H

#include <inttypes.h>
#include <stdio.h>                              /* printf, scanf, NULL */
#include <stdlib.h>                             /* malloc, free, rand */

#include "../inc/opcode.h"

#define MEM_SIZE                0xFFFF          /* The size of the memory                                   */
#define MEM_BYTE_INIT           0xCC            /* The size of the memory                                   */

class Memory
{
public:
    Memory();
    ~Memory();

    /// @brief  This method returns one byte from the memory.
    /// @param  The address of the byte to be read.
    /// @return The read byte.  
    uint8_t get8bitsValue(uint16_t);

    /// @brief  This method returns two bytes from the memory.
    /// @param  The address of the bytes to be read.
    /// @return The read bytes.
    uint16_t get16BitsValue(uint16_t);

    /// @brief  This method returns two bytes address from memory (lowest then highest part of the address)
    /// @param  The address of the bytes to be read.
    /// @return The read address.
    uint16_t getAddress(uint16_t);

    /// @brief  This method stores a byte into memory.
    /// @param  The address of the bytes to be written.
    /// @param  The byte to be written.
    /// @return Nothing.
    void set8bitsValue(uint16_t, uint8_t);

    /// @brief  This method stores two bytes into memory.
    /// @param  The address of the bytes to be written.
    /// @param  The word to be written.
    /// @return Nothing.
    void set16bitsValue(uint16_t, uint16_t);

    /// @brief  This method stores a two bytes address into memory.
    /// @param  The address of the bytes to be written.
    /// @param  The address to be written.
    /// @return Nothing.
    void setAddress(uint16_t, uint16_t);

    /// @brief  This method initialize the memory.
    /// @param  The address of the first byte to be written.
    /// @param  The number of byte(s) to be written.
    /// @param  The byte to be written.
    /// @return Nothing.
    void memInit(uint16_t, uint16_t, uint8_t);

private:
    uint8_t *mMemory;
};

#endif