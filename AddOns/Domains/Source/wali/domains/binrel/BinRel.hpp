#ifndef wali_binrel_BINREL_GUARD
#define wali_binrel_BINREL_GUARD 1

/**
 * @author Nicholas Kidd
 *
 * An implementation of binary relations. 
 * This class models the relations
 * using BDDs, and specifically with the BuDDy
 * BDD library.
 */

#include <fdd.h>
#include <list>
#include <utility>
#include "wali/ref_ptr.hpp"
#include "wali/SemElem.hpp"

namespace wali 
{
  namespace domains 
  {
    namespace binrel 
    {
      class BinRel;

      typedef wali::ref_ptr<BinRel> binrel_t;

      class BinRel : public wali::SemElem 
      {
      public:
	/** @return [int] specifying the number of variables in the domain and range of a binary relation */
	static int getNumVars();

	/** 
	    Raises an exception if v \notin [0,BinRel::getNumVars()).
	    @return void
	 */
	static void check_var(int v);

	/** 
	    Initialize binary relations to have [numVars] in the domain and range.
	    We sometimes refer to "Vars" as the set of all variables.
	    The set "Vars" has the canonical representation [0,numVars).

	    @return true on success 
	*/
	static bool initialize(int numVars);

	/** @return true if BinRel has been successfully initialized */
	static bool is_initialized();

	/** @return relation { [from] -> [to] } */
	static binrel_t Make(int from, int to);

	/** @return relation { a -> b | (a,b) \in [input] } */
	static binrel_t Make(const std::list< std::pair<int,int> >& input);

	/** @return relation {} */
	static binrel_t Empty();

	/** @return relation { a -> a | a \in Vars } */
	static binrel_t Id();

	/** @return binrel_t that implements the relation \lambda S. S + {v} */
	static binrel_t AddVar(int v);

	/** @return binrel_t that implements the relation \lambda S. S - {v} */
	static binrel_t SubVar(int v);
	
      public:
	/** @see BinRel::Compose */
	friend binrel_t operator*(binrel_t a, binrel_t b);

	/** @see BinRel::Union */
	friend binrel_t operator|(binrel_t a, binrel_t b);

	/** @see BinRel::Intersect */
	friend binrel_t operator&(binrel_t a, binrel_t b);

      public:
	BinRel(const BinRel& that);

	virtual ~BinRel();

	binrel_t Compose( binrel_t that );

	binrel_t Union( binrel_t that );

	binrel_t Intersect( binrel_t that );

      public: // SemElem methods
	sem_elem_t one() const { return Id(); }
	sem_elem_t zero() const { return Empty(); }
	
	/** @return [this]->Union( cast<BinRel*>(se) ) */
	sem_elem_t combine(SemElem* se);

	/** @return [this]->Compose( cast<BinRel*>(se) ) */
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
	static bdd cached_id();
      };

    } // namespace binrel

  } // namespace domains

} // namespace wali

#endif  // wali_binrel_BINREL_GUARD


