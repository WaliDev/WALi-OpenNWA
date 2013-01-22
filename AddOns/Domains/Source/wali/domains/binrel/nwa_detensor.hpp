#ifndef __NWA_DETENSOR_H_
#define __NWA_DETENSOR_H_
// ::std
#include <utility>
// ::std::tr1
#include <tr1/tuple>
// ::std::tr1
#include <tr1/unordered_map>
// ::opennwa
#include "opennwa/Nwa.hpp"
#include "opennwa/WeightGen.hpp"
// ::wali
#include "wali/SemElem.hpp"
#include "wali/ref_ptr.hpp"
// ::wali::domains::binrel
#include "BinRel.hpp"

#include "buddy/bdd.h"

class bdd;

void bdd_printdot_levels(bdd r);
int bdd_fnprintdot_levels(char const * fname, bdd r);
void bdd_fprintdot_levels(FILE* ofile, bdd r);
namespace wali
{
  namespace domains
  {
    namespace binrel
    {

      class DetensorNwa : public opennwa::Nwa
      {
        public:
          virtual bool stateIntersect(
              opennwa::Nwa const & first, 
              opennwa::State state1, 
              opennwa::Nwa const& second, 
              opennwa::State state2,
              opennwa::State& resSt, 
              opennwa::ClientInfoRefPtr& resCI);
      };

      class DetensorWeight;
      typedef wali::ref_ptr<DetensorWeight> detensor_weight_t;
      class DetensorWeight : public wali::SemElem
      {
        public:
          static void initialize() 
            {
              detensor_weight_one = new DetensorWeight(bddtrue);
              detensor_weight_zero = new DetensorWeight(bddfalse);
            }
        public:
          DetensorWeight(bdd b) : rel(b) {}
          virtual ~DetensorWeight() {}
          virtual sem_elem_t one() const {
            return detensor_weight_one.get_ptr();
          }
          virtual sem_elem_t zero() const {
            return detensor_weight_zero.get_ptr();
          }
          virtual sem_elem_t extend(SemElem * se) {
            DetensorWeight * dse = static_cast<DetensorWeight*>(se);
            if(rel == bddtrue)
              return dse;
            if(dse->rel == bddtrue)
              return this;
            bdd res = bdd_and(rel, dse->rel);
            if(res == bddfalse)
              return detensor_weight_zero;
            return new DetensorWeight(res);
          }
          virtual sem_elem_t combine(SemElem * se) {
            DetensorWeight * dse = static_cast<DetensorWeight*>(se);
            if(rel == bddfalse)
              return dse;
            if(dse->rel == bddfalse)
              return this;
            bdd res = bdd_or(rel, dse->rel);
            if(res == bddtrue)
              return detensor_weight_one;
            return new DetensorWeight(res);
          }
          virtual bool equal(SemElem * se) const{
            DetensorWeight * dse = static_cast<DetensorWeight*>(se);
            return rel == dse->rel;
          }
          virtual std::ostream& print(std::ostream& o) const {
            return (o << fddset << rel);
          }
          bdd getBdd() const {
            return rel;
          }
        public:
          static detensor_weight_t detensor_weight_one;
          static detensor_weight_t detensor_weight_zero;
        private:
          bdd rel;
      };

      class DetensorWeightGen : public opennwa::WeightGen
      {
        //The kinds of edges that need to be considered.
        //  INTRA:  corresponds to an Internal transition in the NWA
        //  CALL_TO_ENTRY:  corresponds to a Call transition in the NWA
        //  EXIT_TO_RET:  corresponds to the exit-to-return part of a Return transition in the NWA
        //  CALL_TO_RET:  corresponds to the call-to-return part of a Return transition in the NWA -- XXX: afaik, never used 
        
        public:
          typedef std::tr1::tuple< Kind, Key, Key, Key > TransQuad;
          class TransQuadHash
          {
            public:
              size_t operator () (TransQuad const& q) const;
          };
          typedef std::tr1::unordered_map< TransQuad, detensor_weight_t, TransQuadHash > TransWeightMap;

        public:
          DetensorWeightGen(detensor_weight_t o) : theOne(o) { }
          virtual ~DetensorWeightGen() {}

          virtual sem_elem_t getOne( ) const;

          virtual sem_elem_t getWeight( Key src, ClientInfoRefPtr srcInfo, Key sym, Kind kind, Key tgt, ClientInfoRefPtr tgtInfo ) const;

          /*
           * Set the weight for the given transition. If the transition already
           * existed, combine the two weights.
           * @return true if the transition did not already exist, false otherwise 
           **/
          bool setWeight(Kind kind, Key src, Key sym, Key tgt, detensor_weight_t b); 
          void clear();

        private:
          detensor_weight_t theOne;
          TransWeightMap twmap;
      };
    }
  }
}
#endif //__NWA_DETENSOR_H_
