#ifndef REGISTER_PACK_H
#define REGISTER_PACK_H

#include "../inc/Register_8bits.h"
#include "../inc/Register_F.h"
#include "../inc/Register_16bits.h"

struct RegisterPack 
{
	/* 8-bit registers  		*/
	Register_8bits regB;
	Register_8bits regC;
	Register_8bits regD;
	Register_8bits regE;
	Register_8bits regH;
	Register_8bits regL;
	Register_8bits regA;
	Register_F regF;
	Register_8bits regI;
	Register_8bits regR;
	
	/* 16-bit registers  		*/
	Register_16bits regSP;
	Register_16bits regPC;
	Register_16bits regBC;
	Register_16bits regDE;
	Register_16bits regHL;
	Register_16bits regAF;
	Register_16bits regIX;
	Register_16bits regIY;

	/* 16-bit hidden registers	*/
	Register_16bits regAFp;
	Register_16bits regBCp;
	Register_16bits regDEp;
	Register_16bits regHLp;

	/* Flip-flop	*/
	bool iff1;
	bool iff2;
};

#endif