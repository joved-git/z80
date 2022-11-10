#ifndef OPCODE_H
#define OPCODE_H

#define EXTRACT(a, d, l)    (a>>d & ~(((uint8_t) 0b111111111)<<l))  

#define CODE_NOP            (0b00000000)
#define CODE_NOINSTRUCTION  (0b11001011)
#define CODE_LDRR           (0b01000000)
#define CODE_LDRHL          (0b01000110)
#define CODE_LDHLR          (0b01110000)
#define CODE_LDRN           (0b00000110)

#define MASK_NOP            (0b11111111)
#define MASK_LDRR           (0b11000000)
#define MASK_LDRHL          (0b11000111)
#define MASK_LDHLR          (0b11111000)
#define MASK_LDRN           (0b11000111)

#define ONE_BYTE            (2)
#define TWO_BYTES           (4)

#define REGA                (0b111)
#define REGB                (0b000)
#define REGC                (0b001)
#define REGD                (0b010)
#define REGE                (0b011)
#define REGH                (0b100)
#define REGL                (0b101)
#define REGHL               (0b110)

#define INTP_DISPLAY        (0)
#define INTP_EXECUTE        (1)

#endif
