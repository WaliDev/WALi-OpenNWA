#ifndef wali_wpds_WPDS_HANDLER_GUARD
#define wali_wpds_WPDS_HANDLER_GUARD 1

/**
 * @author Nicholas Kidd
 */

#include "StrX.hpp"

#include "wali/Common.hpp"
#include "wali/IWaliHandler.hpp"

#include "wali/wpds/WPDS.hpp"

#include <string>
#include <map>

namespace wali
{
  //class WeightFactory;
  class IUserHandler;

  namespace wpds
  {
    class WpdsHandler : public IWaliHandler
    {
      public:
        //! For <Function> block metadata
        static const std::string FunctionXMLTag;
        //! A map with keys and values of type std::string
        typedef std::map<std::string,std::string> SSMap;

      public:
        //WpdsHandler( WeightFactory& weightFactory, WPDS* thePds=0 );
        WpdsHandler( IUserHandler& userHandler, WPDS* pds=0 );

        virtual ~WpdsHandler();

        /**
         * @return true if [tag] is equal to one of
         * <ul>
         *  <li> wali::wpds::Rule::XMLTag                  </li>
         *  <li> wali::wpds::WPDS::XMLTag                  </li>
         *  <li> wali::wpds::WpdsHandler::FunctionXMLTag   </li>
         * </ul>
         */
        virtual bool handlesElement(std::string tag);

        /** @return the parsed WPDS */
        WPDS& get();

        /** @return the map from function name to entry node/stack symbol.*/
        SSMap& getEntryMapping() {
          return metaEntry;
        }

        /** @return the map from function name to exit node/stack symbol.*/
        SSMap& getExitMapping() {
          return metaExit;
        }

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
        virtual void characters(const XMLCh *const chars, const XMLSize_t length);

        //////////////////////////////////////////////////
        // Helpers
        //////////////////////////////////////////////////
        virtual void handleRule(
            const XMLCh* const uri, 
            const XMLCh* const localname, 
            const XMLCh* const qname, 
            const Attributes& attributes);

      protected:
        //! For getting weight strings
        //bool inWeight;
        //! Translates the "weight string" into a sem_elem_t
        //WeightFactory& weightFactory;
        /** Holds the textual representation of a weight. */
        //std::string weightString;

        /** The parsed WPDS */
        WPDS* pds;

        /** User defined parser for weights */
        IUserHandler& fUserHandler;

        /** Tag for the from state of a rule */
        XMLCh* fromID;
        /** Tag for the from stack of a rule */
        XMLCh* fromStackID;
        /** Tag for the to state of a rule */
        XMLCh* toID;

        /**
         * Tag for the to stack of a rule. 
         * For a push rule, this should be the entry point of a function. 
         * For a step rule, this is the next intraprocedural target.
         */
        XMLCh* toStack1ID;

        /** 
         * Tag for specifying the second right-hand-side stack symbol.
         * This should be the return point of a function call.
         */
        XMLCh* toStack2ID;

        /**
         * Temporary place holders. These hold the values demarcated by
         * the *ID fields declared above.
         */
        StrX from;
        StrX fromStack;
        StrX to;
        StrX toStack1;
        StrX toStack2;

        

        /////////////////////////////////
        // These fields are used in case the WPDS has meta-information
        // about function entry and exit points. This tag looks like:
        //
        //     <Function name='f' entry='e' exit='x'>
        //     </Function>
        //

        /** For naming the Function entry */
        XMLCh* nameID; 
        /** For naming the Function entry */
        XMLCh* entryID; 
        /** For naming the Function exit */
        XMLCh* exitID; 
        /** map from function name to function exit */
        SSMap metaExit;
        /** map from function name to function exit */
        SSMap metaEntry;

    }; // class WpdsHandler

  } // namespace wpds

} // namespace wali

#endif  // wali_wpds_WPDS_HANDLER_GUARD

