# 1.0.8
Add and decode BIT b,(IX+d), BIT b,(IY+d), SET b,(IX+d), SET b,(IY+d), RES b,(IX+d), RES b,(IY+d).

# 1.0.7
Introduce borrow managing: checkHalfBorrowOnSub8(), checkBorrowOnSub16() and checkHalfBorrowOnSub16() functions.
Add Half Borrow detection into NEG instruction. 
Add and decode all RR instructions (RRA, RR r, RR (HL), RR (IX+d) and RR (IY+d)).

# 1.0.6
Add and decode CPL and NEG.

# 1.0.5
Add and decode RL (HL), RL (IX+d) and RL (IY+d).
Correct the affected flags for RLC r and RL r.

# 1.0.4
Add and decode RLA and RL r.

# 1.0.3
Add and decode INC (HL) and DEC (HL).
Add and decode RES b,r and SET b,r.
Add and decode RES b,(HL) and SET b,(HL).

# 1.0.2
Add and decode DEC rr.

# 1.0.1
Search the file into the codefile directory. Handle Windows or Linux compilation.
Add and decode RRC (HL), RRC (IX+d) and RRC (IY+d) instruction.

# 1.0.0
Add the load command. Interprete the org #nn directive.

# 0.9.18
Add and decode RLC (IX+d) and RLC (IY+d). First inverted instruction codes.

# 0.9.17
Decode CALL cc,nn.

# 0.9.16
Add and decode BIT b,(HL).
Add and decode CALL nn.
Add CALL cc,nn.

# 0.9.15
Add and decode BIT b,r.
PUSHBIT() is now able to push '0'.

# 0.9.14
Add and decode ADD A,(HL) and ADC A,(HL).
Add and decode ADD HL,rr.
Add and decode ADD IX,pp.
Add and decode ADD IY,qq.

# 0.9.13
Decode LD (HL),r.
Add and decode ADC A,r.

# 0.9.12
Decode LD (HL),n.
Modify decoding of LD a,n. Add presence of '#' in the second operand.

# 0.9.11
Decode LD (nn),IY.
Correct INC r/DEC r: flags zero and sign not correctly set.

# 0.9.10
Decode LD (IX+d),r and LD (IY+d),r.

#  0.9.9
Add and decode LD (nn),IX.

# 0.9.8
Decode LD IX,(nn), LD IY,(nn), LD rr,(nn).
Correct the cutInstruction() method.

# 0.9.7
Handle the ticket #1. In phase between add and decode instruction.
Decode LD IX,nn and LD IY,nn.

# 0.9.6
Correct the bad decoding (header problem).
Close the ticket #5.

# 0.9.5
Add and decode ADD A,r instruction.

# 0.9.4
Add and decode RRCA and RRC r instructions.

# 0.9.3
Add and decode EX AF,AF' and EXX instructions.

# 0.9.2
Hidden registers integration in the exec mode.

# 0.9.1
Add and decode RLC (HL).
Correct flag settings for RLC r.

# 0.9.0
Add new registers (AF', BC', DE' and HL')
Add 'R' command.

# 0.8.7
Add and decode RLC r.

# 0.8.6
Add and decode RLCA.
New macros for easily set/reset flags (see Z80Machine.h)

# 0.8.5
Add and decode DEC r.

# 0.8.4
Add 'n' for 'next instruction' command.
Change interpretCode() to be able to execute instruction without displaying message.
Execute next instruction.

# 0.8.3
interpretCode() refactoring is finished. The function is able to display next instruction in exec mode. Yeah !!

# 0.8.2
Display next instruction in exec mode.
Correct the python prog (forget DD array).

# 0.8.1
Correct the displayExecRegisters() function (bad double registers).

# 0.8.0
Introduce the exec mode.
Create the displaySimpleRegisters(), displayExecRegisters() and the displayMemory() functions.

# 0.7.12
Add and decode LD (nn),rr.

# 0.7.11
Decode LD (nn),A.
Correct LD (nn),A and LD A,(nn).

# 0.7.10
Decode LD (BC),A and LD (DE),A.

# 0.7.9
Decode LD a,(nn).

# 0.7.8
Add 'm (sp)' command.
Set the INITIAL_STACK_POINTER to 0x0000.
Decode LD r,(HL), LD A,(BC) and LD A,(DE).

# 0.7.7
Add PUSH qq.
Decode PUSH qq.

# 0.7.6
Decode INC r and INC rr.
Add a clean_nn() function. Optimise clean_n().
Decode LD rr,nn.

# 0.7.5
Add INC rr instruction.

# 0.7.4
Add INC r (with flags modification).
Multiply by 2 the length of any instruction (instruction_length.h).
Add the visualisation of the flags with the 'r' command.

# 0.7.3
Add LD (nn),HL. Add setAddress() to Memory class.

# 0.7.2
Switch from #define to array for the instruction lengths.

# 0.7.1
The generate_include.py file is terminated.

# 0.7.0
Add python file (generate_include.py) used to generate instruction_length.h file.
First try with VALUE() macro.

# 0.6.14
Add LD IX,(nn).
Add LD IY,(nn).

# 0.6.13
Add LD dd,(nn).
Add LD HL,(nn).
Add getAddress() into Memory class.

# 0.6.12 
Add LD IX,nn and LD IY,nn.

# 0.6.11
Decode LD r,(IY+d).
Correct the LD r,(IX+d).

# 0.6.10
Correct the LD rr,nn. Invert low and hight bytes.

# 0.6.9
Add LD (IX+d), r and LD (IY+d),r.
Display displacement as a positive or negative number.

# 0.6.8
Add LD r,(IY+d) instruction.
Handle negative displacement (use SIGN() macro).

# 0.6.7
Add 'm pc' command.
Add LD rr,nn instruction.

# 0.6.6
Add clean_r(), clean_ixn() and clean_n() function.
Decode the LD r,(IX+n) command.

# 0.6.5
Handle instruction beginning with 0xDD.
Add LD r,(IX+d) instruction.
Add IX and IY registers.

# 0.6.4
Add direct and indirect register entries into opcode.h.
Able to display both: HL and (HL) for example.

# 0.6.3
Clean the operand.
Decode LD r,n.

# 0.6.2
Add LD (nn),A instruction.

# 0.6.1
Add LD (BC),A and LD (DE),A instructions.

# 0.6.0
Decode first pack of instruction (LD r,r').
Add registerToBit() method.

# 0.5.6
Cut instruction method is terminted (cutInstruction()).

# 0.5.5
Decode LD r,r' instruction.

# 0.5.4
Add LD A,(DE) and LD A,(nn) instruction.

# 0.5.3
Add the memory dump (16 bytes).

# 0.5.2
Add LD A,(BC) instruction.
Change 'm' to 'c for machine code.
Add 'm' for dump memory.
Add 'b' for convert one byte to binary.
Add the binary convertion.

# 0.5.1
Begin the interpretation of instruction. NOP is implemented.
Change interpretCode().

# 0.5.0
Add access to the flags (get, set).

# 0.4.9
Add the HALT command (but not yet executed).

# 0.4.8
Add the LD (HL),n instruction.
Add the command 'e' to view somme useful examples (great !!).

# 0.4.7
Add the LD (HL),r instruction.

# 0.4.6
Add the LD r,(HL) instruction.

# 0.4.5
Add the memory in my machine. class Memory.

# 0.4.4
Add check content into toValue() and toHexa() methods.

# 0.4.3 
Invert 'x' and 'd' commands

# 0.4.2
Add hex and dec converters (command 'x' and 'd').
Change command 'x' to 'q'.

# 0.4.1
Add LD r,n instruction.

# 0.4.0
Add get8bitsRegisterAddress() method in Z80Machine.
LD r,r' is executable. Try it, it's cool ;) !!!

# 0.3.2
Change interpretCode() : use mode to display or execute instruction.
Ready to add first execution instruction (probably LD r,r').

# 0.3.1
Add the AF register (HALF).
Invert 'a' and 'm' command.
Change the help message ('a' and 'm' had bad inverted example).
Add the entry (HL) as a register.
Add the LD r,(HL) and LD (HL), r instructions.
Correct a bug. The command should be translat in uppercase but not the first character. Example: 'a 4e' must be turn into 'a 4E' and 'x' must stay 'x'.

# 0.3.0
Add the 'a' command.
Full encoding of the LD r,r' instrcution.

# 0.2.3
Decode the LD r,r' instruction.

# 0.2.2
Add the setValue() for 16-bit registers.
Adjust display of registers (align them all).

# 0.2.1
Add 16-bit registers and handle half (example BC) and full (like PC) types.

# 0.2.0
Change class Command to Z80Machine.

# 0.1.1
Add register pack into the machine.

# 0.1.0
First version with definitive core.
Add Command class. Code refactoring.

# 0.0.9
Change the way to display register.
Add A and F and binary representation for F.
Add examples into help screen.

# 0.0.8
Add a Makefile for Windows. See REAMDE.md file for the compilation instruction.

# 0.0.7
Add the 'r' function and display only 8-bit registers.

# 0.0.6
Add class for 8-bit registers.

# 0.0.5
Add compilation instruction into README.md file.

# 0.0.4
Add the 'z>' prompt.
Add the 'x' command in the help.

# 0.0.3
Add a CHANGELOG.md file.
Add a version.hpp file. Display the version.

# 0.0.2:
Add a 'h' command.

# 0.0.1: 
First version with almost nothing.
