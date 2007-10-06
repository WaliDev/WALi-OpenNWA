#ifndef WFA_HANDLER_GUARD
#define WFA_HANDLER_GUARD 1

/*!
 * @author Nick Kidd
 */

#include "wali/Common.hpp"
#include "StrX.hpp"
#include "wali/wfa/WFA.hpp"
#include <xercesc/sax2/DefaultHandler.hpp>
#include <string>

XERCES_CPP_NAMESPACE_USE

namespace wali
{

    class WeightFactory;

    namespace wfa
    {
        class WfaHandler : public DefaultHandler
        {
            public:
                WfaHandler( WeightFactory& weightFactory );

                virtual ~WfaHandler();

                //! Return the generated PDS
                WFA& get();
                
                void endDocument();

                void endElement( const XMLCh* const uri,
                        const XMLCh* const localname,
                        const XMLCh* const qname);

                void characters(const XMLCh* const chars, const unsigned int length);

                void ignorableWhitespace(                               
                        const XMLCh* const chars
                        , const unsigned int length
                        );

                void processingInstruction(   
                        const XMLCh* const target
                        , const XMLCh* const data
                        );

                void startDocument();

                void startElement(  const   XMLCh* const    uri,
                        const   XMLCh* const    localname,
                        const   XMLCh* const    qname,
                        const   Attributes&     attributes);

                //////////////////////////////////////////////////
                // Default error handlers
                //////////////////////////////////////////////////
                void warning(const SAXParseException& exc);
                void error(const SAXParseException& exc);
                void fatalError(const SAXParseException& exc);

                //////////////////////////////////////////////////
                // Helpers
                //////////////////////////////////////////////////
                void handleTrans(
                        const XMLCh* const uri
                        , const XMLCh* const localname
                        , const XMLCh* const qname
                        , const Attributes& attributes);

                void handleState(
                        const XMLCh* const uri
                        , const XMLCh* const localname
                        , const XMLCh* const qname
                        , const Attributes& attributes);

                void handleWFA(
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
                XMLCh* stackID;
                XMLCh* toID;
                XMLCh* queryID;

                //! the WFA we parse
                WFA* fa;

                //! place holders
                StrX from;
                StrX stack;
                StrX to;
                wali::Key stateKey;
                bool isInitial;
                bool isFinal;

        }; // class WfaHandler

    } // namespace wfa

} // namespace wali

#endif  // WFA_HANDLER_GUARD

/* Yo, Emacs!
   ;;; Local Variables: ***
   ;;; tab-width: 4 ***
   ;;; End: ***
*/

