#ifndef wpds_STATE_GUARD
#define wpds_STATE_GUARD 1

/*!
 * $Id: State.hpp,v 1.4 2005/06/17 17:26:57 akash Exp $
 *
 * @author Nick Kidd
 * @version $Revision: 1.4 $
 */

#include "wali/Common.hpp"
#include "wali/Printable.hpp"
#include "wali/Markable.hpp"
#include "wali/Countable.hpp"
#include "wali/SemElem.hpp"
#include <list>

namespace wali
{
    namespace wpds
    {
        class WPDS;
        namespace ewpds 
        {
	    class EWPDS;
	}
    }

    namespace wfa
    {
        class CA;
        class Trans;

        /*!
         * @class State
         *
         * This class represents a state in a CA. It extends
         * Markable so States can be in a Worklist for querying
         * a WFA.
         *
         * @see WFA
         * @see SemElem
         */
        class State : public Printable, public Markable
        {
            public:
                friend class WFA;
                friend class wali::wpds::WPDS;
                friend class wali::wpds::ewpds::EWPDS;

                static int numStates;

            public:

                State( wali_key_t name, sem_elem_t W );

                virtual ~State();

                /*!
                 * overrides (implements) Printable::print method
                 *
                 * @param o the std::ostream this is written to
                 * @return std::ostream passed in
                 *
                 * @see Printable
                 */
                virtual std::ostream & print( std::ostream & o ) const;

                void add_trans( Trans * );

            protected:
                wali_key_t name;
                sem_elem_t se;
                sem_elem_t delta;
                sem_elem_t quasi;
                std::list< Trans * > trans_ls;
        };
    }
}
#endif  // wpds_STATE_GUARD
/*
 * $Log: State.hpp,v $
 * Revision 1.4  2005/06/17 17:26:57  akash
 *
 * Namespace of ewpds has changed.
 *
 * Revision 1.3  2005/06/17 17:21:51  kidd
 * Add emacs tab specifier
 *
 * Revision 1.2  2005/06/16 22:18:58  akash
 *
 * EWPDS is a friend of all classes that consider WPDS to be a friend.
 * Add some extra getters / putters.
 *
 * Revision 1.1.1.1  2005/05/31 19:04:01  kidd
 * initial WALi import
 *
 */

/* Yo, Emacs!
;;; Local Variables: ***
;;; tab-width: 4 ***
;;; End: ***
*/
