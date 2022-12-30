/************************************************************/
/* common_values.h: common declaration for all other files. */
/************************************************************/

#ifndef COMMON_VALUES_H
#define COMMON_VALUES_H

#include "opcode.h"

#if defined(__WIN32__)
#define WIN
#else
#define LINUX
#endif

#ifdef WIN
#define ASM_FILE_LOCATION       "..\\codefile\\"
#else
#define ASM_FILE_LOCATION       "../codefile/"
#endif

// #define DEBUG_DISPLAY_INSTR_DATA              /* Display data that will be executed   mEntry*/
// #define DEBUG_DISPLAY_COLOR_CHANGED           /* Display data about color changing    */
// #define DEBUG_DISPLAY_CUTI_DATA               /* Display data after cutting it        */ 
// #define DEBUG_DISPLAY_FINDCODE_DATA           /* Display data after decoding it       */
// #define DEBUG_DISPLAY_READ_ASM_DATA           /* Display the data read from asm file  */
// #define DEBUG_DISPLAY_READ_ASM_DATA           /* Display the data read from asm file  */
// #define DEBUG_DISPLAY_LABEL_ASM_DATA          /* Display label calc asm data          */

#define EOL                 	'\n'            /* The user has typed Enter                                 */                             
#define MAX_LEN	            	256             /* Max length of an entry                                   */
//#define MAX_LEN_FOR_A_CODE  	8               /* 4 bytes is the max size of a machine code instruction    */
#define MAX_OP_LENGTH           16              /* Max size of an operand                                   */
#define MAX_INSTR_LENGTH        3*MAX_OP_LENGTH /* Max length for an instruction                            */

#define HALF					false			/* A 16-bit register made of 2 4-bit registers (example BC)	*/
#define FULL					true			/* A full 16-bit register (example, PC or SP)				*/

#define I_AM_NAN                (1)
#define I_AM_NUMBER             (2)

#define INITIAL_STACK_POINTER	(0x0000)		/* Initial value for the Stack Pointer						*/


#define BIT_0                   0
#define BIT_1                   1
#define BIT_2                   2
#define BIT_3                   3
#define BIT_4                   4
#define BIT_5                   5
#define BIT_6                   6
#define BIT_7                   7

#define VAL_0                   0
#define VAL_1                   1
#define VAL_2                   2
#define VAL_3                   3
#define VAL_4                   4
#define VAL_5                   5
#define VAL_6                   6
#define VAL_7                   7
#define VAL_8                   8
#define VAL_9                   9
#define VAL_A                   10
#define VAL_B                   11
#define VAL_C                   12
#define VAL_D                   13
#define VAL_E                   14
#define VAL_F                   15

#define STRING_REG_UNDEFINED    "?"
#define STRING_REGA             "A"
#define STRING_REGB             "B"
#define STRING_REGC             "C"
#define STRING_REGD             "D"
#define STRING_REGE             "E"
#define STRING_REGF             "F"
#define STRING_REGH             "H"
#define STRING_REGL             "L"
#define STRING_REGI             "I"
#define STRING_REGR             "R"
#define STRING_REGBC            "BC"
#define STRING_REGIBC           "(BC)"
#define STRING_REGDE            "DE"
#define STRING_REGIDE           "(DE)"
#define STRING_REGHL            "HL"
#define STRING_REGIHL           "(HL)"
#define STRING_REGAF            "AF"
#define STRING_REGIAF           "(AF)"
#define STRING_REGSP            "SP"
#define STRING_REGISP           "(SP)"
#define STRING_REGIX            "IX"
#define STRING_REGIY            "IY"

#define CMD_HELP                'h'
#define CMD_MACHINECODE         'c'
#define CMD_EXIT                'q'
#define CMD_REGISTER            'r'
#define CMD_ALL_REGISTER        'R'
#define CMD_ASSEMBLYCODE        'a'
#define CMD_TOHEXA              'x'
#define CMD_TODEC               'd'
#define CMD_TOBIN               'b'
#define CMD_EXAMPLE             'e'
#define CMD_DISPLAY_MEMORY      'm'
#define CMD_NEXT_INSTRUCTION    'n'
#define CMD_LOAD_CODE           'l'
#define CMD_EXEC_MODE_TOGGLE    '!'

#define ERR_NO_ERROR            (0)
#define ERR_NO_REGISTER         (-1)
#define ERR_BAD_OPERAND         (-2)
#define ERR_NO_CONDITION        (-3)

#define MASK_FLAG_SIGN               (0b10000000)
#define MASK_FLAG_ZERO               (0b01000000)
#define MASK_FLAG_HALF_CARRY         (0b00010000)
#define MASK_FLAG_PARITY_OVERFLOW    (0b00000100)
#define MASK_FLAG_ADD_SUBTRACT       (0b00000010)
#define MASK_FLAG_CARRY              (0b00000001)

#define BITPOS_FLAG_SIGN               (7)
#define BITPOS_FLAG_ZERO               (6)
#define BITPOS_FLAG_HALF_CARRY         (4)
#define BITPOS_FLAG_PARITY_OVERFLOW    (2)
#define BITPOS_FLAG_ADD_SUBTRACT       (1)
#define BITPOS_FLAG_CARRY              (0)

#define STRING_COND_UNDEFINED   "?"
#define STRING_CONDNZ           "NZ"
#define STRING_CONDZ            "Z"
#define STRING_CONDNC           "NC"
#define STRING_CONDC            "C"
#define STRING_CONDPO           "PO"
#define STRING_CONDPE           "PE"
#define STRING_CONDP            "P"
#define STRING_CONDM            "M"

/* Return code for interpretCode() function */
#define NOTHING_SPECIAL         0
#define NO_PC_CHANGE            1

/* Return code for findMachineCode() function */
#define NOT_DECODED             0xFFFFFFFF

enum typeOfEntry
    {
        NOTHING=0,
        COMMAND,
        INSTRUCTION,
        CODE
    };

/* Array used to calculate parity   */
/* Be carreful, result is inverted  */
/* to calculate easly the result    */
static unsigned char parityTable[16] = 
{
    1, 0, 0, 1,
    0, 1, 1, 0,
    0, 1, 1, 0,
    1, 0, 0, 1
};

#endif