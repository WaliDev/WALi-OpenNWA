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
#include <vector>
#include <utility>
#include <string>
#include <vector>

#include <boost/shared_ptr.hpp> // It'd be nice to include the standard version but there are too many ways to get it.
#include <boost/unordered_set.hpp>

#include "wali/wfa/WeightMaker.hpp"
#include "wali/Countable.hpp"
#include "wali/ref_ptr.hpp"
#include "wali/SemElemTensor.hpp"
#include "buddy/fdd.h"


/**
 *
 * The following describes how to control the bdd variables are setup in buddy
 * Let x, y, w and z be variables, each requiring two bdd levels (x1 and x2)
 * Further, for the bit x1, there are three different levels x1b, x1t1 and x1t2 
 * -- the first one for base, the second and third for the two tensor spots. 
 * Finally, each spot needs three levels, x1b, x1b' and x1b''.
 *
 * Let's disregard tensors for the moment.
 * By default, the variables are interleaved at the bit level. So, a call to 
 * setInts({(x, 2), (y, 2), (z, 2)}) gives the interleaving
 * x1b x1b' x1b'' y1b y1b' y1b'' z1b z1b' z1b'' x2b x2b' x2b'' y2b y2b' y2b'' z2b z2b' z2b''
 * There is another way of interleaving these variables. Where variables are grouped together.
 * setInts({{(x, 2), (y, 2)}, {(z, 2), (w, 2)}}) 
 * x1b x1b' x1b'' y1b y1b' y1b'' x2b x2b' x2b'' y2b y2b' y2b'' z1b z1b' z1b'' w1b w1b' w1b'' z2b z2b' z2b'' w2b w2b' w2b''
 * 
 * There are two choices with regard to tensors. 
 * (1) TENSOR_MAX_AFFINITY
 * This makes sure that all three levels for a bit are always together, i.e., you always get
 * x1b x1b' x1b'' x1t1 x1t1' x1t1'' x1t2 x1t2' x1t2''
 * The interleaving at higher levels remains the same
 * (2) TENSOR_MIN_AFFINITY
 * This makes sure that the three levels for the whole vocabulary are separate and copies of each other.
 * So, for the example above, the levels would look like.
 * x1b x1b' x1b'' y1b y1b' y1b'' x2b x2b' x2b'' y2b y2b' y2b'' z1b z1b' z1b'' w1b w1b' w1b'' z2b z2b' z2b'' w2b w2b' w2b''
 * x1t1 x1t1' x1t1'' y1t1 y1t1' y1t1'' x2t1 x2t1' x2t1'' y2t1 y2t1' y2t1'' z1t1 z1t1' z1t1'' w1t1 w1t1' w1t1'' z2t1 z2t1' z2t1'' w2t1 w2t1' w2t1''
 * x1t2 x1t2' x1t2'' y1t2 y1t2' y1t2'' x2t2 x2t2' x2t2'' y2t2 y2t2' y2t2'' z1t2 z1t2' z1t2'' w1t2 w1t2' w1t2'' z2t2 z2t2' z2t2'' w2t2 w2t2' w2t2''
 * (3) BASE_MAX_AFFINITY_TENSOR_MIXED
 * This makes sure that base is separate, but mixes together tensors vocabularies. 
 * x1b x1b' x1b'' y1b y1b' y1b'' x2b x2b' x2b'' y2b y2b' y2b'' z1b z1b' z1b'' w1b w1b' w1b'' z2b z2b' z2b'' w2b w2b' w2b''
 * followed by tensor levels as x1t1 x1t1' x1t1'' x1t2 x1t2' x1t2''
 * (4) TENSOR_MATCHED_PAREN
 * Keeps all Three vocabs (base,tensor1,tensor2) separate, like TENSOR_MIN_AFFINITY.
 * Additionally, the three microlevels in each bit x x' x'' are in reverse order for tensor1.
 * So, for the example above, the levels would look like.
 * x1b x1b' x1b'' y1b y1b' y1b'' x2b x2b' x2b'' y2b y2b' y2b'' z1b z1b' z1b'' w1b w1b' w1b'' z2b z2b' z2b'' w2b w2b' w2b''
 * x1t1'' x1t1' x1t1 y1t1'' y1t1' y1t1 x2t1'' x2t1' x2t1 y2t1'' y2t1' y2t1 z1t1'' z1t1' z1t1 w1t1'' w1t1' w1t1 z2t1'' z2t1' z2t1 w2t1'' w2t1' w2t1
 * x1t2 x1t2' x1t2'' y1t2 y1t2' y1t2'' x2t2 x2t2' x2t2'' y2t2 y2t2' y2t2'' z1t2 z1t2' z1t2'' w1t2 w1t2' w1t2'' z2t2 z2t2' z2t2'' w2t2 w2t2' w2t2''
 * NOTE: ***This currently only works correctly with boolean variables.***
 * The tensor choice is determined by the following macro:
 **/
//#define TENSOR_MIN_AFFINITY
//#define TENSOR_MAX_AFFINITY
//#define BASE_MAX_AFFINITY_TENSOR_MIXED
/*** NOTE: This currently only works correctly with boolean variables.***/
#define TENSOR_MATCHED_PAREN


//#define DETENSOR_TOGETHER
#define DETENSOR_BIT_BY_BIT


/** 
  To switch on statistics collection in BinRel
**/
#define BINREL_STATS

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
          typedef boost::unordered_set<int> VocLevelSet;
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

          /** 
           * Add multiple variables to the vocabulary with the given sizes. 
           * This function should not be used after addBoolVar/addIntVar has been
           * used. This function is meant to be an effecient way of adding multiple variables
           **/
          virtual void setIntVars(const std::map<std::string, int>& vars);
          virtual void setIntVars(const std::vector<std::map<std::string, int> >& vars);

          /**
           * These functions are used by an NWA based implementation of detensor.
           * They provide information about the bit level information per variable, and
           * provide ways to test what part of the vocabulary a certain buddy
           * bdd level belongs to.
           **/
          // Setup the sets of levels in the different vocabularies for fast lookup
          void setupLevelSets();
          
          unsigned numVarsPerVoc();
          bool isRootInVocTensor1Lhs(bdd b); //A
          bool isRootInVocTensor1Rhs(bdd b); //A'
          bool isRootInVocTensor2Lhs(bdd b); //B
          bool isRootInVocTensor2Rhs(bdd b); //B'
          bool isRootRelevant(bdd b);

        protected:
          /**
           * More fine grained control when setting multiple int vars together. These together 
           * called in order are the same as calling setIntVars
           **/
          void createIntVars(const std::vector<std::map<std::string, int> >& vars);
          virtual void setupCachedBdds();
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
  
          // Hash sets to store what bdd levels belong to what part of the voc.
          VocLevelSet tensor1LhsLevels;
          VocLevelSet tensor1RhsLevels;
          VocLevelSet tensor2LhsLevels;
          VocLevelSet tensor2RhsLevels;
          
#ifdef BINREL_STATS
        private:
          //Statistics for this contex
          mutable StatCount numCompose;
          mutable StatCount numUnion;
          mutable StatCount numIntersect;
          mutable StatCount numEqual;
          mutable StatCount numKronecker;
          mutable StatCount numTranspose;
          mutable StatCount numEq23Project;
          mutable StatCount numEq13Project;

          // Related functions
        public:
          std::ostream& printStats( std::ostream& o ) const;
          void resetStats();
#endif

        private:
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

      extern
      bdd
      quantifyOutOtherVariables(BddContext const & voc,
                                std::vector<std::string> const & keep_these,
                                bdd b);
      
      
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
          BinRel(BddContext const * con, bdd b, bool is_tensored=false);
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

          bool isOne() const {
            sem_elem_t one_semelem = one();
            BinRel * one_binrel = dynamic_cast<BinRel*>(one_semelem.get_ptr());
            return getBdd() == one_binrel->getBdd();
          }

          bool isZero() const {
            sem_elem_t zero_semelem = zero();
            BinRel * zero_binrel = dynamic_cast<BinRel*>(zero_semelem.get_ptr());
            return getBdd() == zero_binrel->getBdd();
          }
        
          /** @return [this]->Union( cast<BinRel*>(se) ) */
          sem_elem_t combine(SemElem* se);

          /** @return [this]->Compose( cast<BinRel*>(se) ) */
          sem_elem_t extend(SemElem* se);

          sem_elem_t star();

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
        protected:
          //This has to be a raw/weak pointer.
          //BddContext caches some BinRel objects. It is not BinRel's responsibility to
          //manage memory for BddContext. 
          BddContext const * con;
          bdd rel;
          bool isTensored;
      };



      /// Function object that, given two binary relations as weights,
      /// returns the and/intersection of them.
      ///
      /// Useful for intersecting two WFAs.
      struct AndWeightMaker
        : wali::wfa::WeightMaker
      {
        virtual sem_elem_t make_weight( sem_elem_t lhs, sem_elem_t rhs ) {
          wali::domains::binrel::BinRel
            * l_rel = dynamic_cast<wali::domains::binrel::BinRel*>(lhs.get_ptr()),
            * r_rel = dynamic_cast<wali::domains::binrel::BinRel*>(rhs.get_ptr());
          assert(l_rel && r_rel);
          assert(l_rel->getVocabulary() == r_rel->getVocabulary());

          bdd both = bdd_and(l_rel->getBdd(), r_rel->getBdd());
          
          return new wali::domains::binrel::BinRel(&l_rel->getVocabulary(), both);
        }
      }; // WeightMaker
        
      

    } // namespace binrel

  } // namespace domains

} // namespace wali

// Yo, Emacs!
// Local Variables:
//   c-file-style: "ellemtel"
//   c-basic-offset: 2
// End:

#endif  // wali_binrel_BINREL_GUARD


