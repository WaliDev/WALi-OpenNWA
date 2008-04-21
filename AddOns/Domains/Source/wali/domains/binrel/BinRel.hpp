#ifndef wali_binrel_BINREL_GUARD
#define wali_binrel_BINREL_GUARD 1

/*!
 * @author Nicholas Kidd
 *
 * An implementation of binary relations. 
 * This class maps models the relations
 * using BDDs, and specifically with the BuDDy
 * BDD library.
 */

#include <fdd.h>
#include "wali/ref_ptr.hpp"
#include "wali/SemElem.hpp"

namespace wali {

  namespace domains {

    namespace binrel {
      class BinRel;

      typedef wali::ref_ptr<BinRel> binrel_t;

      class BinRel : public wali::SemElem {
        public:
          static int getNumVars();

          static bool initialize(int numVars);

          static binrel_t Make(int from, int to);

          static binrel_t Empty();

          static binrel_t Id();


        public:
          BinRel(const BinRel& that);

          virtual ~BinRel();

          binrel_t Compose( binrel_t that );

          binrel_t Union( binrel_t that );

        public: // SemElem methods
          sem_elem_t one() const { return Id(); }
          sem_elem_t zero() const { return Empty(); }
          sem_elem_t combine(SemElem* se);
          sem_elem_t extend(SemElem* se);
          bool equal(SemElem* se) const;
          std::ostream& print( std::ostream& o ) const;

        protected:
          BinRel(bdd b);

        protected:
          bdd rel;

        private:
          static int NUMVARS;
          static int base;
          // D1 -> D2, and D2 -> D3
          static bddPair* downOne;
          // D3 -> D2
          static bddPair* restore;

          static bdd priv_id();
      };

    } // namespace binrel

  } // namespace domains

} // namespace wali

#endif  // wali_binrel_BINREL_GUARD


