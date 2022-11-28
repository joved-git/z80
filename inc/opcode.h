#ifndef OPCODE_H
#define OPCODE_H

#define ONE_BYTE                (2)
#define TWO_BYTES               (4)
#define THREE_BYTES             (6)
#define FOUR_BYTES              (8)

#define SIZE_1_BYTE             (8)
#define SIZE_2_BYTES            (16)
#define SIZE_3_BYTES            (24)
#define SIZE_4_BYTES            (32)

#define FIRST_LOWEST_BYTE           (0x000000FF)
#define SECOND_LOWEST_BYTE          (0x0000FF00)
#define THIRD_LOWEST_BYTE           (0x00FF0000)
#define FOURTH_LOWEST_BYTE          (0xFF000000)
#define FIRST_TWO_LOWEST_BYTES      (0x0000FFFF)
#define FIRST_THREE_LOWEST_BYTES    (0x00FFFFFF)

#define EXTRACT(a, d, l)        (a>>d & ~(((uint8_t) 0b111111111)<<l)) 
#define PUSHBIT(a, b, l)        (a=a|(b<<l))
#define SIGN(a)                 (a>>7)
#define ZERO(a)                 (a==0?1:0)
#define EVEN(a)                 (BIT(a, 0)==0?1:0)     
#define ODD(a)                  (BIT(a, 0)==1?1:0)
#define BIT(a, n)               EXTRACT (a, n, 1)

#define NATURAL_CODE_LENGTH(c)  natural_code_length[c]
#define CB_CODE_LENGTH(c)       (cb_code_length[c & FIRST_LOWEST_BYTE]+ONE_BYTE)
#define ED_CODE_LENGTH(c)       (ed_code_length[c & FIRST_LOWEST_BYTE]+ONE_BYTE)
#define DD_CODE_LENGTH(c)       (dd_code_length[c & FIRST_LOWEST_BYTE]+ONE_BYTE)
#define FD_CODE_LENGTH(c)       (fd_code_length[c & FIRST_LOWEST_BYTE]+ONE_BYTE)
#define DDCB_CODE_LENGTH(c)     (ddcb_code_length[c]+TWO_BYTES)
#define FDCB_CODE_LENGTH(c)     (fdcb_code_length[c]+TWO_BYTES)

#define ALT_CODE_CB             (0xCB)
#define ALT_CODE_ED             (0xED)
#define ALT_CODE_DD             (0xDD)
#define ALT_CODE_FD             (0xFD)

/* Natural instruction codes    */
#define CODE_NO_INSTRUCTION (0b11001011)                // 0xDE
#define CODE_NOP            (0b00000000)                // 0X00
#define CODE_LDBCA          (0b00000010)                // 0x02
#define CODE_RLCA           (0b00000111)                // 0x07
#define CODE_EXAFAF         (0b00001000)                // 0x08
#define CODE_LDABC          (0b00001010)                // 0x0A
#define CODE_RRCA           (0b00001111)                // 0x0F
#define CODE_LDDEA          (0b00010010)                // 0x12
#define CODE_LDADE          (0b00011010)                // 0x1A
#define CODE_LDNNHL         (0b00100010)                // 0x22
#define CODE_LDHLNN         (0b00101010)                // 0x2A
#define CODE_LDNNA          (0b00110010)                // 0x32
#define CODE_LDHLN          (0b00110110)                // 0x36
#define CODE_LDANN          (0b00111010)                // 0x3A
#define CODE_EXX            (0b11011001)                // 0xD9
#define CODE_LDRR           (0b01000000)                
#define CODE_LDRHL          (0b01000110)
#define CODE_LDHLR          (0b01110000)
#define CODE_LDRN           (0b00000110)
#define CODE_HALT           (0b01110110)
#define CODE_LDRRNN         (0b00000001)
#define CODE_INCR           (0b00000100)
#define CODE_INCRR          (0b00000011)
#define CODE_PUSHQQ         (0b11000101)
#define CODE_DECR           (0b00000101)
#define CODE_DECRR          (0b00001011)
#define CODE_ADDAR          (0b10000000)

/* CB instructions codes    */
#define CODE_CB_RLCR        (0b1100101100000000)                    // RLC r
#define CODE_CB_RRCR        (0b1100101100001000)                    // RRC r
#define CODE_CB_RLCHL       (0b1100101100000110)        // 0xCB06   // RLC (HL) 

/* ED instruction codes     */
#define CODE_ED_LDNNRR      (0b1110110101000011)        //          // LD (nn),rr
#define CODE_ED_LDDDNN      (0b1110110101001011)        //          // LD rr,(nn)

/* DD instruction codes     */
#define CODE_DD_LDIXNN      (0b1101110100100001)        // 0xDD21   // LD IX,nn      
#define CODE_DD_LDNNIX      (0b1101110100100010)        // 0xDD22   // LD (nn),IX
#define CODE_DD_LDIXANN     (0b1101110100101010)        // 0xDD2A   // LD IX,(nn)
#define CODE_DD_LDRIXD      (0b1101110101000110)                    // LD r,(IX+d)
#define CODE_DD_LDIXDR      (0b1101110101110000)                    // LD (IX+d),r      

/* FD instruction codes     */
#define CODE_FD_LDIYNN      (0b1111110100100001)        // 0xFD21   // LD IY,nn 
#define CODE_FD_LDNNIY      (0b1111110100100010)        // 0xFD22   // LD (nn),IY
#define CODE_FD_LDIYANN     (0b1111110100101010)        // 0xFD2A   // LD YX,(nn)
#define CODE_FD_LDRIYD      (0b1111110101000110)        //          // LD r,(IY+d)
#define CODE_FD_LDIYDR      (0b1111110101110000)                    // LD (IY+d),r

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
#define MASK_LDHLNN         (0b11111111)
#define MASK_LDNNHL         (0b11111111)
#define MASK_INCR           (0b11000111)
#define MASK_INCRR          (0b11001111)
#define MASK_PUSHQQ         (0b11001111)
#define MASK_DECR           (0b11000111)
#define MASK_DECRR          (0b11000111)
#define MASK_RLCA           (0b11111111)
#define MASK_EXAFAF         (0b11111111)
#define MASK_EXX            (0b11111111)
#define MASK_RRCA           (0b11111111)
#define MASK_ADDAR          (0b11111000)

/* 16-bit masks */
#define MASK_RLCR           (0b1111111111111000)
#define MASK_RRCR           (0b1111111111111000)
#define MASK_RLCHL          (0b1111111111111111)
#define MASK_LDNNRR         (0b1111111111001111)
#define MASK_LDDDNN         (0b1111111111001111)
#define MASK_LDIXNN         (0b1111111111111111)
#define MASK_LDIXANN        (0b1111111111111111)
#define MASK_LDRIXD         (0b1111111111000111)
#define MASK_LDIYNN         (0b1111111111111111)
#define MASK_LDIYANN        (0b1111111111111111)
#define MASK_LDRIYD         (0b1111111111000111)
#define MASK_LDIXDR         (0b1111111111111000)
#define MASK_LDIYDR         (0b1111111111111000)
#define MASK_LDNNIX         (0b1111111111111111)
#define MASK_LDNNIY         (0b1111111111111111)

/* 8-bit registers binary codes */
#define REGA                (0b111)
#define REGB                (0b000)
#define REGC                (0b001)
#define REGD                (0b010)
#define REGE                (0b011)
#define REGH                (0b100)
#define REGL                (0b101)

/* 16-bit registers binary codes */
#define REGBC               (0b1000)
#define REGDE               (0b1001)
#define REGHL               (0b1010)
#define REGSP               (0b1011)
#define REGAF               (0b1100)
#define REGIX               (0b1101)
#define REGIY               (0b1110)

#define REGBCP              (0b10000)
#define REGDEP              (0b10001)
#define REGHLP              (0b10010)
#define REGAFP              (0b10100)

#define REGIBC              (0b11000)
#define REGIDE              (0b11001)
#define REGIHL              (0b11010)
#define REGISP              (0b11011)
#define REGIAF              (0b11100)

#define INTP_DISPLAY        (0)
#define INTP_EXECUTE        (1)
#define INTP_DETECT         (2)
#define INTP_EXECUTE_BLIND  (3)

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
