/*!
 * $Id: ERule.cpp,v 1.2 2005/06/17 17:42:30 akash Exp $
 *
 * @author Akash Lal
 * @version $Revision: 1.2 $
 */
#include <sstream>
#include <list>
#include "ERule.hpp"
#include "wali/SemElemPair.hpp"

namespace wali
{
  namespace wpds
  {
    namespace ewpds
    {
      typedef ref_ptr<SemElemPair> sem_elem_pair_t;

        ERule::ERule( Config *f_, Config *t_, wali_key_t stk2_, sem_elem_t se_, merge_fn_t mf_ ) :
	  Rule(f_,t_,stk2_,se_), mf(mf_)
        {
	  if(stk2_ != WALI_EPSILON) {
	    if(mf_.get_ptr() == NULL) {
	      mf = new MergeFn(se_);
	    }
	    extended_se = (SemElem *)(new SemElemPair(se->one(), se));
	  } else {
	    extended_se = (SemElem *)(new SemElemPair(se, se->one()));
	  }
        }

        ERule::~ERule()
        {

        }

        std::ostream & ERule::print( std::ostream &o ) const
        {
	  Rule::print(o);
	  o << "\t";
	  if(merge_fn().get_ptr() != NULL) merge_fn()->print(o);
          return o;
        }

        std::ostream & ERule::marshall( std::ostream & o ) const
        {
            o << "<ERule>\n";
            Rule::f->marshall( o << "\t" ) << std::endl;
            Rule::t->marshall( o << "\t" ) << std::endl;
            o << "\t<Stack2 name=\"" << Rule::to_stack2() << "\"/>\n";
            o << "\t<Weight>" << Rule::weight()->to_string() << "</Weight>\n";
	    if(merge_fn().get_ptr() != NULL) {
	      o << "\t<MergeFn>" << merge_fn()->to_string() << "</MergeFn>\n";
	    } else {
	      o << "\t<MergeFn> NONE </MergeFn>\n";
	    }
            o << "</ERule>";
            return o;
        }
    } // namespace ewpds
  } // namespace wpds
} // namespace wali

/*
 * $Log: ERule.cpp,v $
 * Revision 1.2  2005/06/17 17:42:30  akash
 *
 * EWPDSDIR=WPDSDIR/ewpds
 *
 * Revision 1.1  2005/06/17 17:33:38  akash
 *
 * Change EWPDS directory to reflect change in namespace.
 *
 * Revision 1.2  2005/06/17 17:26:07  akash
 *
 * Move ewpds to its own namespace inside ::wali::wpds.
 *
 * Revision 1.1  2005/06/16 22:15:10  akash
 *
 * Introducing EWPDS for WALi. It compiles and works for the Reach semiring.
 *
 */

/* Yo, Emacs!
;;; Local Variables: ***
;;; tab-width: 4 ***
;;; End: ***
*/
