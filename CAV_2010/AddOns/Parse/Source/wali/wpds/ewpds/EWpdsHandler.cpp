/**
 * @author Nicholas Kidd
 * @version $Id$
 */

#include "StrX.hpp"

#include "wali/Common.hpp"
#include "wali/Key.hpp"
#include "wali/IUserHandler.hpp"

#include "wali/wpds/Rule.hpp"

#include "wali/wpds/ewpds/EWpdsHandler.hpp"
#include "wali/wpds/ewpds/EWPDS.hpp"

#include "wali/wpds/fwpds/FWPDS.hpp"
#include "wali/wpds/fwpds/SWPDS.hpp"

#include <xercesc/sax2/Attributes.hpp>


namespace wali
{
  namespace wpds
  {
    namespace ewpds
    {

      //EWpdsHandler::EWpdsHandler( EWPDS* ep, WeightFactory& wf, MergeFnFactory* mf ) :
      EWpdsHandler::EWpdsHandler( IUserHandler& user, EWPDS* ep ) :
        WpdsHandler(user,ep),
        epds(ep)
        //mergeFactory(mf),
        //inMerge(false),
        //mergeString("")
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

      void EWpdsHandler::startElement(  
          const   XMLCh* const    uri,
          const   XMLCh* const    localname,
          const   XMLCh* const    qname,
          const   Attributes&     attributes)
      {
        StrX who(localname);
        if (wali::wpds::ewpds::EWPDS::XMLTag == who.get()) {
          // do nothing
        }
        else if (wali::wpds::fwpds::FWPDS::XMLTag == who.get()) {
          // do nothing
        }
        else if (wali::wpds::fwpds::SWPDS::XMLTag == who.get()) {
          // do nothing
        }
        else {
          WpdsHandler::startElement(uri,localname,qname,attributes);
        }
      }

      void EWpdsHandler::endElement( 
          const XMLCh* const uri,
          const XMLCh* const localname,
          const XMLCh* const qname)
      {
        using wali::Key;
        StrX who(localname);
        if (wali::wpds::ewpds::EWPDS::XMLTag == who.get()) {
          // do nothing
        }
        else if (wali::wpds::fwpds::FWPDS::XMLTag == who.get()) {
          // do nothing
        }
        else if (wali::wpds::fwpds::SWPDS::XMLTag == who.get()) {
          // do nothing
        }
        else if (Rule::XMLTag == who.get()) 
        {
          Key fromKey    = getKey(from.get());
          Key fromStkKey = getKey(fromStack.get());
          Key toKey      = getKey(to.get());
          Key toStk1Key  = getKey(toStack1.get());
          Key toStk2Key  = getKey(toStack2.get());
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

          if (toStk2Key == WALI_EPSILON) 
          {
            //std::cerr << " . Weight S = " << weightString << std::endl;
            get_ewpds().add_rule(
                fromKey, fromStkKey, 
                toKey, toStk1Key, toStk2Key, 
                fUserHandler.getWeight());
          }
          else {
            //std::cerr << " . Weight S = " << weightString << std::endl;
            //std::cerr << " . Merge S = " << mergeString << std::endl;
            sem_elem_t se = fUserHandler.getWeight();
            if (fUserHandler.hasMergeFn())
            {
              get_ewpds().add_rule(
                  fromKey, fromStkKey, 
                  toKey, toStk1Key, toStk2Key, se,
                  fUserHandler.getMergeFn(se));
            }
            else
            {
              get_ewpds().add_rule(
                  fromKey, fromStkKey, 
                  toKey, toStk1Key, toStk2Key, se);
            }
          }
        }
        else {
          WpdsHandler::endElement(uri,localname,qname);
        }
      }

    } // namespace ewpds

  } // namespace wpds

} // namespace wali

