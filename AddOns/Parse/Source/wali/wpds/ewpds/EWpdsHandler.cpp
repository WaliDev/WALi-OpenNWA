/*!
 * @author Nicholas Kidd
 */

#include "StrX.hpp"

#include "wali/Common.hpp"
#include "wali/WeightFactory.hpp"
#include "wali/MergeFnFactory.hpp"
#include "wali/Key.hpp"

#include "wali/wpds/Rule.hpp"

#include "wali/wpds/ewpds/EWpdsHandler.hpp"
#include "wali/wpds/ewpds/EWPDS.hpp"

#include <xercesc/sax2/Attributes.hpp>


namespace wali
{
  namespace wpds
  {
    namespace ewpds
    {

      EWpdsHandler::EWpdsHandler( EWPDS* ep, WeightFactory& wf, MergeFnFactory* mf ) :
        WpdsHandler(wf,ep),
        epds(ep),
        mergeFactory(mf),
        inMerge(false),
        mergeString("")
      {
      }

      EWpdsHandler::~EWpdsHandler()
      {
      }

      EWPDS& EWpdsHandler::get_ewpds() {
        { // BEGIN DEBUGGING
          assert(epds);
        } // END DEBUGGING
        return *epds;
      }

      //////////////////////////////////////////////////
      // Parsing handlers overriddend by EWpdsHandler
      //////////////////////////////////////////////////

      void EWpdsHandler::startElement(  const   XMLCh* const    uri,
          const   XMLCh* const    localname,
          const   XMLCh* const    qname,
          const   Attributes&     attributes)
      {
        StrX who(localname);
        if( MergeFn::XMLTag == who.get() ) {
          //std::cerr << "Start MergeFn\n";
          inMerge = true;
          mergeString = "";
        }
        else {
          WpdsHandler::startElement(uri,localname,qname,attributes);
        }
      }

      void EWpdsHandler::endElement( const XMLCh* const uri,
          const XMLCh* const localname,
          const XMLCh* const qname)
      {
        using wali::Key;
        StrX who(localname);
        if( MergeFn::XMLTag == who.get() ) {
          //std::cerr << "Exit MergeFn\n";
          inMerge = false;
        }
        else if( Rule::XMLTag == who.get() ) {
          Key fromKey = getKey(from.get());
          Key fromStkKey = getKey(fromStack.get());
          Key toKey = getKey(to.get());
          Key toStk1Key = getKey(toStack1.get());
          Key toStk2Key = getKey(toStack2.get());
          /*
             std::cout << "Rule\n";
             std::cout << "\t" << from.get() << std::endl;
             std::cout << "\t" << fromStack.get() << std::endl;
             std::cout << "\t" << to.get() << std::endl;
             if( toStack1.get() )
             std::cout << "\t" << toStack1.get() << std::endl;
             if( toStack2.get() )
             std::cout << "\t" << toStack2.get() << std::endl;
             */

          if( toStk2Key == WALI_EPSILON ) {
            //std::cerr << " . Weight S = " << weightString << std::endl;
            get_ewpds().add_rule(fromKey
                , fromStkKey
                , toKey
                , toStk1Key
                , toStk2Key
                , weightFactory.getWeight(weightString)
                );
          }
          else {
            //std::cerr << " . Weight S = " << weightString << std::endl;
            //std::cerr << " . Merge S = " << mergeString << std::endl;
            get_ewpds().add_rule(fromKey
                , fromStkKey
                , toKey
                , toStk1Key
                , toStk2Key
                , weightFactory.getWeight(weightString)
                , mergeFactory->getMergeFn(mergeString)
                );
          }

        }
        else {
          WpdsHandler::endElement(uri,localname,qname);
        }
      }

      void EWpdsHandler::characters(const XMLCh* const chars, const unsigned int length)
      {
        StrX part(chars);
        //std::cerr << "Parsing '" << part.get() << "'\n";
        if( inMerge ) {
          assert( !inWeight );
          mergeString += part.get();
        }
        else {
          WpdsHandler::characters(chars,length);
        }
      }

    } // namespace ewpds

  } // namespace wpds

} // namespace wali

