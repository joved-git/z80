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

//#define DEBUG_DISPLAY_INSTR_DATA              /* Dsiplay data that will be executer   */
//#define DEBUG_DISPLAY_CUTI_DATA               /* Display data after cutting it        */ 
//#define DEBUG_DISPLAY_FINDCODE_DATA           /* Display data after decoding it       */

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

//#define CODEFILE_LOCATION       "../codefile"

#define STRING_REG_UNDEFINED    "?"
#define STRING_REGA             "A"
#define STRING_REGB             "B"
#define STRING_REGC             "C"
#define STRING_REGD             "D"
#define STRING_REGE             "E"
#define STRING_REGH             "H"
#define STRING_REGL             "L"
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

#define FLAG_SIGN               (0b10000000)
#define FLAG_ZERO               (0b01000000)
#define FLAG_HALF_CARRY         (0b00010000)
#define FLAG_PARITY_OVERFLOW    (0b00000100)
#define FLAG_ADD_SUBTRACT       (0b00000010)
#define FLAG_CARRY              (0b00000001)

#define STRING_COND_UNDEFINED   "?"
#define STRING_CONDNZ           "NZ"
#define STRING_CONDZ            "Z"
#define STRING_CONDNC           "NC"
#define STRING_CONDC            "C"
#define STRING_CONDPO           "PO"
#define STRING_CONDPE           "PE"
#define STRING_CONDP            "P"
#define STRING_CONDM            "M"

enum typeOfEntry
    {
        NOTHING=0,
        COMMAND,
        INSTRUCTION,
        CODE
    };
#endif