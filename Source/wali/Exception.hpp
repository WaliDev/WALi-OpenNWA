#ifndef wali_EXCEPTION_GUARD
#define wali_EXCEPTION_GUARD 1

/*!
 * @author Nicholas Kidd
 */

#include "wali/Common.hpp"
#include "wali/Printable.hpp"
#include <string>
#include <stdexcept>

namespace wali
{

  /*
   * @class Exception
   */
  class Exception : public Printable
  {
    public:

      Exception();

      Exception( const std::string & s );

      virtual ~Exception();

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
       * @return the msg this Exception holds
       */
      const std::string & message() const
      {
        return msg;
      }

    protected:
      std::string msg;
  };

}
#endif  // wali_EXCEPTION_GUARD

