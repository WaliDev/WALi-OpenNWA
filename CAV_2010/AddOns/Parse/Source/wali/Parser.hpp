#ifndef wali_PARSER_GUARD
#define wali_PARSER_GUARD 1

/**
 * @author Nicholas Kidd
 */

#include <string>
#include <xercesc/sax2/DefaultHandler.hpp>

XERCES_CPP_NAMESPACE_USE

/**
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
      /**
       * Given a handler, creates a SAX parser, parses the file,
       * and deletes the SAX parser. The [handler] then has
       * whatever state is recorded during parsing.
       */
      static int parse( DefaultHandler& handler, const std::string& xmlFile );
  };

} // namespace wali

#endif  // wali_PARSER_GUARD

