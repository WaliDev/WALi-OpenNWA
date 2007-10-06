#ifndef wali_PARSER_GUARD
#define wali_PARSER_GUARD 1

/*!
 * @author Nick Kidd
 */
#include <string>
#include <xercesc/sax2/DefaultHandler.hpp>

XERCES_CPP_NAMESPACE_USE

/*!
 * @class Parser
 *
 * Generic class that will set up the Xerces C++ XML API and start
 * the DefaultHandler parsing the file specified by xmlFile
 */
namespace wali
{
    class Parser
    {
        public:
            static int parse( DefaultHandler& handler, const std::string& xmlFile );
    };

} // namespace wali

#endif  // wali_PARSER_GUARD

/* Yo, Emacs!
   ;;; Local Variables: ***
   ;;; tab-width: 4 ***
   ;;; End: ***
*/

