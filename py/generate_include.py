import csv
import more_itertools as mit

NB_OF_ELEMENTS=17
INDICE_DEC=0
INDICE_HEXA=1
POS_OF_NATURAL_CODE=3
POS_OF_CB_CODE=5

result=open("../inc/instruction_length.h", "w")
result.write("/**********************************************************/\n")
result.write("/* Z80 instructions length header file                    */\n")
result.write("/* This file is generated by the extract_size.py file     */\n")
result.write("/**********************************************************/\n")
result.write("\n")
result.write("#ifndef INSTRUCTION_LENGTH_H\n")
result.write("#define INSTRUCTION_LENGTH_H\n\n")

with open('../docs/Z80_Instruction_length.csv', newline='') as csvfile:
    spamreader = csv.reader(csvfile, delimiter=';', quotechar='"')
    iterable = mit.seekable(spamreader)                    # 1

    for row in iterable:
        for i in range(NB_OF_ELEMENTS):
            if (row[i]==''):
                row[i]='0'
        #print(row[0])

        if row[INDICE_DEC]!='Dec':
            result.write("#define LENGTH_NATURAL_")

            if int(row[INDICE_DEC]) < 16:
                result.write("0")

            result.write(row[INDICE_HEXA] + "\t\t" + "(" + row[POS_OF_NATURAL_CODE] +")" + '\n')
    
    iterable.seek(0)
    
    for row in iterable:
        for i in range(NB_OF_ELEMENTS):
            if (row[i]==''):
                row[i]='0'

        if row[INDICE_DEC]!='Dec':
            result.write("#define LENGTH_CB_")

            if int(row[INDICE_DEC]) < 16:
                result.write("0")

            result.write(row[INDICE_HEXA] + "\t\t" + "(" + row[POS_OF_CB_CODE] +")" + '\n')

result.write("\n#endif\n")
result.close()