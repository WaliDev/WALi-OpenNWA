#ifndef wali_STATE_PRIORITY_WORKLIST_GUARD
#define wali_STATE_PRIORITY_WORKLIST_GUARD 1

/*!
 * @author Akash Lal
 */

#include "wali/Common.hpp"
#include "wali/Worklist.hpp"
#include "wali/wfa/State.hpp"
#include <set>
#include <map>

namespace wali 
{
  namespace wfa 
  {
    namespace epr 
    {

      /*! @class StatePriorityWorklist
       *
       * The worklist is a set. It holds onto states and sorts them based on their
       * priority. State priority is provided by the user.
       */

      class StatePriorityWorklist : public ::wali::Worklist<State> {
        private:
          typedef std::pair<unsigned int, State *> WorkItem;
        public:

          StatePriorityWorklist(std::map< Key, unsigned int > &o) : Worklist<State>(), order(o) {}

          virtual ~StatePriorityWorklist() {
            clear();
          }

          /*!
           * put
           *
           */
          bool put( State *item ) {
            if( !item->marked() ) {
              item->mark();
              wl.insert( WorkItem(lookupPriority(item), item) );
              return true;
            }
            else
              return false;
          }

          /*!
           * get
           *
           * Return an item from the worklist.
           * Returns NULL if the Worklist is empty.
           *
           * @return State *
           */
          State * get() {
            if( !empty() ) {
              std::set< WorkItem >::iterator it = wl.begin();
              State *q = (*it).second;
              wl.erase(it);
              q->unmark();
              return q;
            }
            else
              return 0;
          }

          /*!
           * emtpy
           *
           * @return true if the StatePriorityWorklist is empty
           */
          bool empty() const {
            return wl.empty();
          }

          /*!
           * clear
           *
           * Remove and unmark each item in this worklist.
           */
          void clear() {
            std::set< WorkItem >::iterator it = wl.begin();

            // iterate through unmarking items
            for( ; it != wl.end() ; it++ ) {
              State * item = (*it).second;
              item->unmark();
            }
            // clear the list
            wl.clear();
          }

        private:
          unsigned int lookupPriority(State *s) {
            if(order.find(s->name()) != order.end()) {
              return order[s->name()];
            }
            //*waliErr << "StatePriorityWorlist: Priority lookup failed\n";
            return (unsigned int)(-1);
          }

        private:
          std::set< WorkItem > wl; 
          std::map< Key, unsigned int > order;        

      }; // class StatePriorityWorklist

    } // namespace epr

  } // namespace wfa

} // namespace wali

#endif // wali_STATE_PRIORITY_WORKLIST_GUARD

