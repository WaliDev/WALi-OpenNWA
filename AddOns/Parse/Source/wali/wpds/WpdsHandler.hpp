#ifndef wali_wpds_WPDS_HANDLER_GUARD
#define wali_wpds_WPDS_HANDLER_GUARD 1

/*!
 * @author Nicholas Kidd
 */

#include "StrX.hpp"

#include "wali/Common.hpp"

#include "wali/wpds/WPDS.hpp"

#include <string>

#include <xercesc/sax2/DefaultHandler.hpp>

XERCES_CPP_NAMESPACE_USE

namespace wali
{
  class WeightFactory;

  namespace wpds
  {
    class WpdsHandler : public DefaultHandler
    {
      public:
        WpdsHandler( WeightFactory& weightFactory, WPDS* thePds=0 );

        virtual ~WpdsHandler();

        //! Return the generated PDS
        WPDS& get();

        //! @return reference to the WeightFactory
        const WeightFactory& getWeightFactory() const;

        virtual void startDocument();

        virtual void endDocument();

        virtual void startElement(  const   XMLCh* const    uri,
            const   XMLCh* const    localname,
            const   XMLCh* const    qname,
            const   Attributes&     attributes);

        virtual void endElement( const XMLCh* const uri,
            const XMLCh* const localname,
            const XMLCh* const qname);

        virtual void characters(const XMLCh* const chars, const unsigned int length);

        virtual void ignorableWhitespace(                               
            const XMLCh* const chars
            , const unsigned int length
            );

        virtual void processingInstruction(   
            const XMLCh* const target
            , const XMLCh* const data
            );

        //////////////////////////////////////////////////
        // Default error handlers
        //////////////////////////////////////////////////
        virtual void warning(const SAXParseException& exc);
        virtual void error(const SAXParseException& exc);
        virtual void fatalError(const SAXParseException& exc);

        //////////////////////////////////////////////////
        // Helpers
        //////////////////////////////////////////////////
        virtual void handleRule(
            const XMLCh* const uri
            , const XMLCh* const localname
            , const XMLCh* const qname
            , const Attributes& attributes);

      protected:
        WeightFactory& weightFactory;
        //! For getting weight strings
        bool inWeight;
        std::string weightString;

        //! for getting attrs out of rules
        XMLCh* fromID;
        XMLCh* fromStackID;
        XMLCh* toID;
        XMLCh* toStack1ID;
        XMLCh* toStack2ID;

        //! the PDS we parse
        WPDS* pds;

        //! place holders
        StrX from;
        StrX fromStack;
        StrX to;
        StrX toStack1;
        StrX toStack2;

    }; // class WpdsHandler

  } // namespace wpds

} // namespace wali

#endif  // wali_wpds_WPDS_HANDLER_GUARD

