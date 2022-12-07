CC=C:\msys64\mingw64\bin\g++
CCFLAGS=-static-libgcc -static-libstdc++
CFLAGS=-I.
BUILD_DIR=.\build
SRC_DIR=.\src
SRC_FILES=	$(SRC_DIR)\z80.cpp \
		$(SRC_DIR)\Register_8bits.cpp \
		$(SRC_DIR)\Register_16bits.cpp \
		$(SRC_DIR)\Memory.cpp \
		$(SRC_DIR)\Label.cpp \
		$(SRC_DIR)\Z80Machine.cpp

z80:  
	rd /s/q $(BUILD_DIR)
	md $(BUILD_DIR)
	$(CC) $(CCFLAGS) $(SRC_FILES) -o $(BUILD_DIR)\z80.exe
	
