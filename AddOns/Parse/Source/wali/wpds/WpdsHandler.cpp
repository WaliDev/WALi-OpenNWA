/*!
 * @author Nick Kidd
 */

#include "wali/Common.hpp"
#include "wali/wpds/WpdsHandler.hpp"
#include "StrX.hpp"
#include "wali/WeightFactory.hpp"
#include <xercesc/sax2/Attributes.hpp>
#include "wali/Key.hpp"
#include "wali/wpds/WPDS.hpp"
#include "wali/wpds/Rule.hpp"


namespace wali
{
    namespace wpds
    {

        WpdsHandler::WpdsHandler( WeightFactory& wf, WPDS* thePds ) :
            weightFactory(wf), inWeight(false),pds(thePds)
        {
            if( 0 == pds )
                pds = new WPDS();

            // Initialize the XML4C2 system
            // Necessary for using transcode
            XMLPlatformUtils::Initialize();
            fromStackID = XMLString::transcode(Rule::XMLFromStackTag.c_str());
            fromID = XMLString::transcode(Rule::XMLFromTag.c_str());
            toID = XMLString::transcode(Rule::XMLToTag.c_str());
            toStack1ID = XMLString::transcode(Rule::XMLToStack1Tag.c_str());
            toStack2ID = XMLString::transcode(Rule::XMLToStack2Tag.c_str());
        }

        WpdsHandler::~WpdsHandler()
        {
            XMLString::release(&fromID);
            XMLString::release(&fromStackID);
            XMLString::release(&toID);
            XMLString::release(&toStack1ID);
            XMLString::release(&toStack2ID);
			XMLPlatformUtils::Terminate();
            if( 0 == pds ) {
                assert(0);
            }
            delete pds;
        }

        WPDS& WpdsHandler::get() {
            return *pds;
        }

        //////////////////////////////////////////////////
        // Parsing handlers
        //////////////////////////////////////////////////

        void WpdsHandler::startDocument()
        {
        }

        void WpdsHandler::endDocument()
        {
        }

        void WpdsHandler::startElement(  const   XMLCh* const    uri,
                const   XMLCh* const    localname,
                const   XMLCh* const    qname,
                const   Attributes&     attributes)
        {
            StrX who(localname);
            if( Rule::XMLTag == who.get()) {
                handleRule(uri,localname,qname,attributes);
            }
            else if( SemElem::XMLTag == who.get() ) {
                inWeight = true;
                weightString = "";
            }
            else if( WPDS::XMLTag == who.get() ) {
                // do nothing
            }
            else {
                std::cerr << "[WARNING:WpdsHandler] Unrecognized tag.\n";
                std::cerr << "  " << who << "\n";
                for( unsigned i = 0 ; i < attributes.getLength() ; i++ ) {
                    StrX lname(attributes.getLocalName(i));
                    StrX val(attributes.getValue(i));
                    std::cerr << "    " << lname << "\t->\t" << val << "\n";
                }
            }
        }

        void WpdsHandler::endElement( const XMLCh* const uri,
                const XMLCh* const localname,
                const XMLCh* const qname)
        {
            using wali::Key;
            StrX who(localname);
            if( SemElem::XMLTag == who.get() ) {
                inWeight = false;
            }
            else if( Rule::XMLTag == who.get() ) {
                Key fromKey = getKey(from.get());
                Key fromStkKey = getKey(fromStack.get());
                Key toKey = getKey(to.get());
                Key toStk1Key = getKey(toStack1.get());
                Key toStk2Key = getKey(toStack2.get());

                pds->add_rule(fromKey
                        , fromStkKey
                        , toKey
                        , toStk1Key
                        , toStk2Key
                        , weightFactory.getWeight(weightString)
                        );

                /*
                std::cerr << "Rule\n";
                std::cerr << "\t" << from << std::endl;
                std::cerr << "\t" << fromStack << std::endl;
                std::cerr << "\t" << to << std::endl;
                if( toStack1.get() )
                    std::cerr << "\t" << toStack1 << std::endl;
                if( toStack2.get() )
                    std::cerr << "\t" << toStack2 << std::endl;
                    */
            }
            else {
            }
        }

        void WpdsHandler::characters(const XMLCh* const chars, const unsigned int length)
        {
            StrX part(chars);
            if( inWeight) {
                weightString += part.get();
            }
            else {
                //std::cerr << "Parse error?" << "\t\"" << part << "\" was seen.\n";
            }
        }

        void WpdsHandler::ignorableWhitespace(                               
                const XMLCh* const chars
                , const unsigned int length
                )
        {
        }

        void WpdsHandler::processingInstruction(   
                const XMLCh* const target
                , const XMLCh* const data
                )
        {
        }

        //////////////////////////////////////////////////
        // Default error handlers
        //////////////////////////////////////////////////

        void WpdsHandler::warning(const SAXParseException& exc)
        {
        }

        void WpdsHandler::error(const SAXParseException& exc)
        {
        }

        void WpdsHandler::fatalError(const SAXParseException& exc)
        {
        }

        //////////////////////////////////////////////////
        // Helpers
        //////////////////////////////////////////////////
        void WpdsHandler::handleRule(
                const XMLCh* const uri
                , const XMLCh* const localname
                , const XMLCh* const qname
                , const Attributes& attributes)
        {
            from = attributes.getValue(fromID);
            fromStack = attributes.getValue(fromStackID);
            to = attributes.getValue(toID);
            toStack1 = attributes.getValue(toStack1ID);
            toStack2 = attributes.getValue(toStack2ID);
        }

    } // namespace wpds

} // namespace wali

/* Yo, Emacs!
   ;;; Local Variables: ***
   ;;; tab-width: 4 ***
   ;;; End: ***
 */
