#ifndef wali_PRINTABLE_GUARD
#define wali_PRINTABLE_GUARD 1

/*
 * @author Nicholas Kidd
 */

#include <string>
#include <iostream>

namespace wali
{

  /*!
   * @class Printable
   * @brief Interface defining the print method
   *
   * A Printable object must define a print method.
   *
   * The Printable class provides toString() methods
   * for any Printable object by passing a std::ostringstream
   * to the object's print method.
   */
  class Printable
  {
    public:

      virtual ~Printable() {}

      virtual std::ostream & print( std::ostream & ) const = 0;

      std::string to_string() const;

      std::string toString() const;

  }; // class Printable

} // namespace wali

#endif  // wali_PRINTABLE_GUARD

