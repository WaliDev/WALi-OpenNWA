/**
 * @author Nicholas Kidd
 * @version $Id: Parser.cpp 478 2008-11-24 19:33:16Z kidd $
 */

#include "StrX.hpp"
#include "wali/Parser.hpp"
#include <iostream>

// Xerces stuff
#include <xercesc/util/TransService.hpp>
#include <xercesc/sax2/SAX2XMLReader.hpp>
#include <xercesc/sax2/XMLReaderFactory.hpp>
#include <xercesc/util/OutOfMemoryException.hpp>

namespace wali
{

  int Parser::parse( DefaultHandler& handler, const std::string& xmlFile )
  {
    int rc = 0;

    // Create SAX XML parser
    SAX2XMLReader* parser = XMLReaderFactory::createXMLReader();

    try {
      // Set handlers for content and error
      parser->setContentHandler(&handler);
      parser->setErrorHandler(&handler);
      // Call SAX parser
      parser->parse(xmlFile.c_str());
    }
    catch( const OutOfMemoryException&)
    {
      std::cout << "OutOfMemoryException" << XERCES_STD_QUALIFIER endl;
      rc = 1;
    }
    catch (const XMLException& toCatch)
    {
      std::cout << "\nAn error occurred\n  Error: " <<
        StrX(toCatch.getMessage())
        << "\n" << XERCES_STD_QUALIFIER endl;
      rc = 2;
    }
    catch(const SAXException& toCatch ) {
      // Thrown if the file does not exist
      std::cout << "[SAXException] " << StrX(toCatch.getMessage()) << std::endl;
    }
    if( parser )
      delete parser;
    return rc;
  }

} // namespace wali


