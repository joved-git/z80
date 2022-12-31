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
#define EVEN(a)                 (calcParity(a)) 
#define ODD (a)                 (!EVEN(a))
#define BIT(a, n)               EXTRACT (a, n, 1)
#define REVERT(a)               a=((a & FIRST_LOWEST_BYTE)<<8) | ((a & SECOND_LOWEST_BYTE)>>8);
#define UPPER(a)                (a>>4)
#define LOWER(a)                (a & 0x0F)

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
#define CODE_JRNZE          (0b00100000)                // 0x20     // JRNZE
#define CODE_LDNNHL         (0b00100010)                // 0x22     // LD (nn),HL
#define CODE_DAA            (0b00100111)                // 0x27     // DAA
#define CODE_JRZE           (0b00101000)                // 0x28     // JRZE
#define CODE_LDHLNN         (0b00101010)                // 0x2A     // LD HL,(nn)
#define CODE_CPL            (0b00101111)                // 0x2F     // CPL
#define CODE_JRNCE          (0b00110000)                // 0x30     // JRNCE
#define CODE_LDNNA          (0b00110010)                // 0x32     // LD (nn),A
#define CODE_INCHL          (0b00110100)                // 0x34     // INC (HL)
#define CODE_DECHL          (0b00110101)                // 0x35     // DEC (HL)
#define CODE_LDHLN          (0b00110110)                // 0x36     // LD (HL),n
#define CODE_SCF            (0b00110111)                // 0x37     // SCF
#define CODE_JRCE           (0b00111000)                // 0x38     // JRCE
#define CODE_LDANN          (0b00111010)                // 0x3A     // LD A,(nn)
#define CODE_CCF            (0b00111111)                // 0x3F     // CCF
#define CODE_ADDAHL         (0b10000110)                // 0x86     // ADD A,(HL)
#define CODE_ADCAHL         (0b10001110)                // 0x8E     // ADC A,(HL)
#define CODE_SUBAHL         (0b10010110)                // 0x96     // SUB A,(HL)
#define CODE_SBCAHL         (0b10011110)                // 0x9E     // SBC A,(HL)
#define CODE_ANDHL          (0b10100110)                // 0xA6     // AND (HL)
#define CODE_XORHL          (0b10101110)                // 0xAE     // XOR (HL)
#define CODE_ORHL           (0b10110110)                // 0xB6     // OR (HL)
#define CODE_CPHL           (0b10111110)                // 0xBE     // CP (HL)
#define CODE_JPNN           (0b11000011)                // 0xC3     // JP nn
#define CODE_ADDAN          (0b11000110)                // 0xC6     // ADD A,n
#define CODE_RET            (0b11001001)                // 0xC9     // RET
#define CODE_CALLNN         (0b11001101)                // 0xCD     // CALL nn
#define CODE_ADCAN          (0b11001110)                // 0xCE     // ADC A,n
#define CODE_SUBAN          (0b11010110)                // 0xD6     // SUB A,n
#define CODE_SBCAN          (0b11011110)                // 0xDE     // SBC A,n
#define CODE_EXSPHL         (0b11100011)                // 0xE3     // EX (SP),HL
#define CODE_ANDN           (0b11100110)                // 0xE6     // AND n 
#define CODE_JPHL           (0b11101001)                // 0xE9     // JP (HL)
#define CODE_EXDEHL         (0b11101011)                // 0xEB     // EX DE,HL
#define CODE_XORN           (0b11101110)                // 0xEE     // XOR n
#define CODE_EXX            (0b11011001)                // 0xD9     // EXX
#define CODE_DI             (0b11110011)                // 0xF3     // DI
#define CODE_ORN            (0b11110110)                // 0xF6     // OR n
#define CODE_EI             (0b11111011)                // 0xFB     // EI
#define CODE_CPN            (0b11111110)                // 0xFE     // CP n
#define CODE_LDSPHL         (0b11111001)                // 0xF9     // LD SP,HL
#define CODE_CALLCCNN       (0b11000100)                            // CALL cc,nn
#define CODE_LDRR           (0b01000000)                            // LD r,r'       
#define CODE_LDRHL          (0b01000110)                            // LD r,(HL)
#define CODE_LDHLR          (0b01110000)                            // LD (HL),r
#define CODE_LDRN           (0b00000110)                            // LD r,n
#define CODE_HALT           (0b01110110)                            // HALT
#define CODE_LDRRNN         (0b00000001)                            // LD rr,nn
#define CODE_INCR           (0b00000100)                            // INC r
#define CODE_INCRR          (0b00000011)                            // INC rr
#define CODE_PUSHQQ         (0b11000101)                            // PUSH qq
#define CODE_POPQQ          (0b11000001)                            // POP qq
#define CODE_DECR           (0b00000101)                            // DEC r
#define CODE_ADDAR          (0b10000000)                            // ADD A,r
#define CODE_ADCAR          (0b10001000)                            // ADC A,r
#define CODE_ADDHLRR        (0b00001001)                            // ADD HL,rr
#define CODE_DECRR          (0b00001011)                            // DEC rr
#define CODE_ANDR           (0b10100000)                            // AND r
#define CODE_ORR            (0b10110000)                            // OR r
#define CODE_XORR           (0b10101000)                            // XOR r
#define CODE_CPR            (0b10111000)                            // CP r
#define CODE_JPCCNN         (0b11000010)                            // JP cc,nn
#define CODE_SUBAR          (0b10010000)                            // SUB A,r
#define CODE_SBCAR          (0b10011000)                            // SBC A,r
#define CODE_RETCC          (0b11000000)                            // RET cc
#define CODE_RSTP           (0b11000111)                            // RST p

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
#define CODE_ED_LDIA        (0b1110110101000111)        // 0xED47   // LD I,A
#define CODE_ED_LDRA        (0b1110110101001111)        // 0xED4F   // LD R,A
#define CODE_ED_LDAI        (0b1110110101010111)        // 0xED57   // LD A,I
#define CODE_ED_LDAR        (0b1110110101011111)        // 0xED5F   // LD A,R
#define CODE_ED_RLD         (0b1110110101101111)        // 0xED6F   // RLD
#define CODE_ED_RRD         (0b1110110101100111)        // 0xED67   // RRD
#define CODE_ED_LDI         (0b1110110110100000)        // 0xEDA0   // LDI
#define CODE_ED_LDIR        (0b1110110110110000)        // 0xEDB0   // LDIR
#define CODE_ED_LDD         (0b1110110110101000)        // 0xEDA8   // LDD
#define CODE_ED_LDDR        (0b1110110110111000)        // 0xEDB8   // LDDR
#define CODE_ED_CPI         (0b1110110110100001)        // 0xEDA1   // CPI
#define CODE_ED_CPIR        (0b1110110110110001)        // 0xEDB1   // CPIR
#define CODE_ED_CPD         (0b1110110110101001)        // 0xEDA9   // CPD
#define CODE_ED_CPDR        (0b1110110110111001)        // 0xEDB9   // CPDR
#define CODE_ED_ADCHLRR     (0b1110110101001010)                    // ADC HL,rr
#define CODE_ED_SBCHLRR     (0b1110110101000010)                    // SBC HL,rr


/* DD instruction codes     */
#define CODE_DD_LDIXNN      (0b1101110100100001)        // 0xDD21   // LD IX,nn      
#define CODE_DD_LDNNIX      (0b1101110100100010)        // 0xDD22   // LD (nn),IX
#define CODE_DD_INCIX       (0b1101110100100011)        // 0xDD22   // INC IX
#define CODE_DD_LDIXANN     (0b1101110100101010)        // 0xDD2A   // LD IX,(nn)
#define CODE_DD_DECIX       (0b1101110100101011)        // 0xDD2B   // DEC IX
#define CODE_DD_INCIXD      (0b1101110100110100)        // 0xDD34   // INC (IX+d)
#define CODE_DD_DECIXD      (0b1101110100110101)        // 0xDD35   // DEC (IX+d)
#define CODE_DD_LDIXDN      (0b1101110100110110)        // 0xDD36   // LD (IX+d),n
#define CODE_DD_ADDAIXD     (0b1101110110000110)        // 0xDD86   // ADD A,(IX+d)
#define CODE_DD_ADCAIXD     (0b1101110110001110)        // 0xDD8E   // ADC A,(IX+d)
#define CODE_DD_SUBAIXD     (0b1101110110010110)        // 0xDD96   // SUB A,(IX+d)
#define CODE_DD_SBCAIXD     (0b1101110110011110)        // 0xDD9E   // SBC A,(IX+d)
#define CODE_DD_ANDIXD      (0b1101110110100110)        // 0XDDA6   // AND (IX+d)
#define CODE_DD_XORIXD      (0b1101110110101110)        // 0XDDAE   // XOR (IX+d)
#define CODE_DD_ORIXD       (0b1101110110110110)        // 0XDDB6   // OR (IX+d)
#define CODE_DD_CPIXD       (0b1101110110111110)        // 0XDDBE   // CP (IX+d)
#define CODE_DD_POPIX       (0b1101110111100001)        // 0xDDE1   // POP IX
#define CODE_DD_EXSPIX      (0b1101110111100011)        // 0xDDE3   // EX (SH),IX
#define CODE_DD_PUSHIX      (0b1101110111100101)        // 0xDDE5   // PUSH IX
#define CODE_DD_JPIX        (0b1101110111101001)        // 0XDDE9   // JP (IX)
#define CODE_DD_LDSPIX      (0b1101110111111001)        // 0xDDF9   // LD SP,IX
#define CODE_DD_LDRIXD      (0b1101110101000110)                    // LD r,(IX+d)
#define CODE_DD_LDIXDR      (0b1101110101110000)                    // LD (IX+d),r     
#define CODE_DD_ADDIXPP     (0b1101110100001001)                    // ADD IX,pp

/* FD instruction codes     */
#define CODE_FD_LDIYNN      (0b1111110100100001)        // 0xFD21   // LD IY,nn
#define CODE_FD_LDNNIY      (0b1111110100100010)        // 0xFD22   // LD (nn),IY
#define CODE_FD_INCIY       (0b1111110100100011)        // 0xFD22   // INC IY
#define CODE_FD_LDIYANN     (0b1111110100101010)        // 0xFD2A   // LD YX,(nn)
#define CODE_FD_DECIY       (0b1111110100101011)        // 0xFD2B   // DEC IY
#define CODE_FD_INCIYD      (0b1111110100110100)        // 0xFD34   // INC (IY+d)F
#define CODE_FD_DECIYD      (0b1111110100110101)        // 0xFD35   // DEC (IY+d)
#define CODE_FD_LDIYDN      (0b1111110100110110)        // 0xFD36   // LD (IX+d),n
#define CODE_FD_ADDAIYD     (0b1111110110000110)        // 0xFD86   // ADD A,(IY+d)
#define CODE_FD_ADCAIYD     (0b1111110110001110)        // 0xFD8E   // ADC A,(IY+d)
#define CODE_FD_SUBAIYD     (0b1111110110010110)        // 0xFD96   // SUB A,(IY+d)
#define CODE_FD_SBCAIYD     (0b1111110110011110)        // 0xFD9E   // SBC A,(IY+d)
#define CODE_FD_ANDIYD      (0b1111110110100110)        // 0XFDA6   // AND (IY+d)
#define CODE_FD_XORIYD      (0b1111110110101110)        // 0XFDAE   // XOR (IX+d)
#define CODE_FD_ORIYD       (0b1111110110110110)        // 0XFDB6   // OR (IY+d)
#define CODE_FD_CPIYD       (0b1111110110111110)        // 0XFDBE   // CP (IY+d)
#define CODE_FD_POPIY       (0b1111110111100001)        // 0xFDE1   // POP IY
#define CODE_FD_EXSPIY      (0b1111110111100011)        // 0xFDE3   // EX (SH),IY
#define CODE_FD_PUSHIY      (0b1111110111100101)        // 0xFDE5   // PUSH IY
#define CODE_FD_JPIY        (0b1111110111101001)        // 0XFDE9   // JP (IY)
#define CODE_FD_LDSPIY      (0b1111110111111001)        // 0xFDF9   // LD SP,IY
#define CODE_FD_LDRIYD      (0b1111110101000110)                    // LD r,(IY+d)
#define CODE_FD_LDIYDR      (0b1111110101110000)                    // LD (IY+d),r
#define CODE_FD_ADDIYQQ     (0b1111110100001001)                    // LD IY,qq

/* DDCB instruction codes   */
#define CODE_DDCB_RLCIXD    (0b11011101110010110000000000000110)    // RLC (IX+d)
#define CODE_DDCB_RRCIXD    (0b11011101110010110000000000001110)    // RRC (IX+d)
#define CODE_DDCB_RLIXD     (0b11011101110010110000000000010110)    // RL (IX+d)
#define CODE_DDCB_RRIXD     (0b11011101110010110000000000011110)    // RR (IX+d)
#define CODE_DDCB_SLAIXD    (0b11011101110010110000000000100110)    // SLA (IX+d)
#define CODE_DDCB_SRAIXD    (0b11011101110010110000000000101110)    // SRA (IX+d)
#define CODE_DDCB_SRLIXD    (0b11011101110010110000000000111110)    // SRL (IX+d)
#define CODE_DDCB_BITBIXD   (0b11011101110010110000000001000110)    // BIT b,(IX+d)
#define CODE_DDCB_SETBIXD   (0b11011101110010110000000011000110)    // SET b,(IX+d)
#define CODE_DDCB_RESBIXD   (0b11011101110010110000000010000110)    // RES b,(IX+d)

/* FDCB instruction codes   */
#define CODE_FDCB_RLCIYD    (0b11111101110010110000000000000110)    // RLC (IY+d)
#define CODE_FDCB_RRCIYD    (0b11111101110010110000000000001110)    // RRC (IY+d)
#define CODE_FDCB_RLIYD     (0b11111101110010110000000000010110)    // RL (IY+d)
#define CODE_FDCB_RRIYD     (0b11111101110010110000000000011110)    // RR (IY+d)
#define CODE_FDCB_SLAIYD    (0b11111101110010110000000000100110)    // SLA (IY+d)
#define CODE_FDCB_SRAIYD    (0b11111101110010110000000000101110)    // SRA (IY+d)
#define CODE_FDCB_SRLIYD    (0b11111101110010110000000000111110)    // SRL (IY+d)
#define CODE_FDCB_BITBIYD   (0b11111101110010110000000001000110)    // BIT b,(IY+d)
#define CODE_FDCB_SETBIYD   (0b11111101110010110000000011000110)    // SET b,(IY+d)
#define CODE_FDCB_RESBIYD   (0b11111101110010110000000010000110)    // RES b,(IY+d)

/* Recognition masks    */
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
#define MASK_ANDR           (0b11111000)
#define MASK_ORR            (0b11111000)
#define MASK_XORR           (0b11111000)
#define MASK_CPR            (0b11111000)
#define MASK_ADDAHL         (0b11111111)
#define MASK_ADCAHL         (0b11111111)
#define MASK_ADDHLRR        (0b11001111)
#define MASK_CALLNN         (0b11111111)
#define MASK_CALLCCNN       (0b11000111)
#define MASK_RETCC          (0b11000111)
#define MASK_JPCCNN         (0b11000111)
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
#define MASK_EXSPHL         (0b11111111)
#define MASK_ADDAN          (0b11111111)
#define MASK_ADCAN          (0b11111111)
#define MASK_ANDN           (0b11111111)
#define MASK_ANDHL          (0b11111111)
#define MASK_ORN            (0b11111111)
#define MASK_ORHL           (0b11111111)
#define MASK_XORN           (0b11111111)
#define MASK_XORHL          (0b11111111)
#define MASK_CPN            (0b11111111)
#define MASK_CPHL           (0b11111111)
#define MASK_JRCE           (0b11111111)
#define MASK_JRNCE          (0b11111111)
#define MASK_JRZE           (0b11111111)
#define MASK_JRNZE          (0b11111111)
#define MASK_SUBAR          (0b11111000)
#define MASK_SUBAN          (0b11111111)
#define MASK_SUBAHL         (0b11111111)
#define MASK_SBCAR          (0b11111000)
#define MASK_SBCAN          (0b11111111)
#define MASK_SBCAHL         (0b11111111)
#define MASK_JPHL           (0b11111111)
#define MASK_CCF            (0b11111111)
#define MASK_SCF            (0b11111111)
#define MASK_RET            (0b11111111)
#define MASK_RSTP           (0b11000111)
#define MASK_DAA            (0b11111111)
#define MASK_EI             (0b11111111)
#define MASK_DI             (0b11111111)
#define MASK_EXDEHL         (0b11111111)

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
#define MASK_LDI            (0b1111111111111111)
#define MASK_LDIR           (0b1111111111111111)
#define MASK_LDD            (0b1111111111111111)
#define MASK_LDDR           (0b1111111111111111)
#define MASK_CPI            (0b1111111111111111)
#define MASK_CPIR           (0b1111111111111111)
#define MASK_CPD            (0b1111111111111111)
#define MASK_CPDR           (0b1111111111111111)
#define MASK_EXSPIX         (0b1111111111111111)
#define MASK_EXSPIY         (0b1111111111111111)
#define MASK_ADDAIXD        (0b1111111111111111)
#define MASK_ADDAIYD        (0b1111111111111111)
#define MASK_ADCAIXD        (0b1111111111111111)
#define MASK_ADCAIYD        (0b1111111111111111)
#define MASK_INCIXD         (0b1111111111111111)
#define MASK_INCIYD         (0b1111111111111111)
#define MASK_DECIXD         (0b1111111111111111)
#define MASK_DECIYD         (0b1111111111111111)
#define MASK_ANDIXD         (0b1111111111111111)
#define MASK_ANDIYD         (0b1111111111111111)
#define MASK_ORIXD          (0b1111111111111111)
#define MASK_ORIYD          (0b1111111111111111)
#define MASK_XORIXD         (0b1111111111111111)
#define MASK_XORIYD         (0b1111111111111111)
#define MASK_CPIXD          (0b1111111111111111)
#define MASK_CPIYD          (0b1111111111111111)
#define MASK_INCIX          (0b1111111111111111)
#define MASK_INCIY          (0b1111111111111111)
#define MASK_DECIX          (0b1111111111111111)
#define MASK_DECIY          (0b1111111111111111)
#define MASK_ADCHLRR        (0b1111111111001111)
#define MASK_SBCHLRR        (0b1111111111001111)
#define MASK_SUBAIXD        (0b1111111111111111)
#define MASK_SUBAIYD        (0b1111111111111111)
#define MASK_SBCAIXD        (0b1111111111111111)
#define MASK_SBCAIYD        (0b1111111111111111)
#define MASK_JPIX           (0b1111111111111111)
#define MASK_JPIY           (0b1111111111111111)
#define MASK_LDAI           (0b1111111111111111)
#define MASK_LDIA           (0b1111111111111111)
#define MASK_LDAR           (0b1111111111111111)
#define MASK_LDRA           (0b1111111111111111)

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
