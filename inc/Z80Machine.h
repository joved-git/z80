#ifndef Z80MACHINE_H
#define Z80MACHINE_H

#include <inttypes.h>

//#include <iostream>
#include <stdio.h>
#include <string.h>

#include "../inc/common_values.h"
#include "../inc/RegisterPack.h"
#include "../inc/Memory.h"

#define REGISTER_B                    &mRegisterPack.regB
#define REGISTER_C                    &mRegisterPack.regC
#define REGISTER_D                    &mRegisterPack.regD
#define REGISTER_E                    &mRegisterPack.regE
#define REGISTER_H                    &mRegisterPack.regH
#define REGISTER_L                    &mRegisterPack.regL
#define REGISTER_A                    &mRegisterPack.regA
#define REGISTER_F                    &mRegisterPack.regF

#define REGISTER_I                    &mRegisterPack.regI
#define REGISTER_R                    &mRegisterPack.regR

#define REGISTER_BC                   &mRegisterPack.regBC
#define REGISTER_DE                   &mRegisterPack.regDE
#define REGISTER_HL                   &mRegisterPack.regHL
#define REGISTER_AF                   &mRegisterPack.regAF
#define REGISTER_SP                   &mRegisterPack.regSP
#define REGISTER_PC                   &mRegisterPack.regPC
#define REGISTER_IX                   &mRegisterPack.regIX
#define REGISTER_IY                   &mRegisterPack.regIY

#define REGISTER_BCp                  &mRegisterPack.regBCp
#define REGISTER_DEp                  &mRegisterPack.regDEp
#define REGISTER_HLp                  &mRegisterPack.regHLp
#define REGISTER_AFp                  &mRegisterPack.regAFp

/* Some very useful macro   */
#define S_SET   mRegisterPack.regF.setSignFlag(true)
#define S_RESET mRegisterPack.regF.setSignFlag(false)
#define S_IS(s) mRegisterPack.regF.setSignFlag(s)
#define S_GET   mRegisterPack.regF.getSignFlag()

#define Z_SET   mRegisterPack.regF.setZeroFlag(true)
#define Z_RESET mRegisterPack.regF.setZeroFlag(false)
#define Z_IS(s) mRegisterPack.regF.setZeroFlag(s)
#define Z_GET   mRegisterPack.regF.getZeroFlag()

#define H_SET   mRegisterPack.regF.setHalfCarryFlag(true)
#define H_RESET mRegisterPack.regF.setHalfCarryFlag(false)
#define H_IS(s) mRegisterPack.regF.setHalfCarryFlag(s)
#define H_GET   mRegisterPack.regF.getHalfCarryFlag()

#define PV_SET   mRegisterPack.regF.setParityOverflowFlag(true)
#define PV_RESET mRegisterPack.regF.setParityOverflowFlag(false)
#define PV_IS(s) mRegisterPack.regF.setParityOverflowFlag(s)
#define PV_GET   mRegisterPack.regF.getParityOverflowFlag()

#define N_SET   mRegisterPack.regF.setAddSubtractFlag(true)
#define N_RESET mRegisterPack.regF.setAddSubtractFlag(false)
#define N_IS(s) mRegisterPack.regF.setAddSubtractFlag(s)
#define N_GET   mRegisterPack.regF.getAddSubtractFlag()

#define C_SET   mRegisterPack.regF.setCarryFlag(true)
#define C_RESET mRegisterPack.regF.setCarryFlag(false)
#define C_IS(s) mRegisterPack.regF.setCarryFlag(s)
#define C_GET   mRegisterPack.regF.getCarryFlag()

class Z80Machine 
{
public:
    Z80Machine();
    ~Z80Machine();

    /// @brief  This method is used to enter the command that will be analysed.
    /// @param  The command that was entered in command line.  
    void setEntry(char *);

    /// @brief  This method analyses is used to enter the command that will be analysed.
    /// @param  Nothing. 
    bool analyse();

    /// @brief  Display registers.
    /// @param  None.
    /// @return Nothing.
    void displaySimpleRegisters();

   /// @brief  Display all registers (included hidden ones).
    /// @param  None.
    /// @return Nothing.
    void displayAllRegisters();

    /// @brief  Display registers in exec mode.
    /// @param  None.
    /// @return Nothing.
    void displayExecRegisters();

    /// @brief  Display memory from (PC).
    /// @param  The starting address or (SP) or (PC).
    /// @return Nothing.
    void displayMemory(const char *);

    /// @brief  Give the execution mode.
    /// @param  None.
    /// @return True if the mode is execustion.
    bool getExecutionMode();

    /// @brief  Give the next instruction to execute.
    /// @param  return the instruction as a string.
    /// @param  return the length of the machine code (in digit).
    /// @return the machineCode.
    uint32_t getNextInstruction(char *, uint8_t *);

    /// @brief  Give the next instruction as a string.
    /// @param  None.
    /// @return The instruction as a string.
    char *getInstruction();

private:
    char *mEntry;
    bool mCommandIsEntered;

    RegisterPack mRegisterPack;          /* The pack of all the registers                    */

    Memory *mMemory;                     /* The memory of the Z80                            */
    bool mExecMode;                      /* The mode of the machine (exec or normal/default) */
    char mInstruction[MAX_INSTR_LENGTH]; /* The decoded instruction                          */

    /// @brief  This method give the parity of a byte.
    /// @param  A byte. 
    /// @return The parity (1 = even, 0 = odd).
    uint8_t calcParity(uint8_t);

    /// @brief  This method find the type of the entry command.
    /// @param  Nothing. 
    /// @return The type of the entry (NOTHING, COMMAND, INSTRUCTION OR CODE).
    typeOfEntry findEntryType();

    /// @brief  Verify is the string is an hexa number.
    /// @param  Nothing. 
    /// @return True if an entry is a code in hexa (up to 4 bytes).
    bool isACode();

    /// @brief  Convert the Uint8 value into a binary string.
    /// @param  The converted binary string. 
    /// @return The binary string converted.
    const char *byteToBinary(uint8_t);

    /// @brief  Convert the uint8_t value into the name of the register.
    /// @param  The converted register in a char * (ready to be printed). 
    /// @return The result of the command (see opcode.h).
    uint8_t bitToRegister(uint8_t, char *);

    /// @brief  Convert the uint8_t value into the name of the condition.
    /// @param  The converted condition in a char * (ready to be printed). 
    /// @return The result of the command (see opcode.h).
    uint8_t bitToCondition(uint8_t, char *);

    /// @brief  Tell if the given condition is true or not.
    /// @param  The condition (in binary) that must be checked. 
    /// @return The result of the command. True if the given condition is true.
    bool isConditionTrue(uint8_t);

    /// @brief  Convert the name of the register into its binary value.
    /// @param  The register as a char *. 
    /// @return The binary code for the register (see opcode.h).
    uint8_t registerToBit(char *);

    /// @brief  Convert the name of the condition into its binary value.
    /// @param  The condition as a char *. 
    /// @return The binary code for the register (see opcode.h).
    uint8_t conditionToBit(char *);

    /// @brief  Convert the 3-bit number (string) into its binary value (from 0 to 7).
    /// @param  The register as a char *. 
    /// @return The binary code for the register (see opcode.h).
    uint8_t numberToBit(char *);

    /// @brief  Interpret the entry (hexa), tranlate it in machine code then display or execute it.
    /// @param  The entry code to be interpreted.
    /// @param  The length of the entry code (2 = 1 byte = ONE_BYTE).
    /// @param  The mode: INTP_DISPLAY or INTP_EXECUTE.
    /// @return Always 0 for now even if there is a uint8_t.
    uint8_t interpretCode(uint32_t, uint8_t, uint8_t);

    /// @brief  Find the machine code corresponding to the instruction.
    /// @param  The entry code to be interpreted.
    /// @param  In out, the length of the command.
    /// @return Always 0 for now even if there is a uint8_t.
    uint32_t findMachineCode(char *, uint8_t *);

    /// @brief  Convert the hexa string into real value.
    /// @param  The string to convert.
    /// @param  In return, the length of the convertion (in digit, 2 digits = 1 byte).
    /// @param  In return, the effective length.
    /// @return The converted value.
    uint32_t toValue(char *, uint8_t *, uint8_t *);

    /// @brief  Convert the dec string into real value.
    /// @param  The string to convert.
    /// @param  In return, the length of the convertion (in digit, 2 digits = 1 byte).
    /// @param  In return, the effective length.
    /// @return The converted value.
    uint32_t toDecValue(char *, uint8_t *, uint8_t *);

    /// @brief  Convert the string into dec value.
    /// @param  The string to convert.
    /// @return The converted value.
    int32_t toDec(char *);

    /// @brief  Convert a string into uppercase.
    /// @param  The string to convert (as an input and an ouput).
    /// @return Nothing.
    void toUpper(char *);

    /// @brief  Return a pointer to the register defined by its binary code.
    /// @param  The binary value of the register.
    /// @return A pointer to a 8-bit Register (A, B, C, D, E, F, H, or L).
    Register_8bits *get8bitsRegisterAddress(uint8_t);

    /// @brief  Return a pointer to the register defined by its binary code.
    /// @param  The binary value of the register.
    /// @return A pointer to a 8-bit Register (AF, BC, DE, HL, PC or SP).
    Register_16bits *get16bitsRegisterAddress(uint8_t);

    /// @brief  This method dumps 16 bytes of memory from the given address.
    /// @param  The address of the first byte to be diplayed.
    void dumpMemory(uint16_t);

    /// @brief  This method cuts the instruction into an instruction and from 0 to 2 operand(s).
    /// @param  The instruction to cut.
    /// @param  The instruction.
    /// @param  The potential first operand.
    /// @param  The potential second operand.
    /// @return The number of components.
    int8_t cutInstruction(char *, char *, char *, char *);

    /// @brief  Clean a n operand.
    /// @param  The operand as a string.
    /// @return OK or error code (bad operand).
    int8_t clean_n(char *);

    /// @brief  Clean a nn operand.
    /// @param  The operand as a string.
    /// @return OK or error code (bad operand).
    int8_t clean_nn(char *);

    /// @brief  Clean a cc operand.
    /// @param  The operand as a string.
    /// @return OK or error code (bad operand).
    int8_t clean_cc(char *);

    /// @brief  Clean a (nn) operand.
    /// @param  The operand as a string.
    /// @return OK or error code (bad operand).
    int8_t clean_inn(char *);

    /// @brief  Clean a r operand.
    /// @param  The operand as a string.
    /// @return OK or error code (bad operand).
    int8_t clean_r(char *);

    /// @brief  Clean a r operand.
    /// @param  The operand as a string.
    /// @return OK or error code (bad operand).
    int8_t clean_rr(char *);

    /// @brief  Clean a e operand.
    /// @param  The operand as a string.
    /// @return OK or error code (bad operand).
    int8_t clean_e(char *);

    /// @brief  Clean a (IX+d) or (IY+d) operand.
    /// @param  The operand as a string.
    /// @return OK or error code (bas operand).
    int8_t clean_ixn(char *);

    /// @brief  Check if it will be an half carry on an 8-bit addition.
    /// @param  The 2 bytes to add.
    /// @return true if it will be a half carry.
    bool checkHalfCarryOnAdd8(uint8_t, uint8_t);

    /// @brief  Check if it will be a carry on an 8-bit addition.
    /// @param  The 2 bytes to add.
    /// @return true if it will be a carry.
    bool checkCarryOnAdd8(uint8_t, uint8_t);

    /// @brief  Check if it will be an half carry on an 16-bit addition.
    /// @param  The 2 bytes to add.
    /// @return true if it will be a half carry.
    bool checkHalfCarryOnAdd16(uint16_t, uint16_t);

    /// @brief  Check if it will be an half borrow on an 16-bit substrction.
    /// @param  The 2 bytes to add.
    /// @return true if it will be a half borrow.
    bool checkHalfBorrowOnAdd16(uint16_t, uint16_t);

    /// @brief  Check if it will be a carry on an 16-bit addition.
    /// @param  The 2 bytes to add.
    /// @return true if it will be a carry.
    bool checkCarryOnAdd16(uint16_t, uint16_t);

    /// @brief  Check if it will be an overflow on an addition.
    /// @param  The 2 bytes to add.
    /// @return true if it will be an overflow.
    bool checkOverflowOnAdd8(uint8_t, uint8_t);

    /// @brief  Check if it will be an overflow on an subscraction.
    /// @param  The 2 bytes to add.
    /// @return true if it will be an overflow.
    bool checkOverflowOnSub8(uint8_t, uint8_t);

    /// @brief  Check if it will be an half borrow on an 8-bit substraction.
    /// @param  The 2 bytes to substract (#1 - #2).
    /// @return true if it will be a half borrow.
    bool checkHalfBorrowOnSub8(uint8_t, uint8_t);

    /// @brief  Check if it will be a borrow on an 8-bit substraction.
    /// @param  The 2 bytes to substract (#1 - #2).
    /// @return true if it will be a borrow.
    bool checkBorrowOnSub8(uint8_t, uint8_t);

    /// @brief  Check if it will be an half borrow on an 16-bit substraction.
    /// @param  The 2 bytes to substract (#1 - #2).
    /// @return true if it will be a half borrow.
    bool checkHalfBorrowOnSub16(uint16_t, uint16_t);

    /// @brief  Check if it will be a borrow on an 16-bit substraction.
    /// @param  The 2 bytes to substarct (#1 - #2).
    /// @return true if it will be a borrow.
    bool checkBorrowOnSub16(uint16_t, uint16_t);

    /// @brief  Load a file with codes or instructions.
    /// @param  The file to load.
    /// @return Nothing.
    void loadCode(char *);

    /// @brief  Clean the line from an asm  file.
    /// @param  The line to clean (before/after).
    /// @return Nothing.
    void clean_line(char *);

    /// @brief  Reset or init all the changed bool onto all registers.
    /// @param  None.
    /// @return Nothing.
    void resetAllChangedRegister();

    /// @brief  Init the Register Pack.
    /// @param  None.
    /// @return Nothing.
    void initRegisterPack();

    /// @brief  Display the value of 8-bit register and change color if needed.
    /// @param  The reg to display.
    /// @return Nothing.
    void displayReg8Bits(Register_8bits *, const char *);

    /// @brief  Display the value of 16-bit register and change color if needed.
    /// @param  The reg to display.
    /// @return Nothing.
    void displayReg16Bits(Register_16bits *, const char *);

    /// @brief  Display the details of the Register F.
    /// @param  None.
    /// @return Nothing.
    void displayDetailsRegisterF();
};	

#endif
