# z80
This software is a command line tool used to explore the Zilog Z80 processor.

Please, this is the almost first debut of this program. I will prepare Specifications document as soon as possible.
See ya !

# Compilation under Linux
1. Clone the repo.

```
$ cd <bla>
$ git clone git@github.com:joved-git/z80.git
```

2. Go to inside the directory and create the build directory.

```
$ cd z80
$ mkdir build
$ cd build
```

3. Compile the soft

```
$ cmake ..
$ make
```

4. Enjoy !

```
$ ./z80
```

# Compilation under Windows
1. Clone the repo.

```
D:\> cd dev_dir
D:\dev_dir> git@github.com:joved-git/z80.git
```

2. Open a Windows **Terminal**

Go to inside the directory and create a void build directory.

```
D:\dev_dir> cd z80
D:\dev_dir\z80> rd /s /q build
D:\dev_dir\z80> md build
```

3. Compile the soft

```
D:\dev_dir\z80\build> cmake -G "Unix Makefiles" ..
D:\dev_dir\z80\build> make
```

4. Enjoy !

```
D:\dev_dir\z80\build>z80.exe
```

# Note about the instruction_length.h file.

The included Pyhton file generate_length.py aims to create the ./inc/instruction_length.h file.

The instruction_length.h file contains all the instruction lengths used to easily decode Z80 instruction. The original values of all these lengths are stored into an ODS file located in the ./docs directory (Z80_instruction_length.ods).

You can regenerate all the files but it is not necessary. To completely regenerate the .h file from scratch, begin to create the CSV file. Open the ODS file and save it as a CSV file with ';' as the separator and '"' as the string separator.
                                                    
To finally generate the .h file, just lanch the generate_include.py Pyhton file from Visual Studio Code. You will find the instruction_length.h file into the ./inc directory. 
