#ifndef WALI_EWPDS_MERGE_FUNCTIONS_H_
#define WALI_EWPDS_MERGE_FUNCTIONS_H_

/*!
 * $Id: MergeFunction.hpp,v 1.1 2005/06/17 17:33:38 akash Exp $
 *
 * @author Akash Lal
 * @version $Revision: 1.1 $
 */

#include "wali/ref_ptr.hpp"
#include "wali/SemElem.hpp"
#include <stdio.h>


namespace wali {
  namespace wpds {
    namespace ewpds {
      class MergeFn : public Printable, public Countable {
    
      public:
	MergeFn(bool countme = true) : Countable(countme), sr_data(NULL) { }
	MergeFn(SemElem *sr, bool countme = true) : Countable(countme), sr_data(sr) { }
	MergeFn(sem_elem_t sr, bool countme = true) : Countable(countme), sr_data(sr) { }

	virtual SemElem *apply_f(SemElem *w1, SemElem *w2);
	virtual sem_elem_t apply_f(sem_elem_t w1, sem_elem_t w2);

	virtual MergeFn *parse_element(const char *s, SemElem *sem);

	virtual std::ostream &print(std::ostream &o) const;

	virtual ~MergeFn() { }

      private:
	sem_elem_t sr_data;

      };

      /*! @class merge_fn_t
       *
       * Wrapper class for ref_ptr that ensures type safety.
       */
      class merge_fn_t {

      public:

	merge_fn_t() : mf(0) {}

	merge_fn_t( MergeFn *ptr ) : mf(ptr) {}

	merge_fn_t( const merge_fn_t& rhs ) : mf( rhs.mf ) {}

	MergeFn * operator->() {
	  return get_ptr();
	}

	MergeFn & operator*() {
	  return *get_ptr();
	}

	merge_fn_t& operator=( const merge_fn_t& rhs ) {
	  mf = rhs.mf;
	  return *this;
	}

	merge_fn_t& operator=( MergeFn * rhs ) {
	  mf = rhs;
	  return *this;
	}

	MergeFn *get_ptr() {
	  return (MergeFn *)mf.get_ptr();
	}

	//
	// const methods
	//
	const MergeFn * operator->() const {
	  return get_ptr();
	}

	const MergeFn & operator*() const {
	  return *get_ptr();
	}

	const MergeFn * get_ptr() const {
	  return (MergeFn *)mf.get_ptr();
	}

	bool is_valid() const {
	  return mf.is_valid();
	}

	bool is_empty() const {
	  return mf.is_empty();
	}

      private:
	ref_ptr< MergeFn > mf;

      };
    } // namespace ewpds
  } // namespace wpds
} // namespacw wali

#endif

/*
 * $Log: MergeFunction.hpp,v $
 * Revision 1.1  2005/06/17 17:33:38  akash
 *
 * Change EWPDS directory to reflect change in namespace.
 *
 * Revision 1.2  2005/06/17 17:26:07  akash
 *
 * Move ewpds to its own namespace inside ::wali::wpds.
 *
 */

/* Yo, Emacs!
;;; Local Variables: ***
;;; tab-width: 4 ***
;;; End: ***
*/
