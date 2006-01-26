/*!
 * @author Nick Kidd
 */

#include "Reach.hpp"
#include "wali/Common.hpp"

int main() {
    sem_elem_t ONE = new Reach(true);
    sem_elem_t ZERO = ONE->zero();
    assert( ZERO.is_valid());
    return 0;
}
/* Yo, Emacs!
   ;;; Local Variables: ***
   ;;; tab-width: 4 ***
   ;;; End: ***
*/

