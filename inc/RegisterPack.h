#ifndef REGISTER_PACK_H
#define REGISTER_PACK_H

#include "../inc/Register_8bits.h"
#include "../inc/Register_16bits.h"

struct RegisterPack 
	{
        /* 8-bit registers  */
    	Register_8bits regB;
    	Register_8bits regC;
    	Register_8bits regD;
    	Register_8bits regE;
    	Register_8bits regH;
    	Register_8bits regL;
		Register_8bits regA;
    	Register_8bits regF;

        /* 16-bit registers  */
        Register_16bits regSP;
        Register_16bits regPC;
        Register_16bits regBC;
        Register_16bits regDE;
        Register_16bits regHL;
	};

#endif