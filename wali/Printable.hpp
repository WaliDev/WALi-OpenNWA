#ifndef wali_PRINTABLE_GUARD
#define wali_PRINTABLE_GUARD 1

/*
 * @author Nicholas Kidd
 * @version $Revision: 1.4 $
 */

#include "wali/Common.hpp"
#include <string>
#include <iostream>

namespace wali
{

    /*!
     * @class Printable
     * @brief Interface defining the print method
     */
    class Printable
    {
        public:

            virtual ~Printable() {}

            virtual std::ostream & print( std::ostream & ) const = 0;

            std::string to_string() const;

    }; // class Printable

} // namespace wali

#endif  // wali_PRINTABLE_GUARD

/* Yo, Emacs!
;;; Local Variables: ***
;;; tab-width: 4 ***
;;; End: ***
*/

