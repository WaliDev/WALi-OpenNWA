#ifndef __NWA_DETENSOR_H_
#define __NWA_DETENSOR_H_

#include <utility>
// ::opennwa
#include "opennwa/Nwa.hpp"

class bdd;

void bdd_printdot_levels(bdd r);
int bdd_fnprintdot_levels(char *fname, bdd r);
void bdd_fprintdot_levels(FILE* ofile, bdd r);

#endif //__NWA_DETENSOR_H_
