#ifndef wali_wfa_TRANS_GUARD
#define wali_wfa_TRANS_GUARD 1


/*!
 * @author Nicholas Kidd
 */

#include <iostream>
#include "wali/Common.hpp"
#include "wali/Printable.hpp"
#include "wali/Countable.hpp"
#include "wali/Markable.hpp"
#include "wali/SemElem.hpp"
#include "wali/KeyContainer.hpp"
#include "wali/wfa/ITrans.hpp"


// Disable
//   warning C4250: 'wali::wfa::Trans' : inherits 'wali::Markable::wali::Markable::mark' via dominance
// for MSVC
#if defined(_MSC_VER)
#  pragma warning(push)
#  pragma warning(disable: 4250)
#endif


namespace wali
{
  namespace wpds {
    class Config;
  }

  namespace wfa
  {

    class WFA;
    class Trans;

    /*!
     * @class Trans
     *
     * A Trans is a 4-tuple of from state, stack, to state, and Weight. It
     * represents a transition in the WFA.
     *
     * Markable is to make a Trans able to be placed in a Worklist.
     * Countable is for reference counting.
     *
     * @see Printable
     * @see Countable
     * @see Markable
     * @see Worklist
     * @see WFA 
     * @see sem_elem_t
     * @see ref_ptr
     */

    class Trans : public ITrans, public Markable
    {
      //
      // Types
      //
      public:
        friend class WFA;
        static int numTrans;


        //
        // Methods
        //
      public:
        /*!
         * Construct an empty Trans
         */
        Trans();

        /*!
         * @brief constructor
         *
         * This is used by WFA when addTrans is called.
         */
        Trans(  Key from,
            Key stack,
            Key to,
            sem_elem_t se );

        Trans( const Trans & t );

        Trans( const ITrans & t );

        Trans &operator =(const Trans &t);

        Trans &operator =(const ITrans &t);

        virtual ~Trans();

        virtual Trans* copy() const;
        virtual Trans* copy(Key f, Key s, Key t) const;

        //
        // getters (const)
        //
        /*! @return const Key of from state */
        Key from() const throw() {
          return kp.first;
        }

        /*! @return const Key of from state */
        Key from_state() const throw() {
          return kp.first;
        }

        /*! @return const Key of stack symbol */
        Key stack() const throw() {
          return kp.second;
        }

        /*! @return const Key of to state */
        Key to() const throw() {
          return toStateKey;
        }

        /*! @return const Key of to state */
        Key to_state() const throw() {
          return toStateKey;
        }

        /*! @return const sem_elem_t of Trans */
        virtual const sem_elem_t weight() const throw() {
          return se;
        }

        /*!
         * @return const sem_elem_t delta of Trans
         *
         * delta is used in computing fixpoints
         */
        const sem_elem_t getDelta() const throw() {
          return delta;
        }

        ref_ptr<WorklistData> worklistData() const throw() {
          return worklist_data;
        }

        //
        // getters (non const)
        //
        /*! @return Key of from state */
        Key from() throw() {
          return kp.first;
        }

        /*! @return Key of from state */
        Key from_state()throw() {
          return kp.first;
        }

        /*! @return Key of stack symbol */
        Key stack() throw() {
          return kp.second;
        }

        /*! @return Key of to state */
        Key to() throw() {
          return toStateKey;
        }

        /*! @return Key of to state */
        Key to_state() throw() {
          return toStateKey;
        }

        /*! @return sem_elem_t of Trans */
        virtual sem_elem_t weight() throw() {
          return se;
        }

        /*!
         * @return sem_elem_t delta of Trans
         *
         * delta is used in computing fixpoints
         */
        sem_elem_t getDelta() throw() {
          return delta;
        }

        //
        // setters
        //
        /*!
         * Set weight and delta of this Trans
         * to be param [w].
         *
         * @param sem_elem_t for new weight and delta
         *
         * @return void
         */
        virtual void setWeight( sem_elem_t w ) {
          se = w;
          delta = w;
        }

        /*!
         * Set the delta value for the Trans.
         */
        void setDelta( const sem_elem_t w ) {
          delta = w;
        }

        /*!
         * Sets the worklist data
         */
        void setWorklistData( ref_ptr<WorklistData> wd ) {
          worklist_data = wd;
        }

        /*!
         * Sets the weight of the Trans to be param w.
         * The Trans's status will be set to MODIFIED if
         * param w != this->weight(). Delta is set accordingly.
         *
         * @param w the sem_elem_t for the new weight
         */
        virtual void combineTrans( ITrans* tp );

        /*!
         * Return const referenct to this transitions KeyPair
         *
         * The KeyPair holds the from state and stack symbol.
         *
         * @see KeyPair
         * @return const KeyPair reference
         */
        const KeyPair & keypair() const throw()
        {
          return kp;
        }

        /*!
         * @brief return true if the transition has been modified
         *
         * A Trans is considered modified if when its weight changes. This
         * includes the initial creation of a Trans object. This follows
         * from the fact that all Trans can be considered (abstractly)
         * created with a weight of ZERO
         *
         * @return true if this transition has been modified
         */
        bool modified() const throw()
        {
          return (status == MODIFIED);
        }

        /*!
         * @return A null pointer.
         */
        wpds::Config* getConfig() const;

        /*!
         * Set this Trans's Config to c.
         * @return void
         */
        void setConfig( wpds::Config* c );

        /*!
         * This is used by (E)WPDS::poststar
         * during epsilon-transistion contraction.
         * The base case is:
         *   this->weight()->extend(se)
         */
        virtual sem_elem_t poststar_eps_closure( sem_elem_t se );

        virtual TaggedWeight apply_post( TaggedWeight tw) const;
        virtual TaggedWeight apply_pre( TaggedWeight tw) const;
        virtual void applyWeightChanger( util::WeightChanger &wc);

      protected:
        KeyPair kp;
        Key toStateKey;
        mutable sem_elem_t se;
        sem_elem_t delta;
        ref_ptr<WorklistData> worklist_data;

      protected:  // vars used in Process and not relevant to Trans
        status_t status;
        /*!
         * Used by *WPDS during pre and poststar. This is guaranteed
         * to be NULL when not performing a reachability query.
         */
        mutable wpds::Config *config;
    };

  } // namespace wfa

} // namespace wali


// Restore the warning state
#if defined(_MSC_VER)
#  pragma warning(pop)
#endif

#endif  // wali_wfa_TRANS_GUARD

