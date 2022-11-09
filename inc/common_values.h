/************************************************************/
/* common_values.h: common declaration for all other files. */
/************************************************************/

#ifndef COMMON_VALUES_H
#define COMMON_VALUES_H

#define EOL                 	'\n'            /* The user has typed Enter                                 */
#define FALSE	            	0               /* Not true                                                 */
#define TRUE	            	1               /* Not false                                                */
#define MAX_LEN	            	256             /* Max length of an entry                                   */
#define MAX_LEN_FOR_A_CODE  	8               /* 4 bytes is the max size of a machine code instruction   */

#define HALF					false			/* A 16-bit register made of 2 4-bit registers (example BC)	*/
#define FULL					true			/* A full 16-bit register (example, PC or SP)				*/

#define INITIAL_STACK_POINTER	(0xDFF0)		/* Initial value for the Stack Pointer	*/

enum typeOfEntry
    {
        NOTHING=0,
        COMMAND,
        INSTRUCTION,
        CODE
    };
#endif