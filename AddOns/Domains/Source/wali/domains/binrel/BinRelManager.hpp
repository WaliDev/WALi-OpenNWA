#ifndef wN_binrel_BINRELMANAGER_GUARD
#define wN_binrel_BINRELMANAGER_GUARD

// ::wali::domains::binrel
#include "BinRel.hpp"
// ::wali
#include "wali/ref_ptr.hpp"
#include "wali/Countable.hpp"

#include "buddy/fdd.h"

namespace wali
{
  namespace domains
  {
    namespace binrel
    {

      // Forward declare class to define the ref_ptr to BinRelManager. 
      class BinRelManager;
      typedef ref_ptr< BinRelManager > binrel_manager_t;

      // /// The following functions should be used to add variables to a vocabulary *before*
      // /// Creating your BinRelManager Object.
      /** Add a boolean variable to the vocabulary with the name 'name' **/
      void addBoolVar(Voc& voc, std::string name);
      /** Add a int variable to the vocabulary with the name 'name'. The integer can take values
       * between 0...size-1. **/
      void addIntVar(Voc& voc, std::string name, int size);

      class BinRelManager : public Countable
      {
        public:

          /**
           * Initialize the BinRelManager, and the BinRel class. 
           * You do not need to call BinRel::initialize if you call this
           * @param [bddMemSize] the memory size buddy should use. Use 0 for default.
           * @param [cacheSize] the memory size buddy should use. Use 0 for default.
           * @param [voc] The vocabulary of the BinRel semiring. Vocabulary of the BinRel domain
                          can not be changed once the BinRelManager has been created.
                          [TODO] voc is passed by reference, and modified to reflect 
           *              the bdd indices populated.
           * @see BinRel::BddInfo
           **/
          BinRelManager(Voc& voc, int bddMemSize=0, int cacheSize=0);
          ~BinRelManager();

          std::ostream& print(std::ostream& o);

          // ////////////////Create a expression for the variable var///////////////////////
          bdd From(std::string var);
          // ////////////////Non deterministic expression///////////////////////////////////
          bdd NonDet();
          // ////////////////Boolen Expression Generators///////////////////////////////////
          bdd True();
          bdd False();

          bdd And(bdd lexpr, bdd rexpr);
          bdd Or(bdd lexpr, bdd rexpr);
          bdd Not(bdd expr);

          // //////////////Integer Expression Generators///////////////////////////////////
          bdd Const(unsigned val);

          bdd Plus(bdd lexpr, bdd rexpr);
          bdd Minus(bdd lexpr, bdd rexpr);
          bdd Times(bdd lexpr, bdd rexpr);
          bdd Div(bdd lexpr, bdd rexpr);

          // //////////////Statement Generators/////////////////////////////////////////////
          // Statements can not be composed.
          bdd Assign(std::string var, bdd expr);
          bdd Assume(bdd expr1, bdd expr2);

          // //////////////Generates a random bdd///////////////////////////////////////////
          bdd tGetRandomTransformer(bool isTensored = false);

        private:

          //FIXME: add const when initialize() has been removed.
          unsigned BOOLSIZE;

          // This is the bdd index of the size info in each bdd
          unsigned sizeInfo;
          // This is the maximum size a register can have
          unsigned maxSize; 
          // These store indices for the "base" only bdd levels that we use for manipulation
          bddinfo_t regAInfo, regBInfo;

          bdd bddAnd();
          bdd bddOr();
          bdd bddNot();
          bdd bddPlus(unsigned in1Size, unsigned in2Size);
          bdd bddMinus(unsigned in1Size, unsigned in2Size);
          bdd bddTimes(unsigned in1Size, unsigned in2Size);
          bdd bddDiv(unsigned in1Size, unsigned in2Size);

          //Helper functions
          unsigned getRegSize(bdd forThis);
          bdd applyBinOp(bdd lexpr, bdd rexpr, bdd op);
          bdd applyUnOp(bdd expr, bdd op);
      };
    } // namespace binrel
  } //namespace domains
} //namespace wali

// Yo, Emacs!
// Local Variables:
//   c-file-style: "ellemtel"
//   c-basic-offset: 2
// End:

#endif //wN_binrel_BINRELMANAGER_GUARD

