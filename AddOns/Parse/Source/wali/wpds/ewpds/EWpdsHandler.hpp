#ifndef wali_wpds_ewpds_EWPDS_HANDLER_GUARD
#define wali_wpds_ewpds_EWPDS_HANDLER_GUARD 1

/*!
 * @author Nicholas Kidd
 */

#include "wali/Common.hpp"
#include "wali/wpds/WpdsHandler.hpp"
#include "wali/wpds/ewpds/EWPDS.hpp"
#include "StrX.hpp"
#include <xercesc/sax2/DefaultHandler.hpp>
#include <string>

XERCES_CPP_NAMESPACE_USE


namespace wali
{
  class WeightFactory;
  class MergeFnFactory;

  namespace wpds
  {
    namespace ewpds 
    {
      class EWpdsHandler : public WpdsHandler
      {
        public:
          /*! 
           * Must pass pointer to EWPDS. It cannot be created for you.
           */
          EWpdsHandler( EWPDS* pds, 
              WeightFactory& weightFactory, 
              MergeFnFactory* mergeFactory );

          virtual ~EWpdsHandler();

          //! Return the generated PDS
          EWPDS& get_ewpds();

          virtual void startElement(  const   XMLCh* const    uri,
              const   XMLCh* const    localname,
              const   XMLCh* const    qname,
              const   Attributes&     attributes);

          virtual void endElement( const XMLCh* const uri,
              const XMLCh* const localname,
              const XMLCh* const qname);

          virtual void characters(const XMLCh* const chars, const unsigned int length);


        protected:
          // Aliases WpdsHandler::pds but we need
          // a typed EWPDS handle to pass
          // merge functions in
          EWPDS *epds;

          MergeFnFactory* mergeFactory;

          bool inMerge;
          std::string mergeString;

          /*
           *  Inherited from WpdsHandler

           WeightFactory& weightFactory;
           bool inWeight;
           std::string weightString;

           XMLCh* fromID;
           XMLCh* fromStackID;
           XMLCh* toID;
           XMLCh* toStack1ID;
           XMLCh* toStack2ID;

           EWPDS* pds;

           StrX from;
           StrX fromStack;
           StrX to;
           StrX toStack1;
           StrX toStack2;
           */

      }; // class EWpdsHandler

    } // namespace ewpds

  } // namespace wpds

} // namespace wali

#endif  // wali_wpds_ewpds_EWPDS_HANDLER_GUARD

