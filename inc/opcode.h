#ifndef OPCODE_H
#define OPCODE_H

#define ONE_BYTE                    (2)
#define TWO_BYTES                   (4)
#define THREE_BYTES                 (6)
#define FOUR_BYTES                  (8)
    
#define SIZE_1_BYTE                 (8)
#define SIZE_2_BYTES                (16)
#define SIZE_3_BYTES                (24)
#define SIZE_4_BYTES                (32)

#define FIRST_LOWEST_BYTE           (0x000000FF)
#define SECOND_LOWEST_BYTE          (0x0000FF00)
#define THIRD_LOWEST_BYTE           (0x00FF0000)
#define FOURTH_LOWEST_BYTE          (0xFF000000)
#define FIRST_TWO_LOWEST_BYTES      (0x0000FFFF)
#define FIRST_THREE_LOWEST_BYTES    (0x00FFFFFF)

/* Bit operations    */
#define EXTRACT(a, d, l)        (a>>d & ~(((uint8_t) 0b111111111)<<l)) 
#define PUSHBIT(a, b, l)        (b==0?a=a&~(1<<l):a=a|(b<<l))
#define SIGN(a)                 (a>>7)
#define ZERO(a)                 (a==0?1:0)
#define EVEN(a)                 (BIT(a, 0)==0?1:0)     
#define ODD(a)                  (BIT(a, 0)==1?1:0)
#define BIT(a, n)               EXTRACT (a, n, 1)
#define REVERT(a)               a=((a&FIRST_LOWEST_BYTE)<<8) | ((a&SECOND_LOWEST_BYTE)>>8);

#define NATURAL_CODE_LENGTH(c)  natural_code_length[c]
#define CB_CODE_LENGTH(c)       (cb_code_length[c & FIRST_LOWEST_BYTE]+ONE_BYTE)
#define ED_CODE_LENGTH(c)       (ed_code_length[c & FIRST_LOWEST_BYTE]+ONE_BYTE)
#define DD_CODE_LENGTH(c)       (dd_code_length[c & FIRST_LOWEST_BYTE]+ONE_BYTE)
#define FD_CODE_LENGTH(c)       (fd_code_length[c & FIRST_LOWEST_BYTE]+ONE_BYTE)
#define DDCB_CODE_LENGTH(c)     (ddcb_code_length[c & FIRST_LOWEST_BYTE]+TWO_BYTES)
#define FDCB_CODE_LENGTH(c)     (fdcb_code_length[c & FIRST_LOWEST_BYTE]+TWO_BYTES)

#define ALT_CODE_CB             (0xCB)
#define ALT_CODE_ED             (0xED)
#define ALT_CODE_DD             (0xDD)
#define ALT_CODE_FD             (0xFD)

/* Natural instruction codes    */
#define CODE_NO_INSTRUCTION (0b11001011)                // 0xDE
#define CODE_NOP            (0b00000000)                // 0X00     // NOP
#define CODE_LDBCA          (0b00000010)                // 0x02     // LD (BC),A
#define CODE_RLCA           (0b00000111)                // 0x07     // RLCA
#define CODE_EXAFAF         (0b00001000)                // 0x08     // EX AF,AF'
#define CODE_LDABC          (0b00001010)                // 0x0A     // LD A,(BC)
#define CODE_RRCA           (0b00001111)                // 0x0F     // RRCA
#define CODE_DJNZ           (0b00010000)                // 0x10     // DJNZ e
#define CODE_LDDEA          (0b00010010)                // 0x12     // LD (DE),A
#define CODE_RLA            (0b00010111)                // 0x17     // RLA
#define CODE_JRE            (0b00011000)                // 0x18     // JRE
#define CODE_LDADE          (0b00011010)                // 0x1A     // LD A,(DE)
#define CODE_RRA            (0b00011111)                // 0x1F     // RRA
#define CODE_LDNNHL         (0b00100010)                // 0x22     // LD (nn),HL
#define CODE_LDHLNN         (0b00101010)                // 0x2A     // LD HL,(nn)
#define CODE_CPL            (0b00101111)                // 0x2F     // CPL
#define CODE_LDNNA          (0b00110010)                // 0x32     // LD (nn),A
#define CODE_INCHL          (0b00110100)                // 0x34     // INC (HL)
#define CODE_DECHL          (0b00110101)                // 0x35     // DEC (HL)
#define CODE_LDHLN          (0b00110110)                // 0x36     // LD (HL),n
#define CODE_LDANN          (0b00111010)                // 0x3A     // LD A,(nn)
#define CODE_ADDAHL         (0b10000110)                // 0x86     // ADD A,(HL)
#define CODE_ADCAHL         (0b10001110)                // 0x8E     // ADC A,(HL)
#define CODE_CALLCCNN       (0b11000100)                            // CALL cc,nn
#define CODE_JPNN           (0b11000011)                // 0xC3     / JP nn
#define CODE_CALLNN         (0b11001101)                // 0xCD     // CALL nn
#define CODE_EXX            (0b11011001)                // 0xD9     // EXX
#define CODE_LDSPHL         (0b11111001)                // 0xF9     // LD SP,HL
#define CODE_LDRR           (0b01000000)                
#define CODE_LDRHL          (0b01000110)
#define CODE_LDHLR          (0b01110000)
#define CODE_LDRN           (0b00000110)
#define CODE_HALT           (0b01110110)
#define CODE_LDRRNN         (0b00000001)
#define CODE_INCR           (0b00000100)
#define CODE_INCRR          (0b00000011)                            // INC rr
#define CODE_PUSHQQ         (0b11000101)                            // PUSH qq
#define CODE_POPQQ          (0b11000001)                            // POP qq
#define CODE_DECR           (0b00000101)
#define CODE_ADDAR          (0b10000000)
#define CODE_ADCAR          (0b10001000)
#define CODE_ADDHLRR        (0b00001001)
#define CODE_DECRR          (0b00001011)

/* CB instructions codes    */
#define CODE_CB_RLCR        (0b1100101100000000)                    // RLC r
#define CODE_CB_RRCR        (0b1100101100001000)                    // RRC r
#define CODE_CB_RLCHL       (0b1100101100000110)        // 0xCB06   // RLC (HL) 
#define CODE_CB_RRCHL       (0b1100101100001110)        // 0xCB0E   // RRC (HL) 
#define CODE_CB_RLHL        (0b1100101100010110)        // 0xCB16   // RL (HL)
#define CODE_CB_RRHL        (0b1100101100011110)        // 0xCB1E   // RR (HL)
#define CODE_CB_SLAHL       (0b1100101100100110)        // 0xCB26   // SLA (HL)
#define CODE_CB_SRAHL       (0b1100101100101110)        // 0xCB2E   // SRA (HL)
#define CODE_CB_SRLHL       (0b1100101100111110)        // 0xCB3E   // SRL (HL)
#define CODE_CB_BITBR       (0b1100101101000000)                    // BIT b,r
#define CODE_CB_BITBHL      (0b1100101101000110)                    // BIT b,(HL)
#define CODE_CB_RESBR       (0b1100101110000000)                    // RES b,r
#define CODE_CB_SETBR       (0b1100101111000000)                    // SET b,r
#define CODE_CB_RESBHL      (0b1100101110000110)                    // RES b,(HL)
#define CODE_CB_SETBHL      (0b1100101111000110)                    // SET b,(HL)
#define CODE_CB_RLR         (0b1100101100010000)                    // RL r
#define CODE_CB_RRR         (0b1100101100011000)                    // RR r
#define CODE_CB_SLAR        (0b1100101100100000)                    // SLA r
#define CODE_CB_SRAR        (0b1100101100101000)                    // SRA r
#define CODE_CB_SRLR        (0b1100101100111000)                    // SRL r

/* ED instruction codes     */
#define CODE_ED_LDNNRR      (0b1110110101000011)        //          // LD (nn),rr
#define CODE_ED_LDDDNN      (0b1110110101001011)        //          // LD rr,(nn)
#define CODE_ED_NEG         (0b1110110101000100)        // 0xED44   // NEG
#define CODE_ED_RLD         (0b1110110101101111)        // 0xED6F   // RLD
#define CODE_ED_RRD         (0b1110110101100111)        // 0xED67   // RRD

/* DD instruction codes     */
#define CODE_DD_LDIXNN      (0b1101110100100001)        // 0xDD21   // LD IX,nn      
#define CODE_DD_LDNNIX      (0b1101110100100010)        // 0xDD22   // LD (nn),IX
#define CODE_DD_LDIXANN     (0b1101110100101010)        // 0xDD2A   // LD IX,(nn)
#define CODE_DD_LDIXDN      (0b1101110100110110)        // 0xDD36   // LD (IX+d),n
#define CODE_DD_POPIX       (0b1101110111100001)        // 0xDDE1   // POP IX
#define CODE_DD_PUSHIX      (0b1101110111100101)        // 0xDDE5   // PUSH IX
#define CODE_DD_LDSPIX      (0b1101110111111001)        // 0xDDF9   // LD SP,IX
#define CODE_DD_LDRIXD      (0b1101110101000110)                    // LD r,(IX+d)
#define CODE_DD_LDIXDR      (0b1101110101110000)                    // LD (IX+d),r     
#define CODE_DD_ADDIXPP     (0b1101110100001001)                    // LD IX,pp

/* FD instruction codes     */
#define CODE_FD_LDIYNN      (0b1111110100100001)        // 0xFD21   // LD IY,nn
#define CODE_FD_LDNNIY      (0b1111110100100010)        // 0xFD22   // LD (nn),IY
#define CODE_FD_LDIYANN     (0b1111110100101010)        // 0xFD2A   // LD YX,(nn)
#define CODE_FD_LDIYDN      (0b1111110100110110)        // 0xFD36   // LD (IX+d),n
#define CODE_FD_POPIY       (0b1111110111100001)        // 0xFDE1   // POP IY
#define CODE_FD_PUSHIY      (0b1111110111100101)        // 0xFDE5   // PUSH IY
#define CODE_FD_LDSPIY      (0b1111110111111001)        // 0xFDF9   // LD SP,IY
#define CODE_FD_LDRIYD      (0b1111110101000110)        //          // LD r,(IY+d)
#define CODE_FD_LDIYDR      (0b1111110101110000)                    // LD (IY+d),r
#define CODE_FD_ADDIYQQ     (0b1111110100001001)                    // LD IY,qq

/* DDCB instruction codes   */
#define CODE_DDCB_RLCIXD    (0b11011101110010110000000000000110)     // RLC (IX+d)
#define CODE_DDCB_RRCIXD    (0b11011101110010110000000000001110)     // RRC (IX+d)
#define CODE_DDCB_RLIXD     (0b11011101110010110000000000010110)     // RL (IX+d)
#define CODE_DDCB_RRIXD     (0b11011101110010110000000000011110)     // RR (IX+d)
#define CODE_DDCB_SLAIXD    (0b11011101110010110000000000100110)     // SLA (IX+d)
#define CODE_DDCB_SRAIXD    (0b11011101110010110000000000101110)     // SRA (IX+d)
#define CODE_DDCB_SRLIXD    (0b11011101110010110000000000111110)     // SRL (IX+d)
#define CODE_DDCB_BITBIXD   (0b11011101110010110000000001000110)     // BIT b,(IX+d)
#define CODE_DDCB_SETBIXD   (0b11011101110010110000000011000110)     // SET b,(IX+d)
#define CODE_DDCB_RESBIXD   (0b11011101110010110000000010000110)     // RES b,(IX+d)

/* FDCB instruction codes   */
#define CODE_FDCB_RLCIYD    (0b11111101110010110000000000000110)     // RLC (IY+d)
#define CODE_FDCB_RRCIYD    (0b11111101110010110000000000001110)     // RRC (IY+d)
#define CODE_FDCB_RLIYD     (0b11111101110010110000000000010110)     // RL (IY+d)
#define CODE_FDCB_RRIYD     (0b11111101110010110000000000011110)     // RR (IY+d)
#define CODE_FDCB_SLAIYD    (0b11111101110010110000000000100110)     // SLA (IY+d)
#define CODE_FDCB_SRAIYD    (0b11111101110010110000000000101110)     // SRA (IY+d)
#define CODE_FDCB_SRLIYD    (0b11111101110010110000000000111110)     // SRL (IY+d)
#define CODE_FDCB_BITBIYD   (0b11111101110010110000000001000110)     // BIT b,(IY+d)
#define CODE_FDCB_SETBIYD   (0b11111101110010110000000011000110)     // SET b,(IY+d)
#define CODE_FDCB_RESBIYD   (0b11111101110010110000000010000110)     // RES b,(IY+d)

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
#define MASK_POPQQ          (0b11001111)
#define MASK_DECR           (0b11000111)
#define MASK_RLCA           (0b11111111)
#define MASK_EXAFAF         (0b11111111)
#define MASK_EXX            (0b11111111)
#define MASK_RRCA           (0b11111111)
#define MASK_ADDAR          (0b11111000)
#define MASK_ADCAR          (0b11111000)
#define MASK_ADDAHL         (0b11111111)
#define MASK_ADCAHL         (0b11111111)
#define MASK_ADDHLRR        (0b11001111)
#define MASK_CALLNN         (0b11111111)
#define MASK_CALLCCNN       (0b11000111)
#define MASK_DECRR          (0b11001111)
#define MASK_DECHL          (0b11111111)
#define MASK_INCHL          (0b11111111)
#define MASK_RLA            (0b11111111)
#define MASK_CPL            (0b11111111)
#define MASK_RRA            (0b11111111)
#define MASK_JPNN           (0b11111111)
#define MASK_JRE            (0b11111111)
#define MASK_LDSPHL         (0b11111111)
#define MASK_DJNZ           (0b11111111)

/* 16-bit masks */
#define MASK_RLCR           (0b1111111111111000)
#define MASK_RRCR           (0b1111111111111000)
#define MASK_RLCHL          (0b1111111111111111)
#define MASK_RRCHL          (0b1111111111111111)
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
#define MASK_ADDIXPP        (0b1111111111001111)
#define MASK_ADDIYQQ        (0b1111111111001111)
#define MASK_BITBR          (0b1111111111000000)
#define MASK_BITBHL         (0b1111111111000111)
#define MASK_RESBR          (0b1111111111000000)
#define MASK_SETBR          (0b1111111111000000)
#define MASK_RESBHL         (0b1111111111000111)
#define MASK_SETBHL         (0b1111111111000111)
#define MASK_RLR            (0b1111111111111000)
#define MASK_RLHL           (0b1111111111111111)
#define MASK_NEG            (0b1111111111111111)
#define MASK_RRR            (0b1111111111111000)
#define MASK_RRHL           (0b1111111111111111)
#define MASK_SRAR           (0b1111111111111000)
#define MASK_SLAR           (0b1111111111111000)
#define MASK_SRLR           (0b1111111111111000)
#define MASK_SRAHL          (0b1111111111111111)
#define MASK_SLAHL          (0b1111111111111111)
#define MASK_SRLHL          (0b1111111111111111)
#define MASK_LDIXDN         (0b1111111111111111)
#define MASK_LDIYDN         (0b1111111111111111)
#define MASK_LDSPIX         (0b1111111111111111)
#define MASK_LDSPIY         (0b1111111111111111)
#define MASK_PUSHIX         (0b1111111111111111)
#define MASK_PUSHIY         (0b1111111111111111)
#define MASK_POPIX          (0b1111111111111111)
#define MASK_POPIY          (0b1111111111111111)
#define MASK_RLD            (0b1111111111111111)
#define MASK_RRD            (0b1111111111111111)

/* 32-bit mask  */
#define MASK_RLCIXD         (0b11111111111111110000000011111111)
#define MASK_RLCIYD         (0b11111111111111110000000011111111)
#define MASK_RRCIXD         (0b11111111111111110000000011111111)
#define MASK_RRCIYD         (0b11111111111111110000000011111111)
#define MASK_RLIXD          (0b11111111111111110000000011111111)
#define MASK_RLIYD          (0b11111111111111110000000011111111)
#define MASK_RRIXD          (0b11111111111111110000000011111111)
#define MASK_RRIYD          (0b11111111111111110000000011111111)
#define MASK_SLAIXD         (0b11111111111111110000000011111111)
#define MASK_SLAIYD         (0b11111111111111110000000011111111)
#define MASK_SRAIXD         (0b11111111111111110000000011111111)
#define MASK_SRAIYD         (0b11111111111111110000000011111111)
#define MASK_SRLIXD         (0b11111111111111110000000011111111)
#define MASK_SRLIYD         (0b11111111111111110000000011111111)
#define MASK_BITBIXD        (0b11111111111111110000000011000111)
#define MASK_BITBIYD        (0b11111111111111110000000011000111)
#define MASK_SETBIXD        (0b11111111111111110000000011000111)
#define MASK_SETBIYD        (0b11111111111111110000000011000111)
#define MASK_RESBIXD        (0b11111111111111110000000011000111)
#define MASK_RESBIYD        (0b11111111111111110000000011000111)

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
#define REGPC               (0b1111)

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

/* 3-bit number in binary   */
#define BINARY_0            (0b000)
#define BINARY_1            (0b001)
#define BINARY_2            (0b010)
#define BINARY_3            (0b011)
#define BINARY_4            (0b100)
#define BINARY_5            (0b101)
#define BINARY_6            (0b110)
#define BINARY_7            (0b111)

/* 3-bit conditions binary codes */
#define CONDNZ              (0b000)
#define CONDZ               (0b001)
#define CONDNC              (0b010)
#define CONDC               (0b011)
#define CONDPO              (0b100)
#define CONDPE              (0b101)
#define CONDP               (0b110)
#define CONDM               (0b111)

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
