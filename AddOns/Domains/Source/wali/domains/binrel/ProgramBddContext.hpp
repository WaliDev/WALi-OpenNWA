#ifndef wN_binrel_BINRELMANAGER_GUARD
#define wN_binrel_BINRELMANAGER_GUARD

// ::wali::domains::binrel
#include "BinRel.hpp"
// ::wali
#include "wali/ref_ptr.hpp"
#include "wali/Countable.hpp"

#include "buddy/fdd.h"

#define BINREL_I_WANT_MINUS_TIMES_AND_DIV_EVEN_THOUGH_THEY_CAN_BE_EXPONENTIALLY_SLOW
#define BINREL_BUILD_FAST_ADDER 0
#define BINREL_BUILD_SLOW_ADDER 1

namespace wali
{
  namespace domains
  {
    namespace binrel
    {

      // Forward declare class to define the ref_ptr to ProgramBddContext. 
      class ProgramBddContext;
      typedef ref_ptr< ProgramBddContext > program_bdd_context_t;

      class ProgramBddContext : public BddContext
      {
        public:

          /**
           * Initialize the ProgramBddContext. 
           * @param [bddMemSize] the memory size buddy should use. Use 0 for default.
           * @param [cacheSize] the memory size buddy should use. Use 0 for default.
           **/
          ProgramBddContext(int bddMemSize=0, int cacheSize=0);
          ProgramBddContext(const ProgramBddContext& other);
          virtual ProgramBddContext& operator = (const ProgramBddContext& other);
          //This is left undefined. We need this declaration so that the compiler doesn't think we've
          //hidden the user defined operator = for base class BddContext. We leave it undefined so that 
          //you get a link error if you try to call this.
          //virtual ProgramBddContext& operator = (const BddContext& other);
          ~ProgramBddContext();

          /** Add a boolean variable to the vocabulary with the name 'name' **/
          virtual void addBoolVar(std::string name);
          /** Add a int variable to the vocabulary with the name 'name'. The integer can take values
           * between 0...size-1. **/
          virtual void addIntVar(std::string name, unsigned size);

          std::ostream& print(std::ostream& o);

          // ////////////////Create a expression for the variable var///////////////////////
          bdd From(std::string var) const;
          // ////////////////Non deterministic expression///////////////////////////////////
          bdd NonDet() const;
          // ////////////////Boolen Expression Generators///////////////////////////////////
          bdd True() const;
          bdd False() const;

          bdd And(bdd lexpr, bdd rexpr) const;
          bdd Or(bdd lexpr, bdd rexpr) const;
          bdd Not(bdd expr) const;

          // //////////////Integer Expression Generators///////////////////////////////////
          bdd Const(unsigned val) const;

          bdd Plus(bdd lexpr, bdd rexpr) const;
#ifdef BINREL_I_WANT_MINUS_TIMES_AND_DIV_EVEN_THOUGH_THEY_CAN_BE_EXPONENTIALLY_SLOW
          bdd Minus(bdd lexpr, bdd rexpr) const;
          bdd Times(bdd lexpr, bdd rexpr) const;
          bdd Div(bdd lexpr, bdd rexpr) const;
#endif

          // //////////////Statement Generators/////////////////////////////////////////////
          // Statements can not be composed.
          bdd Assign(std::string var, bdd expr) const;
          bdd Assume(bdd expr1, bdd expr2) const;

          // //////////////Generates a random bdd///////////////////////////////////////////
          bdd tGetRandomTransformer(bool isTensored = false, unsigned seed=0);

        private:
          // This is the bdd index of the size info in each bdd
          unsigned sizeInfo;
          // This is the maximum size a register can have
          unsigned maxSize; 
          // These store indices for the "base" only bdd levels that we use for manipulation
          bddinfo_t regAInfo, regBInfo;

          bdd bddAnd() const;
          bdd bddOr() const;
          bdd bddNot() const;
          bdd bddPlus(unsigned in1Size, unsigned in2Size) const;
          bdd bddMinus(unsigned in1Size, unsigned in2Size) const;
          bdd bddTimes(unsigned in1Size, unsigned in2Size) const;
          bdd bddDiv(unsigned in1Size, unsigned in2Size) const;

          //Helper functions
          unsigned getRegSize(bdd forThis) const;
          bdd applyBinOp(bdd lexpr, bdd rexpr, bdd op) const;
          bdd applyUnOp(bdd expr, bdd op) const;
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

