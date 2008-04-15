/*!
 * @author Nicholas Kidd
 */

#include "StrX.hpp"

#include "wali/Common.hpp"
#include "wali/Key.hpp"
#include "wali/WeightFactory.hpp"

#include "wali/wfa/State.hpp"
#include "wali/wfa/Trans.hpp"
#include "wali/wfa/WfaHandler.hpp"

#include <xercesc/sax2/Attributes.hpp>

namespace wali
{
  namespace wfa
  {

    WfaHandler::WfaHandler( WeightFactory& wf ) :
      weightFactory(wf), inWeight(false)
    {
      fa = new WFA();
      // Initialize the XML4C2 system
      // Necessary for using transcode
      XMLPlatformUtils::Initialize();
      fromID = XMLString::transcode(Trans::XMLFromTag.c_str());
      stackID = XMLString::transcode(Trans::XMLStackTag.c_str());
      toID = XMLString::transcode(Trans::XMLToTag.c_str());
      queryID = XMLString::transcode(WFA::XMLQueryTag.c_str());
    }

    WfaHandler::~WfaHandler()
    {
      XMLString::release(&fromID);
      XMLString::release(&stackID);
      XMLString::release(&toID);
      XMLString::release(&queryID);
      XMLPlatformUtils::Terminate();
      delete fa;
    }

    WeightFactory& WfaHandler::getWeightFactory() const {
      return weightFactory;
    }

    WFA& WfaHandler::get() {
      return *fa;
    }

    //////////////////////////////////////////////////
    // Parsing handlers
    //////////////////////////////////////////////////

    void WfaHandler::endDocument()
    {
    }

    void WfaHandler::endElement(
        const XMLCh* const uri ATTR_UNUSED
        , const XMLCh* const localname
        , const XMLCh* const qname ATTR_UNUSED)
    {
      using wali::Key;
      StrX who(localname);
      //std::cout << "End Tag = " << who.get() << std::endl;
      if( SemElem::XMLTag == who.get() ) {
        inWeight = false;
      }
      else if( Trans::XMLTag == who.get() ) {
        Key fromKey = getKey(from.get());
        Key stackKey = getKey(stack.get());
        Key toKey = getKey(to.get());

        //std::cout << "Added trans\n";
        fa->addTrans(fromKey
            , stackKey
            , toKey
            , weightFactory.getWeight(weightString)
            );
      }
      else if( State::XMLTag == who.get() ) {
        //std::cout << "Added state\n";
        fa->addState( stateKey, weightFactory.getWeight(weightString) );
        if( isInitial )
          fa->setInitialState(stateKey);
        if( isFinal )
          fa->addFinalState(stateKey);
      }
      else if( WFA::XMLTag == who.get() ) {
      }
      else {
        std::cout << "Non matched tag? " << who.get() << std::endl;
      }
    }

    void WfaHandler::characters(
        const XMLCh* const chars, 
        const unsigned int length ATTR_UNUSED)
    {
      StrX part(chars);
      if( inWeight) {
        weightString += part.get();
      }
      else {
        //std::cout << "Parse error?" << "\t\"" << part << "\" was seen.\n";
      }
    }

    void WfaHandler::ignorableWhitespace(                               
        const XMLCh* const chars ATTR_UNUSED
        , const unsigned int length ATTR_UNUSED)
    {
    }

    void WfaHandler::processingInstruction(   
        const XMLCh* const target ATTR_UNUSED
        , const XMLCh* const data ATTR_UNUSED)
    {
    }

    void WfaHandler::startDocument()
    {
    }

    void WfaHandler::startElement(
        const XMLCh* const uri,
        const XMLCh* const localname,
        const XMLCh* const qname,
        const Attributes&  attributes)
    {
      StrX who(localname);
      //std::cout << "Start Tag = " << who.get() << std::endl;
      if( Trans::XMLTag == who.get()) {
        handleTrans(uri,localname,qname,attributes);
      }
      else if( SemElem::XMLTag == who.get() ) {
        inWeight = true;
        weightString = "";
      }
      else if( State::XMLTag == who.get() ) {
        handleState(uri,localname,qname,attributes);
      }
      else if( WFA::XMLTag == who.get() ) {
        handleWFA(uri,localname,qname,attributes);
      }
      else {
        std::cerr << "[WARNING:WfaHandler] Unrecognized tag.\n";
        std::cerr << "  " << who << "\n";
        for( unsigned i = 0 ; i < attributes.getLength() ; i++ ) {
          StrX lname(attributes.getLocalName(i));
          StrX val(attributes.getValue(i));
          std::cerr << "    " << lname << "\t->\t" << val << "\n";
        }
      }
    }

    //////////////////////////////////////////////////
    // Default error handlers
    //////////////////////////////////////////////////

    void WfaHandler::warning(const SAXParseException& exc ATTR_UNUSED)
    {
    }

    void WfaHandler::error(const SAXParseException& exc ATTR_UNUSED)
    {
    }

    void WfaHandler::fatalError(const SAXParseException& exc ATTR_UNUSED)
    {
    }

    //////////////////////////////////////////////////
    // Helpers
    //////////////////////////////////////////////////
    void WfaHandler::handleTrans(
        const XMLCh* const uri ATTR_UNUSED
        , const XMLCh* const localname ATTR_UNUSED
        , const XMLCh* const qname ATTR_UNUSED
        , const Attributes& attributes)
    {
      from = attributes.getValue(fromID);
      stack = attributes.getValue(stackID);
      to = attributes.getValue(toID);
    }

    void WfaHandler::handleState(
        const XMLCh* const uri ATTR_UNUSED
        , const XMLCh* const localname ATTR_UNUSED
        , const XMLCh* const qname ATTR_UNUSED
        , const Attributes& attributes)
    {

      isFinal = false;
      isInitial = false;
      bool hasName = false;

      for( unsigned i = 0 ; i < attributes.getLength() ; i++ )
      {
        StrX lname(attributes.getLocalName(i));
        StrX val(attributes.getValue(i));

        if( State::XMLNameTag == lname.get() )
        {
          std::string name = val.get();
          assert(name.size());
          stateKey = getKey(name);
          hasName = true;
        }
        else if( State::XMLInitialTag == lname.get() )
        {
          if( std::string("TRUE") == val.get() )
            isInitial = true;
        }
        else if( State::XMLFinalTag == lname.get() )
        {
          if( std::string("TRUE") == val.get() )
            isFinal = true;
        }
        else {
          std::cout << "Parse Error.\n";
          assert(0);
        }
      }
      assert(hasName);
    }

    void WfaHandler::handleWFA(
        const XMLCh* const uri ATTR_UNUSED
        , const XMLCh* const localname ATTR_UNUSED
        , const XMLCh* const qname ATTR_UNUSED
        , const Attributes& attributes)
    {
      const XMLCh* xch = attributes.getValue(queryID);
      if( xch ) {
        StrX s(xch);
        if( WFA::XMLInorderTag == s.get() ) {
          fa->setQuery( WFA::INORDER );
        }
        else if( WFA::XMLReverseTag == s.get() ) {
          fa->setQuery( WFA::REVERSE );
        }
        else {
          // Default to inorder
          fa->setQuery( WFA::INORDER );
        }
      }
    }

  } // namespace wfa

} // namespace wali

/* Yo, Emacs!
   ;;; Local Variables: ***
   ;;; tab-width: 4 ***
   ;;; End: ***
   */
