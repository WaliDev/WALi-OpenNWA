#ifndef wali_wfa_ITRANS_GUARD
#define wali_wfa_ITRANS_GUARD 1

/*!
 * @author Nicholas Kidd
 */

#include <iostream>
#include "wali/Common.hpp"
#include "wali/Markable.hpp"
#include "wali/Printable.hpp"
#include "wali/SemElem.hpp"
#include "wali/KeyContainer.hpp"

#include "wali/TaggedWeight.hpp"
#include "wali/util/WeightChanger.hpp"

namespace wali
{
  namespace wpds {
    class Config;
  }

  namespace wfa
  {

    class ITrans;

    /*!
     * @struct ITransLT
     *
     * This can be used to place ITrans* into std::set or any other STL
     * container that uses Strict-weak ordering.
     */
    struct ITransLT {
      bool operator()( const ITrans* a, const ITrans* b ) const;
    };

    struct ITransEq {
      bool operator()( const ITrans* a, const ITrans* b );
    };

    /*!
     * @class ITrans
     *
     * ITrans defines the interface that an automaton transition must
     * implement.
     *
     * An automaton transition is basically a 4-tuple of (p,g,q,W), where
     * p is a "from state", g is a "stack symbol", q is a "to state", and W is
     * a weight, or an instance of a SemElem.
     *
     * IMarkable is to make a ITrans able to be placed in a Worklist.
     *
     * @see Printable
     * @see Markable
     * @see Worklist
     * @see WFA 
     * @see sem_elem_t
     * @see ref_ptr
     */

    class ITrans : public Printable, public virtual IMarkable
    {
      //
      // Types
      //
      public:
        enum status_t { MODIFIED,SAME };
        static const std::string XMLTag;
        static const std::string XMLFromTag;
        static const std::string XMLStackTag;
        static const std::string XMLToTag;

        /**
         *  WFA transitions store some information that the worklist can use.
         */
        struct WorklistData : Countable {
          virtual ~WorklistData() {}
        };

        //
        // Methods
        //
      public:
        /*!
         * @return a copy of the transition
         */
        virtual ITrans* copy() const = 0;
        /*!
         * @return a copy of the transition with changed states and stack
         */
        virtual ITrans* copy(Key fromst, Key stk, Key tost) const = 0;

        //
        // getters (const)
        //
        /*! @return const Key of from state */
        virtual Key from() const throw() = 0;

        /*! @return const Key of stack symbol */
        virtual Key stack() const throw() = 0;

        /*! @return const Key of to state */
        virtual Key to() const throw() = 0;

        /*! @return const sem_elem_t of ITrans */
        virtual const sem_elem_t weight() const throw()  = 0;

        /*! @return the worklist info */
        virtual ref_ptr<WorklistData> worklistData() const throw() = 0;

        /*!
         * @return const sem_elem_t delta of ITrans
         *
         * delta is used in computing fixpoints
         */
        virtual const sem_elem_t getDelta() const throw() = 0;

        //
        // getters (non const)
        //
        /*! @return Key of from state */
        virtual Key from() throw() = 0;

        /*! @return Key of stack symbol */
        virtual Key stack() throw() = 0;

        /*! @return Key of to state */
        virtual Key to() throw() = 0;

        /*! @return sem_elem_t of ITrans */
        virtual sem_elem_t weight() throw() = 0;

        /*!
         * @return sem_elem_t delta of ITrans
         *
         * delta is used in computing fixpoints
         */
        virtual sem_elem_t getDelta() throw() = 0;

        //
        // setters
        //
        /*!
         * Set weight and delta of this ITrans
         * to be param [w].
         *
         * @param sem_elem_t for new weight and delta
         *
         * @return void
         */
        virtual void setWeight( sem_elem_t w )  = 0;

        /*!
         * Set the delta value for the ITrans.
         */
        virtual void setDelta( const sem_elem_t w ) = 0;

        /*! @return the worklist info */
        virtual void setWorklistData(ref_ptr<WorklistData> wd) = 0;

        /*!
         * When inserting a transition into an automaton, 
         * the WFA will check to see if the transition
         * already exists. If so, let told be the existing transition
         * and tnew be the transition being inserted. Then
         * the WFA invokes:
         * <code>   
         *           told->combineTrans( tnew )
         * </code>
         *
         * @param ITrans* the new ITrans to be "absorbed"
         */
        virtual void combineTrans( ITrans* tp )= 0;


        /*!
         * Return const referenct to this transitions KeyPair
         *
         * The KeyPair holds the from state and stack symbol.
         *
         * @see KeyPair
         * @return const KeyPair reference
         */
        virtual const KeyPair & keypair() const throw() = 0;

        /*!
         * @brief return true if the transition has been modified
         *
         * A ITrans is considered modified if when its weight changes. This
         * includes the initial creation of a ITrans object. This follows
         * from the fact that all ITrans can be considered (abstractly)
         * created with a weight of ZERO
         *
         * @return true if this transition has been modified
         */
        virtual bool modified() const throw() = 0;

        /*!
         * @return A null pointer.
         */
        virtual wpds::Config* getConfig() const = 0;

        /*!
         * Set this ITrans's Config to c.
         * @return void
         */
        virtual void setConfig( wpds::Config* c ) = 0;

        /*!
         * This is used by (E)WPDS::poststar
         * during epsilon-transistion contraction.
         * The base case is:
         *   this->weight()->extend(se)
         */
        virtual sem_elem_t poststar_eps_closure( sem_elem_t se ) = 0;

        /*!
         * This is used by FunctionalWeight
         * The base case is:
         *   weight \extend tw.weight
         */
        virtual TaggedWeight apply_post( TaggedWeight tw) const = 0;

        /*!
         * This is used by FunctionalWeight
         * The base case is:
         *   tw.weight \extend weight
         */
        virtual TaggedWeight apply_pre( TaggedWeight tw) const = 0;

        /*!
         * This is used by WeightChanger
         * It applies the changer to all weights stored in the transition
         * Base case is that it is applied to t->weight()
         */
        virtual void applyWeightChanger( util::WeightChanger &wc) = 0;


        //****************************************************
        //****************************************************
        // Methods that ITrans actually provides a default
        // implementation for.
        //

        /*! @return true if param rhs is equal to this */
        virtual bool equal( const ITrans & rhs ) const;

        /*! @return true if param rhs is equal to this */
        virtual bool equal( const ITrans * rhs ) const;

        /*!
         * Print this transition to the ostream. Overrides
         * or implements Printable::print
         *
         * @return std::ostream this was written to
         *
         * @see Printable
         *  
         */
        virtual std::ostream& print( std::ostream& o ) const;

        /*!
         * @brief Print ITrans in XML to ostream o
         *
         * @return std::ostream the marshalled ITrans was written to.
         */
        virtual std::ostream& marshall( std::ostream& o ) const;

    };

  } // namespace wfa

} // namespace wali

#endif  // wali_wfa_ITRANS_GUARD

