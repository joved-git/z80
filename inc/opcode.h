#ifndef OPCODE_H
#define OPCODE_H

#define EXTRACT(a, d, l)    (a>>d & ~(((uint8_t) 0b111111111)<<l))  

#define CODE_NO_INSTRUCTION (0b11001011) 
#define CODE_NOP            (0b00000000)        
#define CODE_LDRR           (0b01000000)
#define CODE_LDRHL          (0b01000110)
#define CODE_LDHLR          (0b01110000)
#define CODE_LDRN           (0b00000110)
#define CODE_LDHLN          (0b00110110)
#define CODE_HALT           (0b01110110)
#define CODE_LDABC          (0b00001010)
#define CODE_LDADE          (0b00011010)
#define CODE_LDANN          (0b00111010)

#define MASK_NOP            (0b11111111)
#define MASK_LDRR           (0b11000000)
#define MASK_LDRHL          (0b11000111)
#define MASK_LDHLR          (0b11111000)
#define MASK_LDRN           (0b11000111)
#define MASK_LDHLN          (0b11111111)
#define MASK_HALT           (0b11111111)
#define MASK_LDABC          (0b11111111)
#define MASK_LDADE          (0b11111111)
#define MASK_LDANN          (0b11111111)

#define ONE_BYTE            (2)
#define TWO_BYTES           (4)
#define THREE_BYTES         (6)
#define FOUR_BYTES          (8)

#define SIZE_1_BYTE         (8)
#define SIZE_2_BYTES        (16)
#define SIZE_3_BYTES        (24)
#define SIZE_4_BYTES        (32)

#define REGA                (0b111)
#define REGB                (0b000)
#define REGC                (0b001)
#define REGD                (0b010)
#define REGE                (0b011)
#define REGH                (0b100)
#define REGL                (0b101)

#define REGHL               (0b110)
#define REGAF               (0b1001)
#define REGBC               (0b1010)
#define REGDE               (0b1011)

#define INTP_DISPLAY        (0)
#define INTP_EXECUTE        (1)

/****************************/
/* 0x0A   - LD A,(BC)       */
/* 0x36nn - LD (HL),n       */
/* 0x46   - LD B,(HL)       */
/* 0x4E   - LD C,(HL)       */
/* 0x70   - LD (HL), B      */
/* 0x71   - LD (HL), C      */
/* 0x71   - LD (HL), B      */
/****************************/

#endif
