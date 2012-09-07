/*!
 * @author Nicholas Kidd
 *
 * @version $Id: Config.cpp 1099 2011-06-07 22:22:04Z driscoll $
 */

#include <sstream>
#include "wali/wpds/Config.hpp"

namespace wali
{
  namespace wpds
  {

    int Config::numConfigs = 0;

    Config::Config( wali_key_t the_state, wali_key_t the_stack ) :
      kp(the_state,the_stack)
    {
      numConfigs++;
      //*waliErr << "Config(...) : " << numConfigs << std::endl;
    }

    Config::~Config()
    {
      numConfigs--;
      //*waliErr << "~Config()   : " << numConfigs << std::endl;
    }

    std::ostream & Config::print( std::ostream & o ) const
    {
      o << "(" << key2str(state());
      o << " , " << key2str(stack()) << ")";
      return o;
    }

    std::ostream & Config::marshall( std::ostream & o ) const
    {
      o << "<Config>";
      o << "\t<state name=\"" << key2str(state()) << "\"/>";
      o << "\t<stack name=\"" << key2str(stack()) << "\"/>";
      o << "</Config>";
      return o;
    }
  } // namespace wpds

} // namespace wali

