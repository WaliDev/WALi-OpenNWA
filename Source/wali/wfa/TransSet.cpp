/*!
 * @author Nick Kidd
 */

#include "wali/wfa/TransSet.hpp"
#include "wali/wfa/TransFunctor.hpp"

namespace wali {

    namespace wfa {

        Trans* TransSet::erase( Trans* t ) {
            Trans* tret = NULL;
            iterator it = find(t);
            if( it != end() ) {
                tret = *it;
                impl.erase(it);
            }
            return tret;
        }

        Trans* TransSet::erase( Key from, Key stack, Key to ) {
            Trans terase(from,stack,to,0);
            return erase(&terase);
        }

        TransSet::iterator TransSet::find( Key from, Key stack, Key to ) {
            Trans terase(from,stack,to,0);
            return impl.find(&terase);
        }

        TransSet::const_iterator TransSet::find( Key from, Key stack, Key to ) const {
            Trans terase(from,stack,to,0);
            return impl.find(&terase);
        }

        TransSet::iterator TransSet::find( Trans* t ) {
            return impl.find(t);
        }

        TransSet::const_iterator TransSet::find( Trans* t ) const {
            return impl.find(t);
        }

        void TransSet::each( TransFunctor& tf )
        {
            iterator it = begin();
            iterator END = end();
            for( ; it != END ; it++ ) {
                tf(*it);
            }
        }

        void TransSet::each( ConstTransFunctor& tf ) const
        {
            const_iterator it = begin();
            const_iterator END = end();
            for( ; it != END ; it++ ) {
                tf(*it);
            }
        }

    }
}
/* Yo, Emacs!
   ;;; Local Variables: ***
   ;;; tab-width: 4 ***
   ;;; End: ***
*/

