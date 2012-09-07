/**
 * @author Nicholas Kidd
 *
 * @version $Id: WfaHandler.cpp 634 2009-12-28 17:16:20Z kidd $
 */

#include "StrX.hpp"

#include "wali/Common.hpp"
#include "wali/Key.hpp"
#include "wali/WeightFactory.hpp"
#include "wali/IUserHandler.hpp"

#include "wali/wfa/State.hpp"
#include "wali/wfa/Trans.hpp"
#include "wali/wfa/WfaHandler.hpp"

#include <string>
#include <xercesc/sax2/Attributes.hpp>

namespace wali
{
  namespace wfa
  {

    WfaHandler::WfaHandler( IUserHandler& user ) :
      fUserHandler(user)
    {
      fa = new WFA();
      // Initialize the XML4C2 system
      // Necessary for using transcode
      XMLPlatformUtils::Initialize();
      fromID  = XMLString::transcode(Trans::XMLFromTag.c_str());
      stackID = XMLString::transcode(Trans::XMLStackTag.c_str());
      toID    = XMLString::transcode(Trans::XMLToTag.c_str());
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

    bool WfaHandler::handlesElement(std::string tag)
    {
      return (
          (tag == State::XMLTag) ||
          (tag == Trans::XMLTag) ||
          (tag == WFA::XMLTag  )
          );
    }

    WFA& WfaHandler::get() 
    {
      return *fa;
    }

    //////////////////////////////////////////////////
    // Parsing handlers
    //////////////////////////////////////////////////

    void WfaHandler::startElement(
        const XMLCh* const uri,
        const XMLCh* const localname,
        const XMLCh* const qname,
        const Attributes&  attributes)
    {
      StrX who(localname);
      if (Trans::XMLTag == who.get()) {
        handleTrans(uri,localname,qname,attributes);
      }
      else if (State::XMLTag == who.get()) {
        handleState(uri,localname,qname,attributes);
      }
      else if (WFA::XMLTag == who.get()) {
        handleWFA(uri,localname,qname,attributes);
      }
      else if (fUserHandler.handlesElement(who.get())) {
        fUserHandler.startElement(uri,localname,qname,attributes);
      }
      else {
        *waliErr << "[ERROR] WfaHandler::startElement - unrecognized element.\n";
        *waliErr << "  " << who << "\n";
        for( unsigned i = 0 ; i < attributes.getLength() ; i++ ) {
          StrX lname(attributes.getLocalName(i));
          StrX val(attributes.getValue(i));
          *waliErr << "    " << lname << "\t->\t" << val << "\n";
        }
        throw who.get();
      }
    }

    void WfaHandler::endElement(
        const XMLCh* const uri ATTR_UNUSED, 
        const XMLCh* const localname, 
        const XMLCh* const qname ATTR_UNUSED)
    {
      using wali::Key;
      StrX who(localname);
      if( Trans::XMLTag == who.get() ) 
      {
        Key fromKey = getKey(from.get());
        Key stackKey = getKey(stack.get());
        Key toKey = getKey(to.get());

        //std::cout << "Added trans\n";
        fa->addTrans(
            fromKey, stackKey, toKey, 
            fUserHandler.getWeight());
      }
      else if( State::XMLTag == who.get() ) 
      {
        //std::cout << "Added state\n";
        //fa->addState( stateKey, weightFactory.getWeight(weightString) );
        fa->addState( stateKey, fUserHandler.getWeight() );
        if( isInitial )
          fa->setInitialState(stateKey);
        if( isFinal )
          fa->addFinalState(stateKey);
      }
      else if( WFA::XMLTag == who.get() ) {
      }
      else if (fUserHandler.handlesElement(who.get())) {
        fUserHandler.endElement(uri,localname,qname);
      }
      else {
        *waliErr << "[ERROR] WfaHandler::endElement - unrecognized element: " 
          << who.get() << std::endl;
        throw who.get();
      }
    }

    void WfaHandler::characters(const XMLCh* const chars, const XMLSize_t length)
    {
      fUserHandler.characters(chars,length);
    }

    //////////////////////////////////////////////////
    // Helpers
    //////////////////////////////////////////////////
    void WfaHandler::handleTrans(
        const XMLCh* const uri ATTR_UNUSED, 
        const XMLCh* const localname ATTR_UNUSED, 
        const XMLCh* const qname ATTR_UNUSED, 
        const Attributes& attributes)
    {
      from  = attributes.getValue(fromID);
      stack = attributes.getValue(stackID);
      to    = attributes.getValue(toID);
    }

    void WfaHandler::handleState(
        const XMLCh* const uri ATTR_UNUSED, 
        const XMLCh* const localname ATTR_UNUSED, 
        const XMLCh* const qname ATTR_UNUSED, 
        const Attributes& attributes)
    {

      isFinal      = false;
      isInitial    = false;
      bool hasName = false;

      for( unsigned i = 0 ; i < attributes.getLength() ; i++ )
      {
        StrX lname(attributes.getLocalName(i));
        StrX val(attributes.getValue(i));
        static const std::string str_TRUE("TRUE");
        static const std::string str_true("true");

        if( State::XMLNameTag == lname.get() )
        {
          std::string name = val.get();
          assert(name.size());
          stateKey = getKey(name);
          hasName = true;
        }
        else if( State::XMLInitialTag == lname.get() )
        {
          if( str_true == val.get() || str_TRUE == val.get() )
            isInitial = true;
        }
        else if( State::XMLFinalTag == lname.get() )
        {
          if( str_true == val.get() || str_TRUE == val.get() )
            isFinal = true;
        }
        else {
          *waliErr << "[ERROR] WfaHandler - parse error.\n";
          *waliErr << "        " << lname.get() << " = " << val.get() << std::endl;
          throw lname.get();
        }
      }
      assert(hasName);
    }

    void WfaHandler::handleWFA(
        const XMLCh* const uri ATTR_UNUSED, 
        const XMLCh* const localname ATTR_UNUSED, 
        const XMLCh* const qname ATTR_UNUSED, 
        const Attributes& attributes)
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

