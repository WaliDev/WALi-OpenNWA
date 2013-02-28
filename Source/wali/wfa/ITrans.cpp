/*!
 * @author Nicholas Kidd
 */

#include "wali/Common.hpp"
#include "wali/wfa/ITrans.hpp"

#include <iostream>
#include <sstream>

namespace wali
{

  namespace wfa
  {


    const std::string ITrans::XMLTag("Trans");
    const std::string ITrans::XMLFromTag("from");
    const std::string ITrans::XMLStackTag("stack");
    const std::string ITrans::XMLToTag("to");

    bool ITransLT::operator()( const ITrans* a, const ITrans* b ) const
    {
      if( a->from() == b->from() ) {
        if( a->stack() == b->stack() ) {
          return a->to() < b->to();
        }
        else {
          return a->stack() < b->stack();
        }
      }
      else {
        return a->from() < b->from();
      }
    }

    bool ITransEq::operator()( const ITrans* a, const ITrans* b ) 
    {
      return a->equalIgnoringWeight(b);
    }

    std::ostream & ITrans::print( std::ostream & o ) const
    {
      o << "( ";
      printKey(o,from());
      o << " , ";

      printKey(o,stack());
      o << " , ";

      printKey(o,to());
      o << " )\t";

      o << "\nweight:\n";
      weight()->print(o) << std::endl;
#if defined(JL_EWPDS_DEBUG)
      o << "delta:\n";
      getDelta()->print(o) << std::endl;
#endif
      { // BEGIN DEBUGGING
        // FIXME: make a debugging print
        //o << "\tdelta: " << delta->toString();
      } // END DEBUGGING
      return o;
    }

    std::ostream& ITrans::marshall( std::ostream& o ) const
    {
      o << "<" << XMLTag;
      // from
      o << " " << XMLFromTag << "='" << key2str(from()) << "'";
      // stack
      o << " " << XMLStackTag << "='" << key2str(stack()) << "'";
      //to 
      o << " " << XMLToTag << "='" << key2str(to()) << "'>";

      weight()->marshallWeight(o);

      o << "</" << XMLTag << ">";
      return o;
    }

    bool ITrans::equalIgnoringWeight(const ITrans & rhs) const
    {
      return ((this == &rhs) ||
          (
           (to() == rhs.to()) &&
           (stack() == rhs.stack()) &&
           (from() == rhs.from())
          )
          );
    }

    bool ITrans::equalIgnoringWeight( const ITrans * rhs ) const
    {
      return ((this == rhs) ||
          (
           (to() == rhs->to()) &&
           (stack() == rhs->stack()) &&
           (from() == rhs->from())
          )
          );
    }

  } // namespace wfa

} // namespace wali

