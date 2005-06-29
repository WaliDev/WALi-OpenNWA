/*!
 * $Id: WeightMaker.cpp,v 1.4 2005/06/17 14:01:43 kidd Exp $
 *
 * @author Nick Kidd
 * @version $Revision: 1.4 $
 */

#include "wali/SemElemPair.hpp"
#include "wali/wfa/WeightMaker.hpp"

namespace wali
{
    namespace wfa
    {
        sem_elem_t KeepLeft::make_weight( sem_elem_t lhs, sem_elem_t rhs UNUSED_PARAM )
        {
            return lhs;
        }

        sem_elem_t KeepBoth::make_weight( sem_elem_t lhs, sem_elem_t rhs )
        {
            return new SemElemPair(lhs,rhs);
        }

    } // namespace wfa

} // namespace wali

/*
 * $Log: WeightMaker.cpp,v $
 * Revision 1.4  2005/06/17 14:01:43  kidd
 * Cleaned up includes.
 *
 * Revision 1.3  2005/06/16 23:26:45  kidd
 * Change __attribute__((unused)) to UNUSED_PARAM macro for interoperability with
 * VC++
 *
 * Revision 1.2  2005/06/16 19:13:49  kidd
 * Added __attribute__((unused)) to KeepLeft.
 *
 * Revision 1.1  2005/06/07 18:57:24  kidd
 *
 * WeightMaker specifies the interface used by WFA::intersect in joining weights.
 * KeepLeft and KeepBoth is defined for the user.
 *
 */

/* Yo, Emacs!
;;; Local Variables: ***
;;; tab-width: 4 ***
;;; End: ***
*/

