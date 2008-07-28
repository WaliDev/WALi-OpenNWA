#ifndef wali_wpds_WPDS_HANDLER_GUARD
#define wali_wpds_WPDS_HANDLER_GUARD 1

/*!
 * @author Nicholas Kidd
 */

#include "StrX.hpp"

#include "wali/Common.hpp"

#include "wali/wpds/WPDS.hpp"

#include <string>
#include <map>

#include <xercesc/sax2/DefaultHandler.hpp>

XERCES_CPP_NAMESPACE_USE

namespace wali
{
  class WeightFactory;

  namespace wpds
  {
    class WpdsHandler : public DefaultHandler
    {
      public:
        //! For starting a <Function> block
        static const std::string FunctionXMLTag;
        //! A map with keys and values of type std::string
        typedef std::map<std::string,std::string> SSMap;

      public:
        WpdsHandler( WeightFactory& weightFactory, WPDS* thePds=0 );

        virtual ~WpdsHandler();

        //! Return the generated PDS
        WPDS& get();

        //! Return the map from function name to entry node/stack symbol.
        SSMap& getEntryMapping() {
          return metaEntry;
        }

        //! Return mthe map from function name to exit node/stack symbol.
        SSMap& getExitMapping() {
          return metaExit;
        }

        //! @return reference to the WeightFactory
        const WeightFactory& getWeightFactory() const;

        virtual void startDocument();

        virtual void endDocument();

        virtual void startElement(  const   XMLCh* const    uri,
            const   XMLCh* const    localname,
            const   XMLCh* const    qname,
            const   Attributes&     attributes);

        virtual void endElement( const XMLCh* const uri,
            const XMLCh* const localname,
            const XMLCh* const qname);

        virtual void characters(const XMLCh* const chars, const unsigned int length);

        virtual void ignorableWhitespace(                               
            const XMLCh* const chars
            , const unsigned int length
            );

        virtual void processingInstruction(   
            const XMLCh* const target
            , const XMLCh* const data
            );

        //////////////////////////////////////////////////
        // Default error handlers
        //////////////////////////////////////////////////
        virtual void warning(const SAXParseException& exc);
        virtual void error(const SAXParseException& exc);
        virtual void fatalError(const SAXParseException& exc);

        //////////////////////////////////////////////////
        // Helpers
        //////////////////////////////////////////////////
        virtual void handleRule(
            const XMLCh* const uri
            , const XMLCh* const localname
            , const XMLCh* const qname
            , const Attributes& attributes);

      protected:
        //! the PDS we parse
        WPDS* pds;
        //! For getting weight strings
        bool inWeight;
        //! Translates the "weight string" into a sem_elem_t
        WeightFactory& weightFactory;
        //! Holds the textual representation of a weight.
        std::string weightString;


        //! Tag for the from state of a rule
        XMLCh* fromID;
        //! Tag for the from stack of a rule
        XMLCh* fromStackID;
        //! Tag for the to state of a rule
        XMLCh* toID;

        /*!
         * Tag for the to stack of a rule. 
         * For a push rule, this should be the entry point of a function. 
         * For a step rule, this is the next intraprocedural target.
         */
        XMLCh* toStack1ID;

        /*! 
         * Tag for specifying the second right-hand-side stack symbol.
         * This should be the return point of a function call.
         */
        XMLCh* toStack2ID;

        /*!
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

        //! For naming the Function entry
        XMLCh* nameID; 
        //! For naming the Function entry
        XMLCh* entryID; 
        //! For naming the Function exit
        XMLCh* exitID; 
        //! map from function name to function exit
        SSMap metaExit;
        //! map from function name to function exit
        SSMap metaEntry;

    }; // class WpdsHandler

  } // namespace wpds

} // namespace wali

#endif  // wali_wpds_WPDS_HANDLER_GUARD

