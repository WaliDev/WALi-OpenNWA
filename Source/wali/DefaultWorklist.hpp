#ifndef wali_DEFAULT_WORKLIST_GUARD
#define wali_DEFAULT_WORKLIST_GUARD 1

/*!
 * @author Nick Kidd
 */

#include "wali/Common.hpp"
#include "wali/Worklist.hpp"
#include <list>

namespace wali
{
    /*! @class DefaultWorklist
     *
     * The default DefaultWorklist acts as a Stack and uses
     * std::list to hold items.
     */

    template< typename T > class DefaultWorklist : public ::wali::Worklist<T>
    {
        public:

            DefaultWorklist() : Worklist<T>() {}

            virtual ~DefaultWorklist() {
                clear();
            }

            /*!
             * put
             *
             */
            virtual void put( T *item ) {
                if( !item->marked() ) {
                    item->mark();
                    wl.push_back( item );
                }
            }

            /*!
             * get
             *
             * Return an item from the worklist.
             * Returns NULL if the DefaultWorklist is empty.
             * In the future it may throw an exception
             *
             * @return T *
             */
            virtual T * get() {
                T * item = 0;
                if( !empty() ) {
                    item = wl.back();
                    wl.pop_back();
                    item->unmark();
                }
                return item;
            }

            /*!
             * emtpy
             *
             * @return true if the DefaultWorklist is empty
             */
            virtual bool empty() const {
                return wl.empty();
            }

            /*!
             * clear
             *
             * Remove and unmark each item in this worklist.
             */
            virtual void clear() {
                typename std::list< T* >::iterator it = wl.begin();

                // iterate through unmarking items
                for( ; it != wl.end() ; it++ ) {
                    T * item = *it;
                    item->unmark();
                }
                // clear the list
                wl.clear();
            }

        protected:
            std::list< T* > wl; //!< The default worklist data structure

    }; // class DefaultWorklist

} // namespace wali

#endif // wali_DEFAULT_WORKLIST_GUARD

/* Yo, Emacs!
   ;;; Local Variables: ***
   ;;; tab-width: 4 ***
   ;;; End: ***
 */
