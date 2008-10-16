/**
 * @author Nicholas Kidd
 * @version $Id$
 */

#include "wali/Common.hpp"

#include "wali/IWaliHandler.hpp"

#include <xercesc/sax2/Attributes.hpp>

namespace wali
{

  IWaliHandler::IWaliHandler()
  {
  }

  IWaliHandler::~IWaliHandler()
  {
  }

  virtual std::string getCharactersString()
  {
    return fCharStr;
  }

  //////////////////////////////////////////////////
  // Content handlers
  //////////////////////////////////////////////////
  void IWaliHandler::startDocument()
  {
  }

  void IWaliHandler::endDocument()
  {
  }

  void IWaliHandler::startElement(
      const XMLCh* const uri ATTR_UNUSED,
      const XMLCh* const localname,
      const XMLCh* const qname ATTR_UNUSED,
      const Attributes&  attributes ATTR_UNUSED)
  {
    StrX who(localname);
    *waliErr << "[WARNING] Unhandled element : " << who.get() << std::endl;
    fCharStr = "";
  }

  void IWaliHandler::endElement(
      const XMLCh* const uri ATTR_UNUSED, 
      const XMLCh* const localname, 
      const XMLCh* const qname ATTR_UNUSED)
  {
    StrX who(localname);
    *waliErr << "[WARNING] Unhandled element : " << who.get() << std::endl;
  }

  void IWaliHandler::characters(
      const XMLCh* const chars, 
      const unsigned int length ATTR_UNUSED)
  {
    StrX part(chars);
    fCharStr += part.get();
  }

  void IWaliHandler::ignorableWhitespace(                               
      const XMLCh* const chars ATTR_UNUSED, 
      const unsigned int length ATTR_UNUSED)
  {
  }

  void IWaliHandler::processingInstruction(   
      const XMLCh* const target ATTR_UNUSED, 
      const XMLCh* const data ATTR_UNUSED)
  {
  }

  //////////////////////////////////////////////////
  // Default error handlers
  //////////////////////////////////////////////////

  void QueryHandler::warning(const SAXParseException& exc)
  {
    StrX msg(exc.getMessage());
    std::cerr << "[WARNING] " << msg << std::endl;
  }

  void QueryHandler::error(const SAXParseException& exc)
  {
    StrX msg(exc.getMessage());
    std::cerr << "[ERROR] " << msg << std::endl;
  }

  void QueryHandler::fatalError(const SAXParseException& exc)
  {
    StrX msg(exc.getMessage());
    std::cerr << "[FATAL ERROR] " << msg << std::endl;
    throw exc;
  }

}

