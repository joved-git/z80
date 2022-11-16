#ifndef OPCODE_H
#define OPCODE_H

#define EXTRACT(a, d, l)    (a>>d & ~(((uint8_t) 0b111111111)<<l)) 
#define PUSHBIT(a, b, l)    (a=a|(b<<l))
#define SIGN(a)             (a>>7)

#define ALT_CODE_CB         (0xCB)
#define ALT_CODE_ED         (0xED)
#define ALT_CODE_DD         (0xDD)
#define ALT_CODE_FD         (0xFD)

#define CODE_NO_INSTRUCTION (0b11001011)                // 0xDE
#define CODE_NOP            (0b00000000)                // 0X00
#define CODE_LDRR           (0b01000000)                
#define CODE_LDRHL          (0b01000110)
#define CODE_LDHLR          (0b01110000)
#define CODE_LDRN           (0b00000110)
#define CODE_LDHLN          (0b00110110)                // 0x36
#define CODE_HALT           (0b01110110)
#define CODE_LDABC          (0b00001010)                // 0x0A
#define CODE_LDADE          (0b00011010)                // 0x1A
#define CODE_LDANN          (0b00111010)                // 0x3A
#define CODE_LDBCA          (0b00000010)                // 0x02
#define CODE_LDDEA          (0b00010010)                // 0x12
#define CODE_LDNNA          (0b00110010)                // 0x32
#define CODE_LDRIXD         (0b01000110)
#define CODE_LDRIYD         (0b01000110)
#define CODE_LDRRNN         (0b00000001)
#define CODE_LDIXDR         (0b01110000)   
#define CODE_LDIYDR         (0b01110000) 
#define CODE_DD_LDRIXD      (0b1101110101000110)
#define CODE_FD_LDRIYD      (0b1111110101000110)    
#define CODE_DD_LDIXDR      (0b1111110101110000)    
#define CODE_FD_LDIYDR      (0b1101110101110000)   

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
#define MASK_LDBCA          (0b11111111)
#define MASK_LDDEA          (0b11111111)
#define MASK_LDNNA          (0b11111111)
#define MASK_LDRRNN         (0b11001111)
#define MASK_LDRIXD         (0b11000111)
#define MASK_LDRIYD         (0b11000111)
#define MASK_LDIXDR         (0b11111000)
#define MASK_LDIYDR         (0b11111000)

#define ONE_BYTE            (2)
#define TWO_BYTES           (4)
#define THREE_BYTES         (6)
#define FOUR_BYTES          (8)

#define SIZE_1_BYTE         (8)
#define SIZE_2_BYTES        (16)
#define SIZE_3_BYTES        (24)
#define SIZE_4_BYTES        (32)

#define FIRST_LOWEST_BYTE   (0x000000FF)
#define SECOND_LOWEST_BYTE  (0x0000FF00)
#define THIRD_LOWEST_BYTE   (0x00FF0000)
#define FOURTH_LOWEST_BYTE  (0xFF000000)

#define REGA                (0b111)
#define REGB                (0b000)
#define REGC                (0b001)
#define REGD                (0b010)
#define REGE                (0b011)
#define REGH                (0b100)
#define REGL                (0b101)

#define REGBC               (0b1000)
#define REGDE               (0b1001)
#define REGHL               (0b1010)
#define REGSP               (0b1011)
#define REGAF               (0b1100)
#define REGIX               (0b1101)
#define REGIY               (0b1110)

#define REGIBC              (0b11000)
#define REGIDE              (0b11001)
#define REGIHL              (0b11010)
#define REGISP              (0b11011)
#define REGIAF              (0b11100)

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
