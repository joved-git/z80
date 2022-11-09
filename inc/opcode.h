#ifndef OPCODE_H
#define OPCODE_H

#define CODE_NOP            (0b00000000)
#define CODE_LDRR           (0b01000000)

#define MASK_LDRR           (0b11000000)

#define ONE_BYTE            (2)

#define REGA                (111)
#define REGB                (000)
#define REGC                (001)
#define REGD                (010)
#define REGE                (011)
#define REGH                (100)
#define REGL                (101)

#define EXTRACT(a, d, l)    (a>>d & ~(0b111111111<<l))   

#endif
