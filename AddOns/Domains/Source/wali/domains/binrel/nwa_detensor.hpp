#ifndef __NWA_DETENSOR_H_
#define __NWA_DETENSOR_H_

#include <utility>
// ::opennwa
#include "opennwa/Nwa.hpp"
#include "opennwa/WeightGen.hpp"

class bdd;

void bdd_printdot_levels(bdd r);
int bdd_fnprintdot_levels(char *fname, bdd r);
void bdd_fprintdot_levels(FILE* ofile, bdd r);
namespace wali
{
  namespace domains
  {
    namespace binrel
    {
      class DetensorWeightGen : public opennwa::WeightGen
      {
        //The kinds of edges that need to be considered.
        //  INTRA:  corresponds to an Internal transition in the NWA
        //  CALL_TO_ENTRY:  corresponds to a Call transition in the NWA
        //  EXIT_TO_RET:  corresponds to the exit-to-return part of a Return transition in the NWA
        //  CALL_TO_RET:  corresponds to the call-to-return part of a Return transition in the NWA
        public:

          DetensorWeightGen( ) { }
          virtual ~DetensorWeightGen() {}

          virtual sem_elem_t getOne( ) const;

          virtual sem_elem_t getWeight( Key src, ClientInfoRefPtr srcInfo, Key sym, Kind kind, Key tgt, ClientInfoRefPtr tgtInfo ) const;

          void setWeight(Kind kind, Key src, Key sym, Key tgt, binrel_t b); 
          void clear();
      };
    }
  }
}
#endif //__NWA_DETENSOR_H_
