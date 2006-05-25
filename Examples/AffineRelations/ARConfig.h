#ifndef __A_R_CONFIG_H
#define __A_R_CONFIG_H

#define arEAX   1  ///< Variable number for register eax.
#define arEBX   2  ///< Variable number for register ebx.
#define arECX   3  ///< Variable number for register ecx.
#define arEDX   4  ///< Variable number for register edx.
#define arESI   5  ///< Variable number for register esi.
#define arEDI   6  ///< Variable number for register edi.
#define arEBP   7  ///< Variable number for register ebp.
#define arESP   8  ///< Variable number for register esp.
#define arEAX0  9  ///< Variable number for register eax0.
#define arEBX0  10 ///< Variable number for register ebx0.
#define arECX0  11 ///< Variable number for register ecx0.
#define arEDX0  12 ///< Variable number for register edx0.
#define arESI0  13 ///< Variable number for register esi0.
#define arEDI0  14 ///< Variable number for register edi0.
#define arEBP0  15 ///< Variable number for register ebp0.
#define arESP0  16 ///< Variable number for register esp0.
#define arLAST  8  ///< The last variable number.

/// Number of variables for affine relations. 
#define NVARS          (arLAST)


/// The array of variable names for rendering affine relations.
extern char* arVarNames[];

namespace AR {
	typedef unsigned dim_t;
	const dim_t dim = NVARS + 1;
	extern unsigned long long nVarDist[AR::dim]; ///< Distribution of the number of variables in affine relations
	                                           ///< (Gives an idea of how complex the affine rels are).
	
	extern unsigned long long nDimDist[AR::dim]; ///< Distribution on the dimensionality of the vector space.

	const unsigned VER_WPDS       = 0;
	const unsigned VER_EWPDS      = 1;
	const unsigned VER_WPDS_CHEAT = 2;		
	const unsigned VER_NR         = 3;		
	
	extern unsigned whichWpdsVersion;
#ifdef USE_AKASH_EWPDS
	extern bool constructingEWPDS;
#endif
}

#define VSA_RESTORE_REGS_IN_AFFREL 1


#endif
