/*!
 * $Id: Trans.cpp,v 1.9 2005/06/24 01:14:42 kidd Exp $
 *
 * @author Nick Kidd
 * @version $Revision: 1.9 $
 */
#include <iostream>
#include <sstream>
#include "wali/KeyFactory.hpp"
#include "wali/wfa/Trans.hpp"

namespace wali
{
    namespace wfa
    {

        int Trans::numTrans = 0;

        Trans::Trans() :
            kp(WALI_EPSILON,WALI_EPSILON),t(WALI_EPSILON),se(0),delta(0),status(MODIFIED)
        {
            {
                // TODO : R
                std::cerr << "Trans(...) : " << ++numTrans << std::endl;
            }
        }

        Trans::Trans(
                wali_key_t from,
                wali_key_t stack,
                wali_key_t to,
                sem_elem_t se_ ) :
            kp(from,stack),t(to),se(se_),delta(se_),status(MODIFIED) 
        {
            {
                // TODO : R
                std::cerr << "Trans(...) : " << ++numTrans << std::endl;
            }
        }

        //
        // Creating a new Trans means the delta this Trans
        // knows about is actually the se of param rhs
        //
        Trans::Trans( const Trans & rhs ) :
            Printable(),Countable(),Markable()
        {
            kp      = rhs.kp;
            t       = rhs.t;
            se      = rhs.se;
            delta   = rhs.se;
            status  = rhs.status;
            {
                // TODO : R
                std::cerr << "Trans( const Trans& ) : " << ++numTrans << std::endl;
            }
        }

        Trans::~Trans()
        {
            {
                // TODO : R
                std::cerr << "~Trans()   : " << --numTrans << std::endl;
            }
        }

        // TODO : should delta be set to w or zero?
        void Trans::weight( sem_elem_t w )
        {
            if( se->equal(w) ) {
                status = SAME;
            }
            else {
                status = MODIFIED;
            }
            se = w;
        }

        // TODO : is this correct ?
        void Trans::set_weight( sem_elem_t w )
        {
            se = w;
        }

        void Trans::combine_weight( sem_elem_t w )
        {
            
            // delta returns ( w + se, w - se )
            // Use w->delta(se) b/c we want the returned diff
            // to be what is in the new weight (w) and not
            // in the existing weight (se)
            std::pair< sem_elem_t , sem_elem_t > p = w->delta( se );

            // This's weight is w+se
            se = p.first;

            // Delta is combined with the new delta.
            delta = delta->combine( p.second );

            // Set status
            status = ( delta->equal(delta->zero()) ) ? SAME : MODIFIED;
        }

        std::ostream & Trans::print( std::ostream & o ) const
        {
            o << "( ";
            KeyFactory::print_key(o,from());
            o << " , ";

            KeyFactory::print_key(o,stack());
            o << " , ";

            KeyFactory::print_key(o,to());
            o << " )";

            o << "\t" << se->to_string();
            // FIXME: make a debugging print
            o << "\tdelta: " << delta->to_string();
            return o;
        }

        std::ostream& Trans::marshall( std::ostream& o ) const
        {
            o << "<Trans>\n";
            o << "\t<from_state>";
            o << KeyFactory::key2str( from() );
            o << "</from_state>\n";

            o << "\t<stack>";
            o << KeyFactory::key2str( stack() );
            o << "</stack>\n";

            o << "\t<to_state>";
            o << KeyFactory::key2str( to() );
            o << "</to_state>\n";
            
            o << "\t<weight>";
            weight()->print(o);
            o << "</weight>\n";

            o << "</Trans>\n";
            return o;
        }

        bool Trans::equal( const trans_t & rhs ) const
        {
            return this->equal( rhs.get_ptr() );
        }

        bool Trans::equal( const Trans & rhs ) const
        {
            return ((this == &rhs) ||
                    (
                     (to() == rhs.to()) &&
                     (stack() == rhs.stack()) &&
                     (from() == rhs.from())
                    )
                   );
        }

        bool Trans::equal( const Trans * rhs ) const
        {
            return ((this == rhs) ||
                    (
                     (to() == rhs->to()) &&
                     (stack() == rhs->stack()) &&
                     (from() == rhs->from())
                    )
                   );
        }

    } // namespace wfa

} // namespace wali

/* Yo, Emacs!
;;; Local Variables: ***
;;; tab-width: 4 ***
;;; End: ***
*/
