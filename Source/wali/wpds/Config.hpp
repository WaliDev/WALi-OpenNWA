#ifndef wali_wpds_CONFIG_GUARD
#define wali_wpds_CONFIG_GUARD 1

/*!
 * @author Nick Kidd
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
        class EWPDS;
      }
    
	  class WPDS;
        /*! @class Config
         *
         * This class represents a Surface Configuration in the WPDS's
         * configuration space.  I only keeps track of the state and top
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
                friend class ERule;
                friend class WPDS;
                friend class ewpds::EWPDS;

                // For simplicity
                typedef std::list< rule_t >::const_iterator const_iterator;
                typedef std::list< rule_t >::iterator iterator;

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
                const_iterator rbegin() const throw() {
                    return bwrules.begin();
                }

                /*!
                 * @return an end iterator to list of backward rules
                 *
                 * @see Rule
                 * @see std::list
                 */
                const_iterator rend() const throw() {
                    return bwrules.end();
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
                iterator rbegin() throw() {
                    return bwrules.begin();
                }

                /*!
                 * @return an end iterator to list of backward rules
                 *
                 * @see Rule
                 * @see std::list
                 */
                iterator rend() throw() {
                    return bwrules.end();
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

/* Yo, Emacs!
;;; Local Variables: ***
;;; tab-width: 4 ***
;;; End: ***
*/
