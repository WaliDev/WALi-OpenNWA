/*!
 * @author Nick Kidd
 */

#include "wali/Common.hpp"
#include "wali/PriorityWorklist.hpp"

namespace wali
{
    ////////
    // class StackSymLT
    ////////
    bool StackSymLT::operator()(
            const wfa::Trans* a,
            const wfa::Trans* b) const
    {
        if( (a->stack() != WALI_EPSILON) && (b->stack() != WALI_EPSILON)) {
            return a->stack() < b->stack();
        }
        else {
            return a->stack() && (b->stack() == WALI_EPSILON);
        }
    }

    ////////
    // class PriorityWorklist
    ////////
    PriorityWorklist::PriorityWorklist() : Worklist<wfa::Trans>() {}

    PriorityWorklist::~PriorityWorklist()
    {
        clear();
    }

    bool PriorityWorklist::put( wfa::Trans *t )
    {
        if( !t->marked() ) {
            t->mark();
            workset.insert(t);
            return true;
        }
        else
            return false;
    }

    wfa::Trans * PriorityWorklist::get() 
    {
        pwl_t::iterator i = workset.begin();
        wfa::Trans* t = (*i);
        workset.erase(i);
        t->unmark();
        return t;
    }

    bool PriorityWorklist::empty() const
    {
        return workset.empty();
    }

    void PriorityWorklist::clear()
    {
        pwl_t::iterator i = workset.begin();
        pwl_t::iterator iEND = workset.end();
        // unmark everything
        for( ; i != iEND ; i++ ) {
            (*i)->unmark();
        }
        workset.clear();
    }
}

/* Yo, Emacs!
   ;;; Local Variables: ***
   ;;; tab-width: 4 ***
   ;;; End: ***
*/

