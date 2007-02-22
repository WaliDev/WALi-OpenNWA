/*!
 * @author Nick Kidd
 */

#include <sstream>
#include "wali/wpds/Config.hpp"

namespace wali
{
    namespace wpds
    {

        int Config::numConfigs = 0;

        Config::Config( wali_key_t state, wali_key_t stack ) :
            kp(state,stack)
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

/* Yo, Emacs!
;;; Local Variables: ***
;;; tab-width: 4 ***
;;; End: ***
*/
