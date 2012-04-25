#ifndef wN_binrel_BINREL_GUARD
#define wN_binrel_BINREL_GUARD 1

/**
 * @author Nicholas Kidd
 * @author Prathmesh Prabhu
 *
 * An implementation of binary relations. 
 * This class models the relations
 * using BDDs, and specifically with the BuDDy
 * BDD library. 
 *
 *
 */

/* ****************************
  Changelog (Prathmesh):
    (1) Added limited tensor product functionality. 2-STP implemented. No support
        for more general tensor products.
 */

#include <map>
#include <utility>
#include <string>

#include "wali/Countable.hpp"
#include "wali/ref_ptr.hpp"
#include "wali/SemElemTensor.hpp"
#include "buddy/fdd.h"

namespace wali 
{
  namespace domains 
  {
    namespace binrel 
    {
      /**
        * Used to count the different operations performed by the library
        */
      typedef long long int StatCount;

      /**
       * Variable name, maximum value and the different base indices.
       */
      struct BddInfo : public Countable, public Printable
      {
          int maxVal;
          // /fdd indices
          // The following three indices are for the base relation
          int baseLhs;
          int baseRhs;
          //used for extends
          int baseExtra;
          //The following three indices are for the tensored relations when this
          //variable is in the left operand in the tensor product
          int tensor1Lhs;
          int tensor1Rhs;
          int tensor1Extra;
          //The following three indices are for the tensored relations when this
          //variable is in the right operand in the tensor product
          int tensor2Lhs;
          int tensor2Rhs;
          int tensor2Extra;

          std::ostream& print( std::ostream& o ) const;
      };
        
      typedef ref_ptr<BddInfo> BddInfo_t;

      /**A Voc has the binding information for the variables in the
       * domain.
       */
      typedef std::map<const std::string,BddInfo_t> Voc;
      typedef Voc::iterator VocIter;
      typedef std::map<const int,std::string> RevVoc;

      class BinRel;
      typedef wali::ref_ptr<BinRel> binrel_t;

      class BinRel : public wali::SemElemTensor 
      {
        public:

          /** 
           * Initialize the binary relations semiring with the given vocabulary
           * and bdd package specific parameters.  
           * @param bddMemSize The size of memory BUDDY should be initialized
           *        with.  Default size is used if the argument is 0.  
           * @param cacheSize The size of cache BUDDY should be initialized
           *        with.  Default size is used if the argument is 0.
           * @param voc The vocabulary to initialize with. Required field is the
           *        maximum int value of each variable in the vocabulary
           *        [maxVal]. Optional field is the name of each vaiable [name].
           *        The variable index fields are ignored.
           * @return [const] 
           *        Voc updated with the fddIdx values.
           */
          static const Voc initialize(int bddMemSize, int cacheSize, Voc voc);

          /** @return true if BinRel has been successfully initialized */
          static bool is_initialized();

          static void reset();

          inline static const Voc getVoc() { return voc; }
        public:
          /** @see BinRel::Compose */
          friend binrel_t operator*(binrel_t a, binrel_t b);

          /** @see BinRel::Union */
          friend binrel_t operator|(binrel_t a, binrel_t b);

          /** @see BinRel::Intersect */
          friend binrel_t operator&(binrel_t a, binrel_t b);

        public:
          BinRel(const BinRel& that);
          BinRel(bdd b,bool is_tensored=false);

          virtual ~BinRel();

        public:
          binrel_t Compose( binrel_t that ) const;
          binrel_t Union( binrel_t that ) const;
          binrel_t Intersect( binrel_t that ) const;
          bool Equal(binrel_t se) const;
          binrel_t Transpose() const;
          binrel_t Kronecker( binrel_t that) const;
          binrel_t Eq23Project() const;
          binrel_t Eq13Project() const;
        public: 
          // ////////////////////////////////
          // SemElem methods
          sem_elem_t one() const;
          sem_elem_t zero() const;

          /** @return [this]->Union( cast<BinRel*>(se) ) */
          sem_elem_t combine(SemElem* se);

          /** @return [this]->Compose( cast<BinRel*>(se) ) */
          sem_elem_t extend(SemElem* se);

          /** @return [this]->Equal( cast<BinRel*>(se) ) */
          bool equal(SemElem* se) const;

          std::ostream& print( std::ostream& o ) const;

          // ////////////////////////////////
          // SemElemTensor methods

          /** @return [this]->Transpose() */
          sem_elem_tensor_t transpose();

          /** @return [this]->Kronecker( cast<BinRel*>(se) ) */
          sem_elem_tensor_t tensor(SemElemTensor* se);

          /** @return [this]->Eq23Project() */
          sem_elem_tensor_t detensor();

          /** @return [this]->Eq34Project() */
          sem_elem_tensor_t detensorTranspose();

          // ////////////////////////////////
          // Printing functions
          //static void printHandler(FILE *o, int var);
#ifdef BINREL_STATS
          std::ostream& printStats( std::ostream& o ) const;
#endif
        protected:
          /** gets a bdd for identity relation on the correct bdd variables.*/
          static void setId();

        protected:
          bdd rel;
          bool isTensored;

          // ////////////////////////////////
          // Static variables for the domain
        private:
          static Voc voc;

          // ///////////////////////////////
          // We use the name convention for different 
          // sets as B1,B2,B3; TL1,TL2,TL3; TR1, TR2, TR3.
          // Comments contain the more meaningful, but ghastly names

          //B1->B2 B2->B1
          static bddPair* baseSwap;
          //B1->B2 B2->B3
          static bddPair* baseRightShift;
          //TL1->TL2 TR1->TR2 TL2->TL3 TR2->TR3 
          static bddPair* tensorRightShift; 
          //B3->B2
          static bddPair* baseRestore;
          //TL3->TL2 TR3->TR2
          static bddPair* tensorRestore;
          //B1->LT1 B2->TL2 B3->TL3
          static bddPair* move2Tensor1;
          //B1->TR1 B2->TR2 B3->TR3
          static bddPair* move2Tensor2;
          //TL1->B1 TR2->B2
          static bddPair* move2Base;
          //TL1->B1 TR1->B2
          static bddPair* move2BaseTwisted;

          //sets for operation
          //Set: B2
          static bdd baseSecVocSet;
          //Set: TL2 U TR2
          static bdd tensorSecVocSet;
          //Set: TL2 U TR1
          static bdd commonVocSet23;
          //Id: TL2 = TR1
          static bdd commonVocId23;
          //Set: TL2 U TR2
          static bdd commonVocSet13;
          //Id: TL2 = TR2
          static bdd commonVocId13;

          //Static identities
          static bdd baseId;
          static bdd tensorId;

#ifdef BINREL_STATS
          //Statistics
          static StatCount numCompose;
          static StatCount numUnion;
          static StatCount numIntersect;
          static StatCount numEqual;
          static StatCount numKronecker;
          static StatCount numTranspose;
          static StatCount numEq23Project;
          static StatCount numEq24Project;
#endif
      };

    } // namespace binrel

  } // namespace domains

} // namespace wali

// Yo, Emacs!
// Local Variables:
//   c-file-style: "ellemtel"
//   c-basic-offset: 2
// End:

#endif  // wali_binrel_BINREL_GUARD


