#ifndef OPCODE_H
#define OPCODE_H

#define EXTRACT(a, d, l)    (a>>d & ~(((uint8_t) 0b111111111)<<l))  

#define CODE_NOP            (0b00000000)
#define CODE_LDRR           (0b01000000)

#define MASK_LDRR           (0b11000000)

#define ONE_BYTE            (2)

#define REGA                (0b111)
#define REGB                (0b000)
#define REGC                (0b001)
#define REGD                (0b010)
#define REGE                (0b011)
#define REGH                (0b100)
#define REGL                (0b101)

#endif
