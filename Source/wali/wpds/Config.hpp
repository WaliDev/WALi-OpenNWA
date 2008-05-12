#ifndef wali_wpds_CONFIG_GUARD
#define wali_wpds_CONFIG_GUARD 1

/*!
 * @author Nicholas Kidd
 */

#include <list>
#include <string>
#include "wali/Common.hpp"
#include "wali/Printable.hpp"
#include "wali/KeyContainer.hpp"
#include "wali/wpds/Rule.hpp"

namespace wali
{

  namespace wpds
  {

    namespace ewpds 
    {
      class ERule;
      class EWPDS;
    }

    class WPDS;

    /*! @class Config
     *
     * This class represents a Surface Configuration in the WPDS's
     * configuration space.  It only keeps track of the state and top
     * of stack symbol.  All of the wpds::Config's form a graph that
     * is connected forward and back by the list of Rules.
     *
     * @see KeyPair
     * @see Rule
     * @see WPDS
     */
    class Config : public Printable
    {

      public:
        friend class Rule;
        friend class ewpds::ERule;
        friend class WPDS;
        friend class ewpds::EWPDS;

        //
        // Config does not use iterators in the sense that list uses
        // them. It has 2 lists, one w/ rules going forward and 1 w/
        // lists going backwards. When iterating over backward rules,
        // we use a reverse_iterator in the hopes that the different
        // types will avoid some typos (because rbegin/rend look so
        // much like begin and end
        //

        // Forward iterators
        typedef std::list< rule_t >::const_iterator const_iterator;
        typedef std::list< rule_t >::iterator iterator;
        // Backward iterators
        typedef std::list< rule_t >::const_reverse_iterator const_reverse_iterator;
        typedef std::list< rule_t >::reverse_iterator reverse_iterator;

        static int numConfigs;

      public:
        /*! @brief Config constructor
        */
        Config( wali_key_t state, wali_key_t stack );

        virtual ~Config();

        bool operator ==(const Config &c2) const throw() {
          return (kp == c2.kp);
        }

        /*!
         * @return reference KeyPair for this Config
         *
         * @see KeyPair
         */
        const KeyPair & keypair() const throw() {
          return kp;
        }

        /*! @return the state key for this Config */
        wali_key_t state() const throw() {
          return kp.first;
        }

        /*! @return the stack key for this Config */
        wali_key_t stack() const throw() {
          return kp.second;
        }

        /*! @brief insert a rule into forwards list */
        void insert(  rule_t r ) throw() {
          { // BEGIN DEBUGGING
            assert( r->from_state() == state() );
            assert( r->from_stack() == stack() );
          } // END DEBUGGING

          fwrules.push_back(r);
        }

        /*! @brief insert a rule into backwards list */
        void rinsert(  rule_t r ) throw() {
          { // BEGIN DEBUGGING
            assert( r->to_state() == state() );
            assert( r->to_stack1() == stack() );
          } // END DEBUGGING

          bwrules.push_back(r);
        }

        /*!
         * @return const iterator to list of forward rules
         *
         * @see Rule
         * @see std::list
         */
        const_iterator begin() const throw() {
          return fwrules.begin();
        }

        /*!
         * @return an end iterator to list of forward rules
         *
         * @see Rule
         * @see std::list
         */
        const_iterator end() const throw() {
          return fwrules.end();
        }

        /*!
         * @return const iterator to list of backward rules
         *
         * @see Rule
         * @see std::list
         */
        const_reverse_iterator rbegin() const throw() {
          return bwrules.rbegin();
        }

        /*!
         * @return an end iterator to list of backward rules
         *
         * @see Rule
         * @see std::list
         */
        const_reverse_iterator rend() const throw() {
          return bwrules.rend();
        }

        /*!
         * @return a iterator to list of forward rules
         *
         * @see Rule
         * @see std::list
         */
        iterator begin() throw() {
          return fwrules.begin();
        }

        /*!
         * @return an end iterator to list of forward rules
         *
         * @see Rule
         * @see std::list
         */
        iterator end() throw() {
          return fwrules.end();
        }

        /*!
         * @return iterator to list of backward rules
         *
         * @see Rule
         * @see std::list
         */
        reverse_iterator rbegin() throw() {
          return bwrules.rbegin();
        }

        /*!
         * @return an end iterator to list of backward rules
         *
         * @see Rule
         * @see std::list
         */
        reverse_iterator rend() throw() {
          return bwrules.rend();
        }

        /*!
         * Implements Printable::print. Writes this Config
         * to the passed in ostream.
         *
         * @param o the std::ostream Config is written to
         * @return std::ostream Config was written to 
         *
         * @see Printable
         */
        virtual std::ostream & print( std::ostream & o ) const;

        /*!
         * @return std::string XML representation of this Config
         */
        std::ostream & marshall( std::ostream & o ) const;

      protected:

        KeyPair kp;                     //! < pair of state and stack symbol
        std::list< rule_t > fwrules;    //! < forward rules
        std::list< rule_t > bwrules;    //! < backward rules
    };

  } // namespace wpds

} // namespace wali

#endif  // wali_wpds_CONFIG_GUARD

