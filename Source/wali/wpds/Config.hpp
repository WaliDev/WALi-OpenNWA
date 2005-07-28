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

                /*!
                 * @return const reference to list of forward rules
                 *
                 * @see Rule
                 * @see std::list
                 */
                const std::list< rule_t > & forward() const throw() {
                    return fwrules;
                }

                /*!
                 * @return const reference to list of forward rules
                 *
                 * @see Rule
                 * @see std::list
                 */
                const std::list< rule_t > & backward() const throw() {
                    return bwrules;
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
                /*!
                 * @return reference to list of forward rules
                 *
                 * @see Rule
                 * @see std::list
                 */
                std::list< rule_t >& forward() throw() {
                    return fwrules;
                }

                /*!
                 * @return reference to list of backward rules
                 *
                 * @see Rule
                 * @see std::list
                 */
                std::list< rule_t >& backward() throw() {
                    return bwrules;
                }

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
