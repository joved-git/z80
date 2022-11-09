CC=C:\msys64\mingw64\bin\g++
CFLAGS=-I.
BUILD_DIR=.\build
SRC_DIR=.\src
SRC_FILES=	$(SRC_DIR)\z80.cpp \
		$(SRC_DIR)\Register_8bits.cpp \
		$(SRC_DIR)\Register_16bits.cpp \
		$(SRC_DIR)\Z80Machine.cpp

z80:  
	rd /s/q $(BUILD_DIR)
	md $(BUILD_DIR)
	$(CC) $(SRC_FILES) -o $(BUILD_DIR)\z80.exe
	
