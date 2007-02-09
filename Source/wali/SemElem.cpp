/*!
 * @author Nick Kidd
 */
#include "wali/SemElem.hpp"

namespace wali
{

    const std::string SemElem::XMLTag("Weight");

    SemElem::SemElem( bool countme ) : Countable(countme) {}

    std::ostream& SemElem::marshallWeight( std::ostream& o ) const
    {
        o << "<" << XMLTag << ">";
        marshall(o);
        o << "</" << XMLTag << ">";
        return o;
    }

    std::ostream& SemElem::marshall( std::ostream& o ) const
    {
        o << toString();
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
        std::cerr << "[ERROR] SemElem::diff must be overridden to be used.\n";
        assert(0);
        return 0;
    }

    sem_elem_t SemElem::quasi_one() const
    {
        return one();
    }

    //
    // < this + se, this - se >
    //
    std::pair< sem_elem_t , sem_elem_t > SemElem::delta( SemElem * se )
    {
        std::pair< sem_elem_t , sem_elem_t > rp;
        rp.first = combine(se);

        // Because we do not actually have a difference operator,
        // we simply see if the combine is equal to the passed in parameter.
        // If not, we say that our new "delta" or "difference" is the 
        // combined weight
        if( se->equal( rp.first ) ) {
            rp.second = zero();
        }
        else {
            rp.second = rp.first;
        }
        return rp;
    }

    sem_elem_t SemElem::star() {
        sem_elem_t w = combine(one());
        sem_elem_t wn = w->extend(w);
        while(!w->equal(wn)) {
            w = wn;
            wn = wn->extend(wn);
        }
        return wn;
    }

} // namespace wali

/* Yo, Emacs!
;;; Local Variables: ***
;;; tab-width: 4 ***
;;; End: ***
*/
