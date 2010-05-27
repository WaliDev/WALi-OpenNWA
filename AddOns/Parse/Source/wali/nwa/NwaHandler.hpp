#ifndef wali_wfa_WFA_HANDLER_GUARD
#define wali_wfa_WFA_HANDLER_GUARD 1

/**
 * @author Nicholas Kidd
 */

#include "StrX.hpp"

#include "wali/Common.hpp"

#include "wali/IWaliHandler.hpp"

#include "wali/nwa/NWA.hpp"

#include <string>


namespace wali
{
  XERCES_CPP_NAMESPACE_USE

  //class WeightFactory;
  class IUserHandler;

  namespace nwa
  {
    class NwaHandler : public IWaliHandler
    {
      public:
        typedef NWA<> NWA;

        //NwaHandler( WeightFactory& weightFactory ); //commented out in WfaHandler
        NwaHandler( /*IUserHandler& userHandler*/ );

        virtual ~NwaHandler();

        /**
         * @return true if [tag] is equal to one of
         * <ul>
         *  <li> wali::nwa::State::XMLTag </li>
         *  <li> wali::nwa::TransSet::TransXMLTag </li>
         *  <li> wali::nwa::Symbol::XMLTag </li>
         *  <li> wali::nwa::NWA::XMLTag   </li>
         * </ul>
         */
        virtual bool handlesElement(std::string tag);

        /** Return the generated PDS*/
        wali::ref_ptr<NWA> get() const;

        /////////////////////////////////////////////
        // Handler methods                         //
        /////////////////////////////////////////////
        virtual void startElement(  
            const   XMLCh* const    uri,
            const   XMLCh* const    localname,
            const   XMLCh* const    qname,
            const   Attributes&     attributes);

        virtual void endElement( 
            const XMLCh* const uri,
            const XMLCh* const localname,
            const XMLCh* const qname);

        /** 
         * Delegate to IUserHandler.
         * Important to do the delegationg for wali::UserFactoryHandler
         */
        void characters(const XMLCh* const chars, const unsigned int length);

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

        void handleSymbol(
            const XMLCh* const uri
            , const XMLCh* const localname
            , const XMLCh* const qname
            , const Attributes& attributes);

        void handleNWA(
            const XMLCh* const uri
            , const XMLCh* const localname
            , const XMLCh* const qname
            , const Attributes& attributes);


      protected:
        /** For parsing the weight */
//        IUserHandler& fUserHandler;

        /** for getting attrs out of rules*/
        XMLCh* fromID;
        XMLCh* predID;
        XMLCh* symbolID;
        XMLCh* toID;
        XMLCh* nameID;
        //XMLCh* queryID;

        /** the WFA we parse*/
        wali::ref_ptr<NWA> fa;

        /** place holders*/
        StrX from;
        StrX pred;
        StrX symbol;
        StrX to;
        wali::Key stateKey;
        bool isInitial;
        bool isFinal;

    }; // class WfaHandler


    /// This function wraps the NwaHandler above to just take
    /// an IStream, and return an NWA, or NULL if there was
    /// a parsing or other error.
    wali::ref_ptr<typename NwaHandler::NWA>
    parseNwa( std::istream & is );

  } // namespace wfa

} // namespace wali

#endif  // wali_wfa_WFA_HANDLER_GUARD

