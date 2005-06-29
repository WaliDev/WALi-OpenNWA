#ifndef wali_wfa_WEIGHT_MAKER_GUARD
#define wali_wfa_WEIGHT_MAKER_GUARD 1

/*!
 * $Id: WeightMaker.hpp,v 1.2 2005/06/16 23:26:00 kidd Exp $
 *
 * @author Nick Kidd
 * @version $Revision: 1.2 $
 */

#include "wali/Common.hpp"
#include "wali/SemElem.hpp"

namespace wali
{

    namespace wfa
    {
        /*!
         * @class WeightMaker
         *
         * This class defines the interface for joining the weights
         * on WFA transitions during WFA intersection.
         *
         * @see wali::sem_elem_t
         * @see wali::wfa::WFA
         */
        class WeightMaker
        {
            public:
                virtual ~WeightMaker() {}
                virtual sem_elem_t make_weight( sem_elem_t lhs, sem_elem_t rhs ) = 0;
        }; // WeightMaker

        /*!
         * @class KeepLeft
         *
         * This class joins the weights by ignoring the second parameter
         * (rhs).
         *
         * @see wali::wfa::WeightMaker
         * @see wali::sem_elem_t
         */
        class KeepLeft : public WeightMaker
        {
            public:
                KeepLeft() {}
                virtual ~KeepLeft() {}
                virtual sem_elem_t make_weight( sem_elem_t lhs, sem_elem_t rhs );

        }; // KeepLeft

        /*!
         * @class KeepBoth
         *
         * This class joins the weights by pairing them.
         *
         * @see wali::wfa::WeightMaker
         * @see wali::sem_elem_t
         * @see wali::SemElemPair
         */
        class KeepBoth : public WeightMaker
        {
            public:
                KeepBoth() {}
                virtual ~KeepBoth() {}
                virtual sem_elem_t make_weight( sem_elem_t lhs, sem_elem_t rhs );

        }; // KeepBoth

    } // namespace wfa

} // namespace wfa

#endif	// wali_wfa_WEIGHT_MAKER_GUARD

/*
 * $Log: WeightMaker.hpp,v $
 * Revision 1.2  2005/06/16 23:26:00  kidd
 * Ensure Common.hpp is included by each hpp file
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

