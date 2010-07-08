/**
 * @author Nicholas Kidd & Evan Driscoll
 *
 * @version $Id: NwaHandler.cpp 650 2010-02-10 20:47:55Z burtona $
 */

#include "StrX.hpp"

#include "wali/Common.hpp"
#include "wali/Key.hpp"
#include "wali/WeightFactory.hpp"
#include "wali/IUserHandler.hpp"

#include "wali/nwa/TransSet.hpp"
#include "wali/nwa/SymbolSet.hpp"

#include "NwaHandler.hpp"

#include <xercesc/sax2/Attributes.hpp>
#include <xercesc/sax2/DefaultHandler.hpp>
#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/util/TransService.hpp>
#include <xercesc/sax2/SAX2XMLReader.hpp>
#include <xercesc/sax2/XMLReaderFactory.hpp>
#include <xercesc/util/OutOfMemoryException.hpp>
#include <xercesc/util/BinInputStream.hpp>
#include <xercesc/sax/InputSource.hpp>


XERCES_CPP_NAMESPACE_USE

// Xerces is dumb and apparently thinks this is useless
//
// http://mail-archives.apache.org/mod_mbox//xerces-general/200002.mbox/%3C06C0B38AD1222036*/c=fr/admd=atlas/prmd=edfgdf/o=notes/s=Filippini/g=Gilles/@MHS%3E
class IStreamBinInputStream : public BinInputStream
{
  std::istream & mIn;
public:
  IStreamBinInputStream(std::istream & in)
    : mIn(in)
  { }

  virtual ~IStreamBinInputStream(void) { }

  virtual XMLFilePos curPos(void) const {
    return mIn.tellg();
  }

  virtual XMLSize_t readBytes(XMLByte * const buf, const XMLSize_t max) {
    mIn.read(reinterpret_cast<char*>(buf), max);
    return mIn.gcount();
  }

  virtual const XMLCh* getContentType() const {
    return 0;
  }
};

// Ditto the above "Xerces is dumb" comment.
class IStreamInputSource : public InputSource
{
  std::istream & mIn;
public:
  IStreamInputSource(std::istream & in)
    : InputSource("istream")
    , mIn(in) 
  { }

  virtual ~IStreamInputSource(void) { }
	
  virtual BinInputStream * makeStream(void) const {
    return new IStreamBinInputStream(mIn);
  }
};




namespace wali
{
  namespace nwa
  {

    NwaHandler::NwaHandler( /*IUserHandler& user*/ )
//      : fUserHandler(user)
    {
      fa = new NWA();
      // Initialize the XML4C2 system
      // Necessary for using transcode
      XMLPlatformUtils::Initialize();
      fromID  = XMLString::transcode(NWA::Trans::XMLFromAttr().c_str());
      predID  = XMLString::transcode(NWA::Trans::XMLPredAttr().c_str());
      symbolID= XMLString::transcode(NWA::Trans::XMLSymbolAttr().c_str());
      toID    = XMLString::transcode(NWA::Trans::XMLToAttr().c_str());
      nameID  = XMLString::transcode(SymbolSet::XMLNameAttr().c_str());
    }

    NwaHandler::~NwaHandler()
    {
      XMLString::release(&fromID);
      XMLString::release(&symbolID);
      XMLString::release(&toID);
      XMLString::release(&predID);
      XMLString::release(&nameID);
      XMLPlatformUtils::Terminate();
    }

    bool NwaHandler::handlesElement(std::string tag)
    {
      return (
          (tag == SymbolSet::XMLSymbolTag()) ||
          (tag == NWA::States::XMLStateTag()) ||
          (tag == NWA::Trans::InternalXMLTag()) ||
          (tag == NWA::Trans::CallXMLTag()) ||
          (tag == NWA::Trans::ReturnXMLTag()) ||
          (tag == NWA::XMLTag()  )
          );
    }

    wali::ref_ptr<NwaHandler::NWA> NwaHandler::get() const
    {
      return fa;
    }

    //////////////////////////////////////////////////
    // Parsing handlers
    //////////////////////////////////////////////////

    void NwaHandler::startElement(
        const XMLCh* const uri,
        const XMLCh* const localname,
        const XMLCh* const qname,
        const Attributes&  attributes)
    {
      StrX who(localname);
      if (NWA::Trans::InternalXMLTag() == who.get()) {
        handleTrans(uri,localname,qname,attributes);
      }
      else if (NWA::Trans::CallXMLTag() == who.get()) {
        handleTrans(uri,localname,qname,attributes);
      }
      else if (NWA::Trans::ReturnXMLTag() == who.get()) {
        handleTrans(uri,localname,qname,attributes);
      }
      else if (NWA::States::XMLStateTag() == who.get()) {
        handleState(uri,localname,qname,attributes);
      }
      else if (SymbolSet::XMLSymbolTag() == who.get()) {
        handleSymbol(uri, localname, qname, attributes);
      }
      else if (NWA::XMLTag() == who.get()) {
        handleNWA(uri,localname,qname,attributes);
      }
//      else if (fUserHandler.handlesElement(who.get())) {
//        fUserHandler.startElement(uri,localname,qname,attributes);
//      }
      else {
        *waliErr << "[ERROR] NwaHandler::startElement - unrecognized element.\n";
        *waliErr << "  " << who << "\n";
        for( unsigned i = 0 ; i < attributes.getLength() ; i++ ) {
          StrX lname(attributes.getLocalName(i));
          StrX val(attributes.getValue(i));
          *waliErr << "    " << lname << "\t->\t" << val << "\n";
        }
        throw who.get();
      }
    }

    void NwaHandler::endElement(
        const XMLCh* const uri ATTR_UNUSED, 
        const XMLCh* const localname, 
        const XMLCh* const qname ATTR_UNUSED)
    {
      using wali::Key;
      StrX who(localname);
      if( NWA::Trans::InternalXMLTag() == who.get() ) 
      {
        Key fromKey = getKey(from.get());
        Key symbolKey = getKey(symbol.get());
        Key toKey = getKey(to.get());

        //std::cout << "Added trans\n";
        fa->addInternalTrans(fromKey, symbolKey, toKey);
      }
      else if( SymbolSet::XMLSymbolTag() == who.get() ) 
      {
        Key symbolKey = getKey(symbol.get());

        //std::cout << "Added trans\n";
        fa->addSymbol(symbolKey);
      }
      else if( NWA::Trans::CallXMLTag() == who.get() ) 
      {
        Key fromKey = getKey(from.get());
        Key symbolKey = getKey(symbol.get());
        Key toKey = getKey(to.get());

        //std::cout << "Added trans\n";
        fa->addCallTrans(fromKey, symbolKey, toKey);
      }
      else if( NWA::Trans::ReturnXMLTag() == who.get() ) 
      {
        Key fromKey = getKey(from.get());
        Key predKey = getKey(pred.get());
        Key symbolKey = getKey(symbol.get());
        Key toKey = getKey(to.get());

        //std::cout << "Added trans\n";
        fa->addReturnTrans(
            fromKey, predKey, symbolKey, toKey);
      }
      else if( NWA::States::XMLStateTag() == who.get() ) 
      {
        //std::cout << "Added state\n";
        //fa->addState( stateKey, weightFactory.getWeight(weightString) );
        fa->addState( stateKey );
        if( isInitial )
          fa->addInitialState(stateKey);
        if( isFinal )
          fa->addFinalState(stateKey);
      }
      else if( NWA::XMLTag() == who.get() ) {
      }
//      else if (fUserHandler.handlesElement(who.get())) {
//        fUserHandler.endElement(uri,localname,qname);
//      }
      else {
        *waliErr << "[ERROR] NwaHandler::endElement - unrecognized element: " 
          << who.get() << std::endl;
        throw who.get();
      }
    }

    void NwaHandler::characters(const XMLCh* const chars, const unsigned int length)
    {
//      fUserHandler.characters(chars,length);
    }

    //////////////////////////////////////////////////
    // Helpers
    //////////////////////////////////////////////////
    void NwaHandler::handleTrans(
        const XMLCh* const uri ATTR_UNUSED, 
        const XMLCh* const localname ATTR_UNUSED, 
        const XMLCh* const qname ATTR_UNUSED, 
        const Attributes& attributes)
    {
      from  = attributes.getValue(fromID);
      pred  = attributes.getValue(predID);
      symbol= attributes.getValue(symbolID);
      to    = attributes.getValue(toID);
    }

    void NwaHandler::handleSymbol(
        const XMLCh* const uri ATTR_UNUSED, 
        const XMLCh* const localname ATTR_UNUSED, 
        const XMLCh* const qname ATTR_UNUSED, 
        const Attributes& attributes)
    {
      symbol = attributes.getValue(symbolID);
    }

    void NwaHandler::handleState(
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

        if( NWA::States::XMLNameAttr() == lname.get() )
        {
          std::string name = val.get();
          assert(name.size());
          stateKey = getKey(name);
          hasName = true;
        }
        else if( NWA::States::XMLInitialAttr() == lname.get() )
        {
          if( std::string("TRUE") == val.get() )
            isInitial = true;
        }
        else if( NWA::States::XMLFinalAttr() == lname.get() )
        {
          if( std::string("TRUE") == val.get() )
            isFinal = true;
        }
        else {
          *waliErr << "[ERROR] NwaHandler - parse error.\n";
          *waliErr << "        " << lname.get() << " = " << val.get() << std::endl;
          throw lname.get();
        }
      }
      assert(hasName);
    }

    void NwaHandler::handleNWA(
        const XMLCh* const uri ATTR_UNUSED, 
        const XMLCh* const localname ATTR_UNUSED, 
        const XMLCh* const qname ATTR_UNUSED, 
        const Attributes& attributes)
    {
    }


    wali::ref_ptr<typename NwaHandler::NWA>
    parseNwa( std::istream & is )
    {
      XERCES_CPP_NAMESPACE_USE

      NwaHandler handler;

      int errorCode = 0;

      // Initialize the XML4C2 system
      try {    
        XMLPlatformUtils::Initialize();
      }   
      catch (const XMLException& toCatch) {       
        std::cerr << "Error during initialization! :\n" 
                  << StrX(toCatch.getMessage()) << std::endl;
        return NULL; 
      }

      SAX2XMLReader* parser = XMLReaderFactory::createXMLReader();

      try {
        parser->setContentHandler(&handler);
        parser->setErrorHandler(&handler);
        IStreamInputSource input(is);
        parser->parse(input);
      }
      catch( const OutOfMemoryException&) {
        std::cerr << "OutOfMemoryException\n";
        errorCode = 2;
      }
      catch (const XMLException& toCatch) {
        std::cerr << "\nAn error occurred\n  Error: "
                  << StrX(toCatch.getMessage())
                  << "\n\n";
        errorCode = 3;
      }
      catch( const SAXException& toCatch ) {
        std::cerr << "Error in Main: " << StrX(toCatch.getMessage()) << std::endl;
        errorCode = 4;
      }

      delete parser;

      XMLPlatformUtils::Terminate();

      if (errorCode == 0) {
        return handler.get();
      }
      else {
        return NULL;
      }
    }


  } // namespace wfa

} // namespace wali

