#ifndef wali_wfa_TRANSSET_GUARD
#define wali_wfa_TRANSSET_GUARD 1

/*!
 * @author Nick Kidd
 */

#include "wali/Common.hpp"
#include "wali/Printable.hpp"
#include "wali/wfa/Trans.hpp"
#include <set>

namespace wali
{
    namespace wfa
    {
        class TransFunctor;
        class ConstTransFunctor;

        /*!
         * @class TransSet
         *
         * This class basically wraps the std::set implementation
         * to provide a "wali::Key friendly" interface.
         */
        class TransSet : public Printable
        {
            public:
                typedef std::set< Trans*,TransLT > SetImpl;
                typedef SetImpl::iterator iterator;
                typedef SetImpl::const_iterator const_iterator;

            public:
                TransSet() {}

                ~TransSet() {}

            public:
                Trans* erase( Trans* t );

                Trans* erase( Key from, Key stack, Key to );

                iterator find( Key from, Key stack, Key to );

                iterator TransSet::find( Trans* t );

                const_iterator find( Key from, Key stack, Key to ) const;

                const_iterator TransSet::find( Trans* t ) const;

                void each( TransFunctor& tf );

                void each( ConstTransFunctor& tf ) const;

                void erase( iterator it ) {
                    impl.erase( it );
                }

                bool insert( Trans* t ) {
                    bool b = impl.insert(t).second;
                    // BEGIN DEBUGGING
                    // We should never insert the same transition twice
                    assert(b);
                    // END DEBUGGING
                    return b;
                }

                void clear() {
                    impl.clear();
                }

                bool empty() const {
                    return impl.empty();
                }

                void clearAndReleaseResources() {
                    SetImpl tmp;
                    tmp.swap(impl);
                }

                iterator begin() {
                    return impl.begin();
                }

                iterator end() {
                    return impl.end();
                }

                const_iterator begin() const {
                    return impl.begin();
                }

                const_iterator end() const {
                    return impl.end();
                }

                std::ostream& print( std::ostream& o ) const;

            protected:
                std::set< Trans*,TransLT > impl;

        }; // class TransSet

    } // namespace wfa

} // namespace wali

#endif  // wali_wfa_TRANSSET_GUARD

/* Yo, Emacs!
   ;;; Local Variables: ***
   ;;; tab-width: 4 ***
   ;;; End: ***
*/

