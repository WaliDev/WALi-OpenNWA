/**
 * @author Nicholas Kidd
 *
 * @version $Id: QueryHandler.cpp 557 2009-10-19 18:51:54Z kidd $
 */

#include "StrX.hpp"

#include "wali/Common.hpp"
#include "wali/Key.hpp"
#include "wali/MergeFnFactory.hpp"
#include "wali/QueryHandler.hpp"
#include "wali/WeightFactory.hpp"
#include "wali/IUserHandler.hpp"

#include "wali/wfa/WFA.hpp"
#include "wali/wfa/WfaHandler.hpp"

#include "wali/wpds/WPDS.hpp"
#include "wali/wpds/DebugWPDS.hpp"
#include "wali/wpds/WpdsHandler.hpp"

#include "wali/wpds/ewpds/EWPDS.hpp"
#include "wali/wpds/ewpds/EWpdsHandler.hpp"

#include "wali/wpds/fwpds/FWPDS.hpp"
#include "wali/wpds/fwpds/SWPDS.hpp"

#include <xercesc/sax2/Attributes.hpp>

#include <iostream>
#include <fstream>


namespace wali
{

  const std::string QueryHandler::XMLTag("Query");
  const std::string QueryHandler::XMLPoststarTag("poststar");
  const std::string QueryHandler::XMLPrestarTag("prestar");

  QueryHandler::QueryHandler( IUserHandler& user ) :
    fUserHandler(user),
    currentHandler(NULL),
    isPrestar(false),
    pdsHandler(NULL),
    faHandler(NULL)
  {
    XMLPlatformUtils::Initialize();
    typeID = XMLString::transcode("type");
  }

  QueryHandler::~QueryHandler()
  {
    XMLString::release(&typeID);
    XMLPlatformUtils::Terminate();
    delete pdsHandler;
    delete faHandler;
  }

  bool QueryHandler::handlesElement( std::string tag )
  {
    return (tag == QueryHandler::XMLTag);
  }

  bool QueryHandler::queryIsPrestar() const
  {
    return isPrestar;
  }

  wfa::WfaHandler* QueryHandler::getWfaHandler()
  {
    return faHandler;
  }

  wpds::WpdsHandler* QueryHandler::getWpdsHandler()
  {
    return pdsHandler;
  }

  wfa::WFA& QueryHandler::result()
  {
    return fa;
  }

  wfa::WFA& QueryHandler::run()
  {
    try {
      // Ensure that everything parsed correctly
      if (pdsHandler != NULL && faHandler != NULL) {
        if( isPrestar ) {
          pdsHandler->get().prestar(faHandler->get(),fa);
        }
        else {
          pdsHandler->get().poststar(faHandler->get(),fa);
        }
      }
    } catch(...) {
      std::cerr << "[ERROR] QueryHandler::run() caught an exception.\n";
      std::cerr << "\tCurrent status of resultant WFA := \n";
      fa.print( std::cerr ) << std::endl;
      std::ofstream of("QH.run.result.dot");
      fa.print_dot(of);
      of.close();
    }
    return fa;
  }

  //////////////////////////////////////////////////
  // Parsing handlers
  //////////////////////////////////////////////////

  void QueryHandler::startDocument()
  {
#if 0
    if (pdsHandler != NULL)
      pdsHandler->startDocument();
    if (faHandler != NULL)
      faHandler->startDocument();
#endif
    fUserHandler.startDocument();
  }

  void QueryHandler::endDocument()
  {
#if 0
    static std::string perrmsg = "[ERROR] No *WPDS has been defined.";
    static std::string ferrmsg = "[ERROR] No WFA has been defined.";
    // Query must have a PDS
    if (pdsHandler == NULL) {
      *waliErr << perrmsg << std::endl;
      throw perrmsg;
    }
    // Query must have a WFA
    if (faHandler == NULL) {
      *waliErr << ferrmsg << std::endl;
      throw ferrmsg;
    }
    pdsHandler->endDocument();
    faHandler->endDocument();
#endif
    fUserHandler.endDocument();
  }

  void QueryHandler::startElement(  
      const   XMLCh* const    uri,
      const   XMLCh* const    localname,
      const   XMLCh* const    qname,
      const   Attributes&     attributes)
  {
    using wpds::WPDS;
    using wpds::DebugWPDS;
    using wpds::ewpds::EWPDS;
    using wpds::fwpds::FWPDS;
    using wpds::fwpds::SWPDS;
    static std::string type_errmsg = "[ERROR] Invalid Query type : ";

    StrX who(localname);
    if( XMLTag == who.get() ) 
    {
      StrX type = attributes.getValue(typeID);
      if (XMLPrestarTag == type.get()) {
        isPrestar = true;
      }
      else if (XMLPoststarTag == type.get()) {
        isPrestar = false;
      }
      else {
        *waliErr << type_errmsg << type.get() << std::endl;
        assert(0);
      }
    }
    else {
      // Separated here special cases QueryHandler tag
      // b/c we want to assert that [currentHandler]
      // is not [NULL] for all other tags.
      if (wfa::WFA::XMLTag == who.get()) 
      {
        if (faHandler == NULL) {
          //faHandler = new wfa::WfaHandler(wf);
          faHandler = new wfa::WfaHandler(fUserHandler);
        }
        currentHandler = faHandler;
      }
      else if (wpds::WPDS::XMLTag == who.get()) 
      {
        if (pdsHandler == NULL) {
          pdsHandler = 
            new wpds::WpdsHandler(
                fUserHandler,new WPDS(fUserHandler.getWrapper()));
        }
        currentHandler = pdsHandler;
      }
      else if (wpds::DebugWPDS::XMLTag == who.get())
      {
        if (pdsHandler == NULL) {
          pdsHandler = 
            new wpds::WpdsHandler(
                fUserHandler,new DebugWPDS(fUserHandler.getWrapper()));
        }
        currentHandler = pdsHandler;
      }
      else if (wpds::ewpds::EWPDS::XMLTag == who.get()) 
      {
        if (pdsHandler == NULL) {
          pdsHandler = 
            new wpds::ewpds::EWpdsHandler(
                fUserHandler,new EWPDS(fUserHandler.getWrapper()));
        }
        currentHandler = pdsHandler;
      }
      else if (wpds::fwpds::FWPDS::XMLTag == who.get()) 
      {
        if (pdsHandler == NULL) {
          pdsHandler = 
            new wpds::ewpds::EWpdsHandler(
              fUserHandler,new FWPDS(fUserHandler.getWrapper()));
        }
        currentHandler = pdsHandler;
      }
      else if (wpds::fwpds::SWPDS::XMLTag == who.get()) 
      {
        if (pdsHandler == NULL) {
          pdsHandler = 
            new wpds::ewpds::EWpdsHandler(
                fUserHandler,new SWPDS(fUserHandler.getWrapper()));
        }
        currentHandler = pdsHandler;
      }
      else if (currentHandler != NULL) {
        currentHandler->startElement(uri,localname,qname,attributes);
      }
      else {
        *waliErr << "[ERROR] QueryHandler::startElement - unhandled element <> '" 
          << who << "'" << std::endl;
        throw who.get();
      }
    }
  }

  void QueryHandler::endElement( 
      const XMLCh* const uri,
      const XMLCh* const localname,
      const XMLCh* const qname)
  {
    StrX who(localname);
    if( XMLTag == who.get() ) {
      // nothing todo
      currentHandler = 0;
    }
    else {
      assert(currentHandler);
      currentHandler->endElement(uri,localname,qname);
    }
  }

  //
  // All characters should be white space
  //
  void QueryHandler::characters(
      const XMLCh* const chars, 
      const XMLSize_t length)
  {
    if( currentHandler ) {
      currentHandler->characters(chars,length);
    }
    else {
      // do nothing. Get calls to characters for all white space 
      // b/w elements
      IWaliHandler::characters(chars,length);
    }
  }

  void QueryHandler::ignorableWhitespace(                               
      const XMLCh* const chars, 
      const XMLSize_t length)
  {
    if( currentHandler ) {
      currentHandler->ignorableWhitespace(chars,length);
    }
    else {
      IWaliHandler::ignorableWhitespace(chars,length);
    }
  }

  void QueryHandler::processingInstruction(   
      const XMLCh* const target, 
      const XMLCh* const data)
  {
    if( currentHandler ) {
      currentHandler->processingInstruction(target,data);
    }
    else {
      IWaliHandler::processingInstruction(target,data);
    }
  }

} // namespace wali


