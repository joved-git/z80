#---------------------------------------------------------------#
# generate_include.py - v 0.3                                   #
# The file aims to create ./inc/instruction_length.h file.      #
# The instruction_length.h file contains all the instruction    #
# lengths used to decode Z80 instruction.                       #
# The original values are stored into an ODS file located in    #
# the /docs directory (Z80_instruction_length.ods).             #
# To generate the CSV file, open the ODS file and save as it as #
# a CSV file. ';' is the separator and use '"' as a string      #
# separator.                                                    #
# To generate the .h file, just lanch this Python file and the  #
# inc/instruction_length.h file will be created in the inc      #
# directory. Do that only once if needed. That's all.           # 
#---------------------------------------------------------------#

import csv
import more_itertools as mit

# Some constants
NB_OF_ELEMENTS      = 17
INDICE_DEC          = 0
INDICE_HEXA         = 1
POS_OF_NATURAL_CODE = 3
POS_OF_CB_CODE      = 5
POS_OF_ED_CODE      = 7 
POS_OF_DD_CODE      = 9
POS_OF_FD_CODE      = 11
POS_OF_DDCB_CODE    = 13
POS_OF_FDCB_CODE    = 15
NB_OF_INST          = 256

# Open the result file and add header
result=open("./inc/instruction_length.h", "w")
result.write("/**********************************************************/\n")
result.write("/* Z80 instructions length header file                    */\n")
result.write("/* This file is generated by the generate_include.py file */\n")
result.write("/* Please, don't modify it.                               */\n")
result.write("/**********************************************************/\n")
result.write("\n")
result.write("#ifndef INSTRUCTION_LENGTH_H\n")
result.write("#define INSTRUCTION_LENGTH_H\n\n")

with open('docs/Z80_Instruction_length.csv', newline='') as csvfile:
    spamreader = csv.reader(csvfile, delimiter=';', quotechar='"')
    iterable = mit.seekable(spamreader)

    # Add the natural size #define into the file
    #for row in iterable:
    #    for i in range(NB_OF_ELEMENTS):
    #        if (row[i]==''):
    #            row[i]='0'
    #
    #    if row[INDICE_DEC]!='Dec':
    #        result.write("#define LENGTH_NATURAL_")
    #
    #        if int(row[INDICE_DEC]) < 16:
    #            result.write("0")
    #
    #        result.write(row[INDICE_HEXA] + "\t\t" + "(" + row[POS_OF_NATURAL_CODE] +")" + '\n')
    
    result.write("\n")
    iterable.seek(0)
    result.write("char natural_code_length[" + str(NB_OF_INST) + "]=\n{\n")
    j=0

    # Add the natural size array into the instruction_length.h file
    for row in iterable:
        for i in range(NB_OF_ELEMENTS):
            if (row[i]==''):
                row[i]='0'

        if row[INDICE_DEC]!='Dec':
            result.write(row[POS_OF_NATURAL_CODE])

            if j<NB_OF_INST-1:
                result.write(', ')

            j=j+1

            if j%16==0:
                result.write('\n')

    result.write("};\n")

    # Add the CB size array into the instruction_length.h file
    result.write("\n")
    iterable.seek(0)
    result.write("char cb_code_length[" + str(NB_OF_INST) + "]=\n{\n")
    j=0

    for row in iterable:
        for i in range(NB_OF_ELEMENTS):
            if (row[i]==''):
                row[i]='0'

        if row[INDICE_DEC]!='Dec':
            result.write(row[POS_OF_CB_CODE])

            if j<NB_OF_INST-1:
                result.write(', ')

            j=j+1

            if j%16==0:
                result.write('\n')

    result.write("};\n")

    # Add the ED size array into the instruction_length.h file
    result.write("\n")
    iterable.seek(0)
    result.write("char ed_code_length[" + str(NB_OF_INST) + "]=\n{\n")
    j=0

    for row in iterable:
        for i in range(NB_OF_ELEMENTS):
            if (row[i]==''):
                row[i]='0'

        if row[INDICE_DEC]!='Dec':
            result.write(row[POS_OF_ED_CODE])

            if j<NB_OF_INST-1:
                result.write(', ')

            j=j+1

            if j%16==0:
                result.write('\n')

    result.write("};\n")

    # Add the FD size array into the instruction_length.h file
    result.write("\n")
    iterable.seek(0)
    result.write("char fd_code_length[" + str(NB_OF_INST) + "]=\n{\n")
    j=0

    for row in iterable:
        for i in range(NB_OF_ELEMENTS):
            if (row[i]==''):
                row[i]='0'

        if row[INDICE_DEC]!='Dec':
            result.write(row[POS_OF_FD_CODE])

            if j<NB_OF_INST-1:
                result.write(', ')

            j=j+1

            if j%16==0:
                result.write('\n')

    result.write("};\n")

    # Add the DDCB size array into the instruction_length.h file
    result.write("\n")
    iterable.seek(0)
    result.write("char ddcb_code_length[" + str(NB_OF_INST) + "]=\n{\n")
    j=0

    for row in iterable:
        for i in range(NB_OF_ELEMENTS):
            if (row[i]==''):
                row[i]='0'

        if row[INDICE_DEC]!='Dec':
            result.write(row[POS_OF_DDCB_CODE])

            if j<NB_OF_INST-1:
                result.write(', ')

            j=j+1

            if j%16==0:
                result.write('\n')

    result.write("};\n")

    # Add the FDCB size array into the instruction_length.h file
    result.write("\n")
    iterable.seek(0)
    result.write("char fdcb_code_length[" + str(NB_OF_INST) + "]=\n{\n")
    j=0

    for row in iterable:
        for i in range(NB_OF_ELEMENTS):
            if (row[i]==''):
                row[i]='0'

        if row[INDICE_DEC]!='Dec':
            result.write(row[POS_OF_FDCB_CODE])

            if j<NB_OF_INST-1:
                result.write(', ')

            j=j+1

            if j%16==0:
                result.write('\n')

    result.write("};\n")

result.write("\n#endif\n")
result.close()