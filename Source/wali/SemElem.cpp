/*!
 * @author Nick Kidd
 */
#include "wali/SemElem.hpp"

namespace wali
{

    const std::string SemElem::XMLTag("Weight");

    SemElem::SemElem( bool countme ) : Countable(countme) {}

    std::ostream& SemElem::marshall( std::ostream& o ) const
    {
        o << "<" << XMLTag << ">";
        o << toString();
        o << "</" << XMLTag << ">";
        return o;
    }

    /*
     * diff returns This - se
     * The default implementation justs lets the diff
     * be the combine of this and se if the comb is
     * not equal to this.  What that means is that if
     * "(this + se ) == this" then "this" already 
     * approximates the changes of se. The algorithm
     * stops the saturation process when all transitions'
     * deltas are zero.
     */
    sem_elem_t SemElem::diff( SemElem * se )
    {
        sem_elem_t comb = combine( se );
        if( equal( comb ) )
        {
            // reclaim this memory
            return zero();
        }
        else
            return comb;
    }

    sem_elem_t SemElem::quasi_one() const
    {
        return one();
    }

    std::pair< sem_elem_t , sem_elem_t > SemElem::delta( SemElem * se )
    {
        std::pair< sem_elem_t , sem_elem_t > rp;
        rp.first = combine(se);
        rp.second = se->diff(this);
        return rp;
    }

} // namespace wali

/* Yo, Emacs!
;;; Local Variables: ***
;;; tab-width: 4 ***
;;; End: ***
*/
