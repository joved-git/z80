# 1.2.14
Change the prompt in exec mode, put the 'zee' at he end of the line.

# 1.2.13
Code the check_r(), clean_rr() and clean_e() functions (close Issue #12).

# 1.2.12
Correct the Issue #17: problem with affectation of a void mRelated16bitsRegister into Register_8bits::setValue().

# 1.2.11
Add interrupt flip-flops (IFF1 and IFF2).
Add and decode DI and EI.
Add and decode EX DE,HL.

# 1.2.10
Add I and R registers.
Add and decode LD A,I, LD I,A, LD A,R and LD R,A.
The PV flags is not yet handled into LD A,I and LD A,R (see issue #16).

# 1.2.9
Add and decode RST p.

# 1.2.8
Add and decode CCF and SCF.
Correct the isACode() function that returns true for the CCF or DAA instruction (CCF is interpreted like the hexa number 0xCCF and DAA like 0xDAA). It must return false for that instruction.
Add and decode RET.
Correct the CALL and the CALL cc (add 3 to the saved PC).
Add and decode RET cc.
Add and decode DAA.
Remove all the ";" at the end of the flag macro (like H_SET or PV_IS() for example, see Z80Machine.h).
Add X_GET with all flags to retrieve flag values.
Add VAL_x (from 0 to F) to easly convert hexa to decimal values.

# 1.2.7
The color changing is finished for details flags.

# 1.2.6
Create the Register_F class.
Resolve the issue #6.

# 1.2.5
The color register is finished but stay the register F to do.

# 1.2.4
Resolve the issue #2.
Color changing in Register_8bits and Register_16bits.
Correct the twice "was executed" on INC (HL).
Correct the INC r that was not executing anymore.

# 1.2.3
Add and decode SBC HL,ss.
Add checkHalfBorrowOnSub16() function.

# 1.2.2
Add and decode JP (HL), JP (IX) and JP (IY).

# 1.2.1
Add and decode SUB r, SUB n, SUB (HL), SUB (IX+d) and SUB (IX+d).
Add and decode SBC r, SBC n, SBC (HL), SBC (IX+d) and SBC (IX+d).

# 1.2.0
Add JR C,e, JR NC,e, JE Z,e and JR NZ,e.
Correct many printf '02X' in BIT, SET, RES instructions.
Correct the label decoding: some instruction lengths were bad.
Add JRCOND.asm files (with and without opcodes).

# 1.1.20
Add and decode JP cc,nn.
Create a clean_cc() function. To be written.
Correct the NO_PC_CHANGE for CALL cc,nn and JP cc,nn.

# 1.1.19
Add and decode ADC HL,rr.

# 1.1.18
Add and decode INC IX, INC IY, DEC IX and DEC IY.
Correct detection of "INC" or "DEC": add parenthesys into the test.

# 1.1.17
Add and decode OR r, OR n, OR (HL), OR (IX+d) and OR (IY+d).
Add and decode XOR r, XOR n, XOR (HL), XOR (IX+d) and XOR (IY+d).
Add and decode CP r, CP n, CP (HL), CP (IX+d) and CP (IY+d).
Change the name of checkOverflowOnAdd() to checkOverflowOnAdd8();
Create the checkOverflowOnSub8() function.

# 1.1.16
Add and decode DEC (IX+d) and DEC (IY+D).
Correct INC (HL) and DEC (HL): no flags setting.
Add and decode AND r, AND n, AND (HL), AND (IX+d) and AND (IY+d).
Create calcParity() function.
Change EVEN(a) macro to calcParity(a).

# 1.1.15
Add and decode ADD A,n.
Correct the size of ADD A,n into ods and csv (.h regenerated).
Add and decode ADD A,(IX+d) and ADD A,(IY+d). 
Add and decode ADC A,n, ADC A,(IX+d) and ADC A,(IY+d). 
Add and decode INC (IX+d) and INC (IY+D).

# 1.1.14
Add EX (SP),HL, EX (SP), IX and EX (SP),IY.

# 1.1.13
Add and decode LDI, LDIR, LDD and LDDR.
Add and decode CPI, CPIR, CPD and CPDR.

# 1.1.12
Add and decode RLD and RRD.

# 1.1.11
Add and decode SRA r, SRA (HL), SRA (IX+d) and SRA (IY+d).
Add and decode SRL r, SRL (HL), SRL (IX+d) and SRL (IY+d).
Add 'm (ix)', 'm (hl)' and 'm (iy)' commands.

# 1.1.10
Add and decode DJNZ e.

# 1.1.9
Add and decode SLA r, SLA (HL), SLA (IX+d) and SLA (IY+d).
Correct the RR r. Always use REGA. Prepare SRA and SRL instructions.

# 1.1.8
Add and decode PUSH IX, PUSH IY, POP qq, POP IX and POP IY.
Correct the PUSH IX/IyY (bad length).

# 1.1.7
Correct LD rr,nn. Add presence of '#' into the 'nn' string.
Add and decode LD SP,HL, LD SP,IX and LD SP,IY.

# 1.1.6
Add and decode LD (IX+d),n and LD (IY+d),n.

# 1.1.5
Integrate the complete 'JR e' into asm file and load function.

# 1.1.4
Add the loop detection process (see issue #14).
Add ".asm" is not present during a load 'l' command.

# 1.1.3
Correct bug: jp #0 was not understood.

# 1.1.2
Add and decode JR e.

# 1.1.1
Add some forgotten files to the repo.

# 1.1.0
Label detection is finished for JP, JP cc, CALL and CALL cc.

# 1.0.12
Handle label and instruction on the same line with command load 'l'.

# 1.0.11
Add new Label class.

# 1.0.9
Add and decode JP nn (prepare handling of labels into ASM programs).
Correct execution of CALL and JP: the PC was not correctly calculated.

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
The refactoring of interpretCode() is finished. The function is able to display next instruction in exec mode. Yeah !!

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
Add LD (IX+d),r and LD (IY+d),r.
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
Add LD A,(DE) and LD A,(nn) instructions.

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
Add the LD r,(HL) and LD (HL),r instructions.
Correct a bug. The command should be translat in uppercase but not the first character. Example: 'a 4e' must be turn into 'a 4E' and 'x' must stay 'x'.

# 0.3.0
Add the 'a' command.
Full encoding of the LD r,r' instruction.

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
Add A, F and binary representation for F.
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
