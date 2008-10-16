#ifndef wali_QUERY_HANDLER_GUARD
#define wali_QUERY_HANDLER_GUARD 1

/**
 * @author Nicholas Kidd
 */


#include "wali/Common.hpp"

#include "wali/wfa/WFA.hpp"

#include "wali/IWaliHandler.hpp"

XERCES_CPP_NAMESPACE_USE


namespace wali
{
  class WeightFactory;
  class MergeFnFactory;

  namespace wfa {
    class WfaHandler;
  }

  namespace wpds {
    class WpdsHandler;
  }

  class QueryHandler : public IWaliHandler
  {

    public:
      static const std::string XMLTag;
      static const std::string XMLPoststarTag;
      static const std::string XMLPrestarTag;

    public:
      QueryHandler( WeightFactory& weightFactory,MergeFnFactory* mf=NULL );

      /*!
       * QueryHandler takes ownership of the passed in Handlers
       * and will delete them upon exiting.
       */
      QueryHandler(wpds::WpdsHandler* wpdsh , wfa::WfaHandler* wfah ); 

      virtual ~QueryHandler();

      /**
       * @return true if this handler hanldes an XML
       * element with tag [tag].
       */
      virtual bool handlesElement( std::string tag );

      bool queryIsPrestar() const;

      wfa::WfaHandler* getWfaHandler();

      wpds::WpdsHandler* getWpdsHandler();

      /*!
       * Return reference to result of query. Only valid
       * after performing run.
       *
       * @return WFA
       * @see QueryHandler::run
       */
      wfa::WFA& result();

      /*!
       * Runs the query, i.e. prestar or poststar,
       * and returns a reference to the resulting WFA
       *
       * @return WFA
       * @see wali::wfa::WFA
       */
      wfa::WFA& run();

      //////////////////////////////////////////////////
      // Content handlers
      //////////////////////////////////////////////////
      void startDocument();

      void endDocument();

      void startElement(  const   XMLCh* const    uri,
          const   XMLCh* const    localname,
          const   XMLCh* const    qname,
          const   Attributes&     attributes);

      void endElement( const XMLCh* const uri,
          const XMLCh* const localname,
          const XMLCh* const qname);

      void characters(const XMLCh* const chars, const unsigned int length);

      void ignorableWhitespace(                               
          const XMLCh* const chars
          , const unsigned int length
          );

      void processingInstruction(   
          const XMLCh* const target
          , const XMLCh* const data
          );



    protected:
      WeightFactory& wf;
      MergeFnFactory* mf;

      DefaultHandler* currentHandler;
      bool isPrestar;
      wpds::WpdsHandler* pdsHandler;
      wfa::WfaHandler* faHandler;

      //! Result of the query
      wfa::WFA fa;

      XMLCh* typeID;

  }; // class QueryHandler

} // namespace wali

#endif  // wali_QUERY_HANDLER_GUARD

