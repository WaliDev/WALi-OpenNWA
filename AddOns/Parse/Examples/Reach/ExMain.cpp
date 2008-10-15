/**
 * @author Nick Kidd
 * @version $Id$
 */

#include "wali/QueryHandler.hpp"
#include "wali/wpds/WpdsHandler.hpp"
#include "wali/wfa/WfaHandler.hpp"
#include "wali/util/ParseArgv.hpp"
#include "StrX.hpp"
#include "Reach.hpp"
#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/util/TransService.hpp>
#include <xercesc/sax2/SAX2XMLReader.hpp>
#include <xercesc/sax2/XMLReaderFactory.hpp>
#include <xercesc/util/OutOfMemoryException.hpp>

XERCES_CPP_NAMESPACE_USE

int parseWpds( std::string& xmlFile );
int parseWfa( std::string& xmlFile );
int parseQuery( std::string& xmlFile );
int parseFile( DefaultHandler&, std::string& xmlFile );

void usage() {
  std::cout << "ExMain <options>\n"
    "    --wpds=<fname>\n"
    "    --wfa=<fname>\n"
    "    --query=<fname>\n";
}

int main( int argc, char** argv )
{
  XMLPlatformUtils::Initialize();
  wali::util::ParseArgv parser(argc,argv);
  if (argc == 1 || parser.exists("--help") || parser.exists("-h")) {
    usage();
    return 0;
  }
  std::string fname;
  int rc=0;
  if( parser.get("--wpds",fname) ) {
    rc |= parseWpds(fname);
  }
  if( parser.get("--wfa",fname) ) {
    rc |= parseWfa(fname);
  }
  if( parser.get("--query",fname) ) {
    rc |= parseQuery(fname);
  }
  XMLPlatformUtils::Terminate();
  return rc;
}

int parseWpds( std::string& xmlFile )
{
  Reach wf(true);
  wali::wpds::WpdsHandler handler(wf);
  int rc = parseFile(handler,xmlFile);
  handler.get().print( std::cout );
  return rc;
}

int parseWfa( std::string& xmlFile )
{
  Reach wf(true);
  wali::wfa::WfaHandler handler(wf);
  int rc = parseFile(handler,xmlFile);
  handler.get().print( std::cout );
  return rc;
}

int parseQuery( std::string& xmlFile )
{
  Reach wf(true);
  wali::QueryHandler handler(wf);
  int rc = parseFile(handler,xmlFile);
  if( 0 == rc ) {
    std::cout << "Query Result\n";
    std::cout << "----------------------------------------\n";
    handler.run().print( std::cout );
  }
  return rc;
}

int parseFile( DefaultHandler& handler,std::string& xmlFile )
{
  int errorCode = 0;

  // Initialize the XML4C2 system
  try {    
    XMLPlatformUtils::Initialize();
  }   
  catch (const XMLException& toCatch) {       
    XERCES_STD_QUALIFIER cerr << "Error during initialization! :\n" << StrX(toCatch.getMessage()) << XERCES_STD_QUALIFIER endl;
    return 1; 
  }

  SAX2XMLReader* parser = XMLReaderFactory::createXMLReader();


  try {
    parser->setContentHandler(&handler);
    parser->setErrorHandler(&handler);
    parser->parse(xmlFile.c_str());
  }
  catch( const OutOfMemoryException&)
  {
    XERCES_STD_QUALIFIER cerr << "OutOfMemoryException" << XERCES_STD_QUALIFIER endl;
    errorCode = 2;
  }
  catch (const XMLException& toCatch)
  {
    XERCES_STD_QUALIFIER cerr << "\nAn error occurred\n  Error: "
      << StrX(toCatch.getMessage())
      << "\n" << XERCES_STD_QUALIFIER endl;
    errorCode = 3;
  }
  catch( const SAXException& toCatch ) {
    std::cerr << "Error in Main: " << StrX(toCatch.getMessage()) << std::endl;
    errorCode = 4;
  }

  delete parser;

  XMLPlatformUtils::Terminate();

  return errorCode;
}
