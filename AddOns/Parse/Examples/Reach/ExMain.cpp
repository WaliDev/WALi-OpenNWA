/**
 * @author Nick Kidd
 * @version $Id: ExMain.cpp 634 2009-12-28 17:16:20Z kidd $
 */

#include "StrX.hpp"
#include "Reach.hpp"

#include "wali/Parser.hpp"
#include "wali/QueryHandler.hpp"
// For using old WeightFactory bindings.
#include "wali/UserFactoryHandler.hpp"

#include "wali/wfa/WfaHandler.hpp"

#include "wali/wpds/WpdsHandler.hpp"

#include "wali/util/ParseArgv.hpp"

#include "wali/regex/Regex.hpp"

#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/util/TransService.hpp>
#include <xercesc/sax2/SAX2XMLReader.hpp>
#include <xercesc/sax2/XMLReaderFactory.hpp>
#include <xercesc/util/OutOfMemoryException.hpp>

#include <iostream>
#include <fstream>
#include <sstream>

using std::cout;
using std::cerr;
using std::endl;

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
      cerr << "Parsing wfa file : " << fname << endl;
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
  wali::UserFactoryHandler ufh(&wf,NULL);
  wali::wpds::WpdsHandler handler(ufh);
  int rc = parseFile(handler,xmlFile);
  handler.get().print( std::cout );
  return rc;
}

int parseWfa( std::string& xmlFile )
{
    Reach wf(true);
    wali::UserFactoryHandler ufh(&wf,NULL);
    wali::wfa::WfaHandler handler(ufh);
    int rc = parseFile(handler,xmlFile);
    wali::wfa::WFA& wfa=handler.get();
    wfa.print( std::cout );
    // Run path summary
    wali::regex::regex_t re = wfa.toRegex();
    std::ofstream of((xmlFile + ".dot").c_str());
    if (of.is_open())
    {
        re->write_dot(of);
        of.close();
    }
  return rc;
}

int parseQuery( std::string& xmlFile )
{
  Reach wf(true);
  wali::UserFactoryHandler ufh(&wf,NULL);
  wali::QueryHandler handler(ufh);
  int rc = parseFile(handler,xmlFile);
  if( 0 == rc ) 
  {
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
