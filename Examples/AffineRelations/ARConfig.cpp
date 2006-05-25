#include "ARConfig.h"

// Variable Names for use in rendering the affine relations.
char *arVarNames[]={"1","eax","ebx","ecx","edx","esi","edi","ebp","esp"};
//char *arVarNames[]={"1","eax","ebx","ecx","edx","esi","edi","ebp","esp",
//                        "eax0","ebx0","ecx0","edx0","esi0","edi0","ebp0","esp0"};

namespace AR {
	/// Distribution of the number of variables in affine relations
	/// (Gives an idea of how complex the affine rels are).
	unsigned long long nVarDist[AR::dim] = {0, 0, 0, 0, 0, 0, 0, 0, 0}; 
	                                         
	/// Distribution on the dimensionality of the vector space.
	unsigned long long nDimDist[AR::dim] = {0, 0, 0, 0, 0, 0, 0, 0, 0};


	unsigned whichWpdsVersion = VER_EWPDS;
#ifdef USE_AKASH_EWPDS
	bool constructingEWPDS = false;
#endif
}
