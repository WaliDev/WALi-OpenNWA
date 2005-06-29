/*!
 * $Id: MergeFunction.cpp,v 1.1 2005/06/17 17:33:38 akash Exp $
 *
 * @author Akash Lal
 * @version $Revision: 1.1 $
 */

#include "MergeFunction.hpp"

namespace wali
{
  namespace wpds {

	namespace ewpds {
	  SemElem *MergeFn::apply_f(SemElem *w1, SemElem *w2)
	  {
		assert(w1 != NULL && w2 != NULL);

		if(sr_data.is_empty()) {
		  return (w1->extend(w2)).get_ptr();
		}
		else {
		  return (w1->extend(sr_data->extend(w2))).get_ptr();
		}
	  }

	  sem_elem_t MergeFn::apply_f(sem_elem_t w1, sem_elem_t w2)
	  {
		assert(w1.is_valid() && w2.is_valid());

		if(sr_data.is_empty()) {
		  return w1->extend(w2);
		}
		else {
		  return w1->extend(sr_data->extend(w2));
		}
	  }

	  MergeFn *MergeFn::parse_element(const char *s ATTR_UNUSED, SemElem *sem)
	  {
		return new MergeFn(sem);
	  }

	  std::ostream &MergeFn::print(std::ostream &o) const
	  {
		o << "MergeFn[";
		if(sr_data.is_empty()) {
		  o <<"ONE";
		} else {
		  sr_data->print(o);
		}
		o<< "]";
		return o;
	  }
	} // namespace ewpds
  } // namespace wpds
} // namespace wali

/*
 * $Log: MergeFunction.cpp,v $
 * Revision 1.1  2005/06/17 17:33:38  akash
 *
 * Change EWPDS directory to reflect change in namespace.
 *
 * Revision 1.4  2005/06/17 17:26:07  akash
 *
 * Move ewpds to its own namespace inside ::wali::wpds.
 *
 * Revision 1.3  2005/06/17 14:01:37  kidd
 * Added CVS directives, cleaned up includes.
 *
 */

/* Yo, Emacs!
;;; Local Variables: ***
;;; tab-width: 4 ***
;;; End: ***
*/
