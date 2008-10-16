/**
 * @author Nicholas Kidd
 * @version $Id$
 */

#include "StrX.hpp"
#include "wali/SemElem.hpp"
#include "wali/WeightFactory.hpp"
#include "wali/MergeFnFactory.hpp"
#include "wali/UserFactoryHandler.hpp"

namespace wali
{

  UserFactoryHandler::UserFactoryHandler(WeightFactory* wf,MergeFnFactory* mf)
    : fWeightFactory(wf), fMergeFactory(mf)
  {
    assert(fWeightFactory != NULL);
  }

  UserFactoryHandler::~UserFactoryHandler()
  {
  }

  sem_elem_t UserFactoryHandler::getWeight()
  {
    return fWeight;
  }

  merge_fn_t UserFactoryHandler::getMergeFn()
  {
    return fMergeFn;
  }

  void UserFactoryHandler::endElement(
      const XMLCh* const uri ATTR_UNUSED,
      const XMLCh* const localname,
      const XMLCh* const qname ATTR_UNUSED)
  {
    StrX who(localname);
    if (who.get() == wali::SemElem::XMLTag) 
    {
      fWeight = fWeightFactory->getWeight(getCharactersString());
    }
    else if (who.get() == wali::MergeFn::XMLTag) 
    {
      fMergeFn = fMergeFactory->getMergeFn(getCharactersString());
    }
    else {
      *waliErr << "[ERROR] Unhandled XML element : " << who.get() << std::endl;
      assert(0);
    }
  }

}

