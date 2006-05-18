/*!
 * @author Nick Kidd
 */

#include "wali/Common.hpp"
#include "wali/wfa/Trans.hpp"
#include <iostream>
#include <sstream>

namespace wali
{
    namespace wfa
    {


        int Trans::numTrans = 0;
        const std::string Trans::XMLTag("Trans");
        const std::string Trans::XMLFromTag("from");
        const std::string Trans::XMLStackTag("stack");
        const std::string Trans::XMLToTag("to");

        bool TransLT::operator()( const Trans* a, const Trans* b ) const
        {
            if( a->from() == b->from() ) {
                if( a->stack() == b->stack() ) {
                    return a->to() < b->to();
                }
                else {
                    return a->stack() < b->stack();
                }
            }
            else {
                return a->from() < b->from();
            }
        }

        bool TransEq::operator()( const Trans* a, const Trans* b ) const
        {
            return a->equal(b);
        }

        Trans::Trans() :
            Countable(true),
            kp(WALI_EPSILON,WALI_EPSILON), toStateKey(WALI_EPSILON),
            se(0),delta(0),status(MODIFIED)
        {
            {
                // TODO : R
                numTrans++;
                //std::cerr << "Trans(...) : " << numTrans << std::endl;
            }
        }

        Trans::Trans(
                Key from_,
                Key stack_,
                Key to_,
                sem_elem_t se_ ) :
            Countable(true),
            kp(from_,stack_), toStateKey(to_),
            se(se_), delta(se_), status(MODIFIED) 
        {
            {
                // TODO : R
                numTrans++;
                //std::cerr << "Trans(...) : " << numTrans << std::endl;
            }
        }

        //
        // Creating a new Trans means the delta this Trans
        // knows about is actually the se of param rhs
        //
        Trans::Trans( const Trans & rhs ) :
            Printable(),Countable(true),Markable()
        {
            kp      = rhs.kp;
            toStateKey= rhs.toStateKey;
            se      = rhs.se;
            delta   = rhs.se;
            status  = rhs.status;
            {
                // TODO : R
                numTrans++;
                //std::cerr << "Trans( const Trans& ) : " << numTrans << std::endl;
            }
        }

        Trans::~Trans()
        {
            {
                // TODO : R
                numTrans--;
                //std::cerr << "~Trans()   : " << numTrans << std::endl;
            }
        }

        void Trans::combine_weight( sem_elem_t wnew )
        {
            
            // delta returns ( wnew + se, wnew - se )
            // Use w->delta(se) b/c we want the returned diff
            // to be what is in the new weight (wnew) and not
            // in the existing weight (se)
            std::pair< sem_elem_t , sem_elem_t > p = wnew->delta( se );

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
            printKey(o,from());
            o << " , ";

            printKey(o,stack());
            o << " , ";

            printKey(o,to());
            o << " )";

            o << "\t" << se->toString();
            { // BEGIN DEBUGGING
                // FIXME: make a debugging print
                //o << "\tdelta: " << delta->toString();
            } // END DEBUGGING
            return o;
        }

        std::ostream& Trans::marshall( std::ostream& o ) const
        {
            o << "<" << XMLTag;
            // from
            o << " " << XMLFromTag << "='" << key2str(from()) << "'";
            // stack
            o << " " << XMLStackTag << "='" << key2str(stack()) << "'";
            //to 
            o << " " << XMLToTag << "='" << key2str(to()) << "'>";
            
            weight()->marshall(o);

            o << "</" << XMLTag << ">";
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
