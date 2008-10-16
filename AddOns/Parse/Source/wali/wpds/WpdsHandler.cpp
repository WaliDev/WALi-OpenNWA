/**
 * @author Nicholas Kidd
 *
 * @version $Id$
 */

#include "StrX.hpp"

#include "wali/Common.hpp"
#include "wali/Key.hpp"
#include "wali/WeightFactory.hpp"

#include "wali/wpds/Rule.hpp"
#include "wali/wpds/WPDS.hpp"
#include "wali/wpds/WpdsHandler.hpp"

#include <xercesc/sax2/Attributes.hpp>

namespace wali
{
  namespace wpds
  {

    const std::string WpdsHandler::FunctionXMLTag("Function");


    WpdsHandler::WpdsHandler( WeightFactory& wf, WPDS* thePds ) :
      pds(thePds), inWeight(false),weightFactory(wf)
    {
      if( NULL == pds )
        pds = new WPDS();

      // Initialize the XML4C2 system
      // Necessary for using transcode
      XMLPlatformUtils::Initialize();
      fromStackID = XMLString::transcode(Rule::XMLFromStackTag.c_str());
      fromID = XMLString::transcode(Rule::XMLFromTag.c_str());
      toID = XMLString::transcode(Rule::XMLToTag.c_str());
      toStack1ID = XMLString::transcode(Rule::XMLToStack1Tag.c_str());
      toStack2ID = XMLString::transcode(Rule::XMLToStack2Tag.c_str());

      // For the function information
      nameID  = XMLString::transcode("name");
      entryID  = XMLString::transcode("entry");
      exitID  = XMLString::transcode("exit");
    }

    WpdsHandler::~WpdsHandler()
    {
      // Rule tags
      XMLString::release(&fromID);
      XMLString::release(&fromStackID);
      XMLString::release(&toID);
      XMLString::release(&toStack1ID);
      XMLString::release(&toStack2ID);
      // Function tags
      XMLString::release(&nameID);
      XMLString::release(&entryID);
      XMLString::release(&exitID);

      XMLPlatformUtils::Terminate();
      if( NULL == pds ) {
        assert(false);
      }
      delete pds;
    }

    bool WpdsHandler::handlesElement(std::string tag)
    {
      return (
          (tag == Rule::XMLTag) ||
          (tag == WPDS::XMLTag) ||
          (tag == WpdsHandler::FunctionXMLTag)
          );
    }

    WPDS& WpdsHandler::get() {
      return *pds;
    }

    const WeightFactory& WpdsHandler::getWeightFactory() const {
      return weightFactory;
    }

    //////////////////////////////////////////////////
    // Parsing handlers
    //////////////////////////////////////////////////

    void WpdsHandler::startElement(  const   XMLCh* const    uri,
        const   XMLCh* const    localname,
        const   XMLCh* const    qname,
        const   Attributes&     attributes)
    {
      StrX who(localname);
      if (Rule::XMLTag == who.get()) {
        handleRule(uri,localname,qname,attributes);
      }
      else if (SemElem::XMLTag == who.get()) {
        inWeight = true;
        weightString = "";
      }
      else if (WPDS::XMLTag == who.get()) {
        // do nothing
      }
      else if (WpdsHandler::FunctionXMLTag == who.get()) {
        StrX fname = attributes.getValue(nameID);
        StrX e = attributes.getValue(entryID);
        StrX x = attributes.getValue(exitID);

        std::string sfun(fname.get());
        std::string se(e.get());
        metaEntry[sfun] = se;
        std::string sx(x.get());
        metaExit[sfun] = sx;
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

    void WpdsHandler::endElement( const XMLCh* const uri ATTR_UNUSED,
        const XMLCh* const localname,
        const XMLCh* const qname ATTR_UNUSED)
    {
      using wali::Key;
      StrX who(localname);
      if( SemElem::XMLTag == who.get() ) {
        inWeight = false;
      }
      else if (WpdsHandler::FunctionXMLTag == who.get()) {
        // do nothing
      }
      else if( Rule::XMLTag == who.get() ) {
        Key fromKey = getKey(from.get());
        Key fromStkKey = getKey(fromStack.get());
        Key toKey = getKey(to.get());
        Key toStk1Key = getKey(toStack1.get());
        Key toStk2Key = getKey(toStack2.get());

#if 0
        std::cerr << "(Rule\n";
        std::cerr << "\t" << from << std::endl;
        std::cerr << "\t" << fromStack << std::endl;
        std::cerr << "\t" << to << std::endl;
        if( toStack1.get() )
          std::cerr << "\t" << toStack1 << std::endl;
        if( toStack2.get() )
          std::cerr << "\t" << toStack2 << std::endl;
        std::cerr << "Weight >>  " << weightString << std::endl;
        std::cerr << ")" << std::endl;
        wali::sem_elem_t se = weightFactory.getWeight(weightString);
        se->print( std::cerr ) << std::endl;
#endif

        pds->add_rule(fromKey
            , fromStkKey
            , toKey
            , toStk1Key
            , toStk2Key
            , weightFactory.getWeight(weightString)
            );

      }
      else {
      }
    }

    void WpdsHandler::characters(
        const XMLCh* const chars, 
        const unsigned int length ATTR_UNUSED)
    {
      StrX part(chars);
      if( inWeight) {
        weightString += part.get();
      }
      else {
        //std::cerr << "Parse error?" << "\t\"" << part << "\" was seen.\n";
      }
    }

    //////////////////////////////////////////////////
    // Helpers
    //////////////////////////////////////////////////
    void WpdsHandler::handleRule(
        const XMLCh* const uri ATTR_UNUSED
        , const XMLCh* const localname ATTR_UNUSED
        , const XMLCh* const qname ATTR_UNUSED
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

