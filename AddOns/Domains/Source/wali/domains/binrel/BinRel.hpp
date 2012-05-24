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
#include <vector>

#include <boost/shared_ptr.hpp> // It'd be nice to include the standard version but there are too many ways to get it.

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
      /// Reference-counting bddPair pointer. These must be freed using
      /// bdd_freepair, not delete, so we can't used ref_ptr. This just wraps
      /// Boost's shared_ptr, setting bdd_freepair as the custom
      /// deleter. (And this is the main reason for not just using a typedef.)
      struct BddPairPtr
        : public boost::shared_ptr<bddPair>
      {
        typedef boost::shared_ptr<bddPair> Base;
        
        // We keep with the Boost/std practice of making this constructor
        // explicit, for it is a Good Idea.
        explicit
        BddPairPtr(bddPair * ptr)
          : Base(ptr, bdd_freepair)
        {}

        BddPairPtr(BddPairPtr const & other)
          : Base(other)
        {}

        BddPairPtr()
        {}
      };

      /**
        * Used to count the different operations performed by the library
        */
      typedef long long int StatCount;

      /**
       * Variable name, maximum value and the different base indices.
       */
      struct BddInfo : public Countable, public Printable
      {
          unsigned maxVal;
          // /fdd indices
          // The following three indices are for the base relation
          unsigned baseLhs;
          unsigned baseRhs;
          //used for extends
          unsigned baseExtra;
          //The following three indices are for the tensored relations when this
          //variable is in the left operand in the tensor product
          unsigned tensor1Lhs;
          unsigned tensor1Rhs;
          unsigned tensor1Extra;
          //The following three indices are for the tensored relations when this
          //variable is in the right operand in the tensor product
          unsigned tensor2Lhs;
          unsigned tensor2Rhs;
          unsigned tensor2Extra;

          std::ostream& print( std::ostream& o ) const;
      };
        
      typedef ref_ptr<BddInfo> bddinfo_t;

      class BinRel;
      typedef wali::ref_ptr<BinRel> binrel_t;
      /**A BddContext has the binding information for the variables in the
       * domain.
       */


      class BddContext : public std::map<const std::string,bddinfo_t>
      {
        friend class BinRel;
        public:
           /** 
           * A BddContext manages the vocabularies and stores some useful bdds
           * that speed up BinRel operations.
           * @param bddMemSize The size of memory BUDDY should be initialized
           *        with.  Default size is used if the argument is 0.  
           * @param cacheSize The size of cache BUDDY should be initialized
           *        with.  Default size is used if the argument is 0.
           */
          BddContext(int bddMemeSize=0, int cacheSize=0);
          BddContext(const BddContext& other);
          BddContext& operator = (const BddContext& other);
          virtual ~BddContext();

          /** Add a boolean variable to the vocabulary with the name 'name' **/
          virtual void addBoolVar(std::string name);
          /** Add a int variable to the vocabulary with the name 'name'. The
           * integer can take values between 0...size-1. **/
          virtual void addIntVar(std::string name, unsigned size);
        public:
          //using wali::Countable::count;
          int count;
        private:
          /** caches zero/one binrel objects for this context **/
          void populateCache();
        private:
          // ///////////////////////////////
          // We use the name convention for different 
          // sets as B1,B2,B3; TL1,TL2,TL3; TR1, TR2, TR3.
          // Comments contain the more meaningful, but ghastly names
          //B1->B2 B2->B1
          BddPairPtr baseSwap;
          //B1->B2 B2->B3
          BddPairPtr baseRightShift;
          //TL1->TL2 TR1->TR2 TL2->TL3 TR2->TR3 
          BddPairPtr tensorRightShift; 
          //B3->B2
          BddPairPtr baseRestore;
          //TL3->TL2 TR3->TR2
          BddPairPtr tensorRestore;
          //B1->LT1 B2->TL2 B3->TL3
          BddPairPtr move2Tensor1;
          //B1->TR1 B2->TR2 B3->TR3
          BddPairPtr move2Tensor2;
          //TL1->B1 TR2->B2
          BddPairPtr move2Base;
          //TL1->B1 TR1->B2
          BddPairPtr move2BaseTwisted;
          //sets for operation
          //Set: B2
          bdd baseSecBddContextSet;
          //Set: TL2 U TR2
          bdd tensorSecBddContextSet;
          //Set: TL2 U TR1
          bdd commonBddContextSet23;
          //Id: TL2 = TR1
          bdd commonBddContextId23;
          //Set: TL2 U TR2
          bdd commonBddContextSet13;
          //Id: TL2 = TR2
          bdd commonBddContextId13;

          //We cache zero and one BinRel objects, since they are used so much
          binrel_t cachedBaseOne;
          binrel_t cachedBaseZero;
          binrel_t cachedTensorOne;
          binrel_t cachedTensorZero;

          //Initialization of buddy is taken care of opaquely 
          //by keeping track of the number of BddContext objects alive
          static int numBddContexts;
      };

      
      typedef BddContext::iterator BddContextIter;
      typedef std::map<const int,std::string> RevBddContext;
      typedef wali::ref_ptr< BddContext > bdd_context_t;

      typedef std::map<std::string, int> Assignment;
      
      extern
      std::vector<Assignment>
      getAllAssignments(BddContext const & voc);

      extern
      void
      printImagemagickInstructions(bdd b, BddContext const & voc,
                                   std::ostream & os, std::string const & for_file);
      
      
      class BinRel : public wali::SemElemTensor 
      {
        public:
          static void reset();
        public:
          /** @see BinRel::Compose */
          friend binrel_t operator*(binrel_t a, binrel_t b);
          /** @see BinRel::Union */
          friend binrel_t operator|(binrel_t a, binrel_t b);
          /** @see BinRel::Intersect */
          friend binrel_t operator&(binrel_t a, binrel_t b);
        public:
          BinRel(const BinRel& that);
          BinRel(BddContext const * con, bdd b,bool is_tensored=false);
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

          /** @return The backing BDD */
          bdd getBdd() const {
            return rel;
          }

          /** @return Get the vocabulary the relation is over */
        // TODO: change name -eed May 14 2012
          BddContext const & getVocabulary() const {
            return *con;
          }

          // ////////////////////////////////
          // Printing functions
          //static void printHandler(FILE *o, int var);
#ifdef BINREL_STATS
          std::ostream& printStats( std::ostream& o ) const;
#endif
        protected:
          //This has to be a raw/weak pointer.
          //BddContext caches some BinRel objects. It is not BinRel's responsibility to
          //manage memory for BddContext. 
          BddContext const * con;
          bdd rel;
          bool isTensored;
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


