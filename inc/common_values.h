/************************************************************/
/* common_values.h: common declaration for all other files. */
/************************************************************/

#ifndef COMMON_VALUES_H
#define COMMON_VALUES_H

#define EOL '\n'
#define FALSE	0
#define TRUE	1
#define MAX_LEN	256

enum typeOfEntry
    {
        NOTHING=0,
        COMMAND,
        INSTRUCTION,
        CODE
    };

#endif