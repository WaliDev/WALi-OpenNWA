#ifndef wpds_STATE_GUARD
#define wpds_STATE_GUARD 1

/*!
 * @author Nick Kidd
 */

#include "wali/Common.hpp"
#include "wali/Printable.hpp"
#include "wali/Markable.hpp"
#include "wali/Countable.hpp"
#include "wali/SemElem.hpp"
#include "wali/wfa/TransSet.hpp"
#include <list>

namespace wali
{
    namespace wpds
    {
        class WPDS;
        namespace ewpds 
        {
            class EWPDS;
        }
    }

    namespace wfa
    {
        class CA;
        class State;
        //typedef State* state_t;
        //typedef ref_ptr<State> state_t;

        /*!
         * @class State
         *
         * This class represents a state in a CA. It extends
         * Markable so States can be in a Worklist for querying
         * a WFA.
         *
         * @see WFA
         * @see SemElem
         */
        class State : public Printable, public Markable, public Countable
        {
            public: // friends
                friend class WFA;
                friend class wali::wpds::WPDS;
                friend class wali::wpds::ewpds::EWPDS;

            public: // typedefs
                typedef TransSet::iterator iterator;
                typedef TransSet::const_iterator const_iterator;

            public: // static vars
                static int numStates;
                static const std::string XMLTag;
                static const std::string XMLInitialTag;
                static const std::string XMLFinalTag;
                static const std::string XMLNameTag;

            public:

                State();

                State( Key name, sem_elem_t W );

                virtual ~State();

                /*!
                 * overrides (implements) Printable::print method
                 *
                 * @param o the std::ostream this is written to
                 * @return std::ostream passed in
                 *
                 * @see Printable
                 */
                virtual std::ostream & print( std::ostream & o ) const;

                /*!
                 * Add Trans* t to the set of transitions that lead into this
                 * State.
                 */
                void addTrans( Trans * t );

                /*!
                 * Return a reference to the State's weight
                 */
                sem_elem_t& weight() {
                    return se;
                }

                const sem_elem_t& weight() const {
                    return se;
                }

                /*!
                 * Return reference to the State's delta
                 */
                sem_elem_t& delta() {
                    return delta_se;
                }

                /*!
                 * Return the key associated with this State
                 */
                Key name() const {
                    return key;
                }

                iterator begin()
                {
                    return transSet.begin();
                }

                iterator end()
                {
                    return transSet.end();
                }

                bool eraseTrans(
                        Key from,
                        Key stack,
                        Key to );

                bool eraseTrans( Trans* t );

                void clearTransSet();

                /*!
                 * @brief Used for placing States in STL containers
                 */
                bool operator()( const State* a, const State* b ) const;

            protected:

                TransSet& getTransSet()
                {
                    return transSet;
                }

            protected:
                Key key;
                sem_elem_t se;
                sem_elem_t delta_se;
                sem_elem_t quasi;
                TransSet transSet;
                int tag;            //!< Used by WFA::prune

        }; //class State

    } // namespace wfa

} // namespace wali

#endif  // wpds_STATE_GUARD

/* Yo, Emacs!
   ;;; Local Variables: ***
   ;;; tab-width: 4 ***
   ;;; End: ***
 */
