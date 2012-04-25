#ifndef wN_binrel_BINRELMANAGER_GUARD
#define wN_binrel_BINRELMANAGER_GUARD

#include "BinRel.hpp"
#include "buddy/fdd.h"

namespace wali
{
  namespace domains
  {
    namespace binrel
    {

      const Voc initialize(int bddMemSize, int cacheSize, Voc voc);
      void binRelDone();

      void addBoolVar(Voc& voc, std::string name);
      void addIntVar(Voc& voc, std::string name, int size);

      std::ostream& dumpManagerData(std::ostream& o);
      /** Makes an expression bdd from the variable */
      bdd From(std::string var);
      bdd True();
      bdd False();
      bdd Const(int val);
      bdd NonDet();

      bdd And(bdd lexpr, bdd rexpr);
      bdd Or(bdd lexpr, bdd rexpr);
      bdd Not(bdd expr);

      bdd Plus(bdd lexpr, bdd rexpr);
      bdd Minus(bdd lexpr, bdd rexpr);
      bdd Times(bdd lexpr, bdd rexpr);
      bdd Div(bdd lexpr, bdd rexpr);

      bdd Assign(std::string var, bdd expr);
      bdd Assume(bdd expr1, bdd expr2);

      bdd tGetRandomTransformer(const Voc& voc, bool isTensored = false);
    }
  }
}

// Yo, Emacs!
// Local Variables:
//   c-file-style: "ellemtel"
//   c-basic-offset: 2
// End:

#endif //wN_binrel_BINRELMANAGER_GUARD

