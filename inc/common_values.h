/************************************************************/
/* common_values.h: common declaration for all other files. */
/************************************************************/

#ifndef COMMON_VALUES_H
#define COMMON_VALUES_H

#define EOL                 '\n'            /* The user has typed Enter                                 */
#define FALSE	            0               /* Not true                                                 */
#define TRUE	            1               /* Not false                                                */
#define MAX_LEN	            256             /* Max length of an entry                                   */
#define MAX_LEN_FOR_A_CODE  8               /* 4 bytes is the max size of a machine code instruction   */

class Register_8bits;

enum typeOfEntry
    {
        NOTHING=0,
        COMMAND,
        INSTRUCTION,
        CODE
    };
#endif