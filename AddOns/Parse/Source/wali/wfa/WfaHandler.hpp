#ifndef wali_wfa_WFA_HANDLER_GUARD
#define wali_wfa_WFA_HANDLER_GUARD 1

/**
 * @author Nicholas Kidd
 */

#include "StrX.hpp"

#include "wali/Common.hpp"

#include "wali/IWaliHandler.hpp"

#include "wali/wfa/WFA.hpp"

#include <string>

XERCES_CPP_NAMESPACE_USE

namespace wali
{

  class WeightFactory;

  namespace wfa
  {
    class WfaHandler : public IWaliHandler
    {
      public:
        WfaHandler( WeightFactory& weightFactory );

        virtual ~WfaHandler();

        /**
         * @return true if [tag] is equal to one of
         * <ul>
         *  <li> wali::wfa::State::XMLTag </li>
         *  <li> wali::wfa::Trans::XMLTag </li>
         *  <li> wali::wfa::WFA::XMLTag   </li>
         * </ul>
         */
        virtual bool handlesElement(std::string tag);

        //! Return reference to the WeightFactory being used
        WeightFactory& getWeightFactory() const;

        //! Return the generated PDS
        WFA& get();

        /////////////////////////////////////////////
        // Handler methods                         //
        /////////////////////////////////////////////
        virtual void endElement( 
            const XMLCh* const uri,
            const XMLCh* const localname,
            const XMLCh* const qname);

        virtual void characters(const XMLCh* const chars, const unsigned int length);

        virtual void startElement(  
            const   XMLCh* const    uri,
            const   XMLCh* const    localname,
            const   XMLCh* const    qname,
            const   Attributes&     attributes);


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

#endif  // wali_wfa_WFA_HANDLER_GUARD

