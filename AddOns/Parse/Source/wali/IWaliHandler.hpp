#ifndef wali_IWALI_HANDLER_GUARD
#define wali_IWALI_HANDLER_GUARD 1

/**
 * @author Nicholas Kidd
 */

#include "wali/Common.hpp"

#include "wali/Countable.hpp"

#include <xercesc/sax2/DefaultHandler.hpp>

XERCES_CPP_NAMESPACE_USE

namespace wali
{
  class IWaliHandler : public DefaultHandler, public Countable
  {
    public:
      IWaliHandler();

      virtual ~IWaliHandler();

      /**
       * @return true if this handler hanldes an XML
       * element with tag [tag].
       */
      virtual bool handlesElement( std::string tag ) = 0;

      /**
       * @retrun the string that is in the 'characters'
       * of a tag.
       */
      virtual std::string getCharactersString();

      //////////////////////////////////////////////////
      // Content handlers
      //////////////////////////////////////////////////
      virtual void startDocument();

      virtual void endDocument();

      virtual void startElement(  const   XMLCh* const    uri,
          const   XMLCh* const    localname,
          const   XMLCh* const    qname,
          const   Attributes&     attributes);

      virtual void endElement( const XMLCh* const uri,
          const XMLCh* const localname,
          const XMLCh* const qname);

      virtual void characters(const XMLCh *const chars, const XMLSize_t length);

      virtual void ignorableWhitespace(                               
          const XMLCh* const chars, 
          const XMLSize_t length
          );

      virtual void processingInstruction(   
          const XMLCh* const target, 
          const XMLCh* const data
          );

      //////////////////////////////////////////////////
      // Default error handlers
      //////////////////////////////////////////////////
      void warning(const SAXParseException& exc);
      void error(const SAXParseException& exc);
      void fatalError(const SAXParseException& exc);

    protected:
      std::string fCharStr;

  }; // class WaliHandler

} // namespace wali

#endif  // wali_IWALI_HANDLER_GUARD


