#ifndef wali_wfa_DECORATOR_TRANS_GUARD
#define wali_wfa_DECORATOR_TRANS_GUARD 1

/*!
 * @author Nicholas Kidd
 */

#include <iostream>
#include "wali/Common.hpp"
#include "wali/Markable.hpp"
#include "wali/Printable.hpp"
#include "wali/SemElem.hpp"
#include "wali/KeyContainer.hpp"
#include "wali/wfa/ITrans.hpp"

namespace wali
{
  namespace wfa
  {
    /*!
     * @class DecoratorTrans
     *
     * A DecoratorTrans assumes ownership of its delegate, and the destructor
     * deletes the delegate. 
     *
     * Note, delegate transitions should 
     * <b>NOT BE SHARED</b> by two or more decorators.
     *
     */

    class DecoratorTrans : public ITrans
    {
      public:
        DecoratorTrans( ITrans* it ) : delegate(it) {
        }

        virtual ~DecoratorTrans() {
          delete delegate;
        }

        virtual ITrans* copy() const = 0;
        virtual ITrans* copy(Key fromst, Key stk, Key tost) const = 0;

        //
        // getters (const)
        //
        /*! @return const Key of from state */
        virtual Key from() const throw() {
          return delegate->from();
        }

        /*! @return const Key of stack symbol */
        virtual Key stack() const throw() {
          return delegate->stack();
        }

        /*! @return const Key of to state */
        virtual Key to() const throw() {
          return delegate->to();
        }

        /*! @return const sem_elem_t of ITrans */
        virtual const sem_elem_t weight() const throw() {
          return delegate->weight();
        }

        /*!
         * @return const sem_elem_t delta of ITrans
         *
         * delta is used in computing fixpoints
         */
        virtual const sem_elem_t getDelta() const throw() {
          return delegate->getDelta();
        }

        //
        // getters (non const)
        //
        /*! @return Key of from state */
        virtual Key from() throw() {
          return delegate->from();
        }

        /*! @return Key of stack symbol */
        virtual Key stack() throw() {
          return delegate->stack();
        }

        /*! @return Key of to state */
        virtual Key to() throw() {
          return delegate->to();
        }

        /*! @return sem_elem_t of ITrans */
        virtual sem_elem_t weight() throw() {
          return delegate->weight();
        }

        /*!
         * @return sem_elem_t delta of ITrans
         *
         * delta is used in computing fixpoints
         */
        virtual sem_elem_t getDelta() throw() {
          return delegate->getDelta();
        }

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
        virtual void setWeight( sem_elem_t w ) {
          delegate->setWeight(w);
        }

        /*!
         * Set the delta value for the ITrans.
         */
        virtual void setDelta( const sem_elem_t w ) {
          delegate->setDelta( w );
        }

        /*!
         * Print the transition
         */
        virtual std::ostream & print(std::ostream &o) const {
          return delegate->print( o );
        }

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
        virtual void combineTrans( ITrans* tp ) {
          delegate->combineTrans( tp );
        }


        /*!
         * Return const referenct to this transitions KeyPair
         *
         * The KeyPair holds the from state and stack symbol.
         *
         * @see KeyPair
         * @return const KeyPair reference
         */
        virtual const KeyPair& keypair() const throw() {
          return delegate->keypair();
        }

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
        virtual bool modified() const throw() {
          return delegate->modified();
        }

        /*!
         * @return A null pointer.
         */
        virtual wpds::Config* getConfig() const throw() {
          return delegate->getConfig();
        }

        /*!
         * Set this ITrans's Config to c.
         * @return void
         */
        virtual void setConfig( wpds::Config* c ) {
          delegate->setConfig( c );
        }

        /*!
         * This is used by (E)WPDS::poststar
         * during epsilon-transistion contraction.
         * The base case is:
         *   this->weight()->extend(se)
         */
        virtual sem_elem_t poststar_eps_closure( sem_elem_t se ) {
          return delegate->poststar_eps_closure(se);
        }

        //********************
        // Markable delegation
        virtual void mark() const throw() {
          delegate->mark();
        }

        virtual void unmark() const throw() {
          delegate->unmark();
        }

        virtual bool marked() const throw() {
          return delegate->marked();
        }

      protected:
        /*! @return pointer to the delegating ITrans */
        ITrans* getDelegate() {
          return delegate;
        }

        /*! @return const pointer to the delegating ITrans */
        const ITrans* getDelegate() const {
          return delegate;
        }


        ref_ptr<WorklistData> worklistData() const throw() {
          return worklist_data;
        }

        void setWorklistData(ref_ptr<WorklistData> wd) {
          worklist_data = wd;
        }

      protected:
        ITrans* delegate; //! < Decorator delegates all ITrans methods to the delegate
        ref_ptr<WorklistData> worklist_data;
    };

  } // namespace wfa

} // namespace wali

#endif  // wali_wfa_DECORATOR_TRANS_GUARD

