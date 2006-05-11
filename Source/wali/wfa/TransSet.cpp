/*!
 * @author Nick Kidd
 */

#include "wali/wfa/TransSet.hpp"
#include "wali/wfa/TransFunctor.hpp"

#if IMPL_LIST
#   define IMPLFIND( impl,t ) std::find<TransSet::iterator,TransSet::iterator,TransEq>(impl.begin(),impl.end(),t)
#else
#   define IMPLFIND( impl,t ) impl.find(t)
#endif

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
            return IMPLFIND(impl,&terase);
        }

        TransSet::const_iterator TransSet::find( Key from, Key stack, Key to ) const {
            Trans terase(from,stack,to,0);
            return find(&terase);
        }

        TransSet::iterator TransSet::find( Trans* t ) {
            return IMPLFIND(impl,t);
        }

        TransSet::const_iterator TransSet::find( Trans* t ) const {
            return IMPLFIND(impl,t);
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
            const_iterator it = impl.begin();
            const_iterator itEND = impl.end();
            for( ; it != itEND ; it++ ) {
                tf(*it);
            }
        }

        bool TransSet::insert( Trans* t )
        {
            bool b = true;
#if IMPL_LIST
            impl.push_back(t);
#else
            b = impl.insert(t).second;
            // BEGIN DEBUGGING
            // We should never insert the same transition twice
            assert(b);
            // END DEBUGGING
#endif
            return b;
        }

        std::ostream& TransSet::print( std::ostream& o ) const {
            const_iterator it = begin();
            const_iterator itEND = end();
            bool first = true;
            o << "{";
            for( ; it != itEND ; it++,first=false ) {
                if( !first )
                    o << ", ";
                (*it)->print(o);
            }
            o << "}";
            return o;
        }
    }
}
/* Yo, Emacs!
   ;;; Local Variables: ***
   ;;; tab-width: 4 ***
   ;;; End: ***
*/

