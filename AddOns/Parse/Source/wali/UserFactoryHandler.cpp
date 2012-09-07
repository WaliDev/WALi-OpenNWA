/**
 * @author Nicholas Kidd
 * @version $Id: UserFactoryHandler.cpp 478 2008-11-24 19:33:16Z kidd $
 */

#include "StrX.hpp"
#include "wali/SemElem.hpp"
#include "wali/WeightFactory.hpp"
#include "wali/MergeFnFactory.hpp"
#include "wali/UserFactoryHandler.hpp"

namespace wali
{

  UserFactoryHandler::UserFactoryHandler(WeightFactory* wf,MergeFnFactory* mf)
    : fWeightFactory(wf), fMergeFactory(mf), fHasMergeFn(false)
  {
    assert(fWeightFactory != NULL);
  }

  UserFactoryHandler::~UserFactoryHandler()
  {
  }

  void UserFactoryHandler::startElement(  
      const   XMLCh* const    uri ATTR_UNUSED,
      const   XMLCh* const    localname,
      const   XMLCh* const    qname ATTR_UNUSED,
      const   Attributes&     attributes ATTR_UNUSED)
  {
    StrX who(localname);
    if (!handlesElement(who.get())) {
      *waliErr << "[ERROR] UserFactoryHandler::startElement - unhandled element.\n";
      *waliErr << "        Element is '" << who.get() << "'" << std::endl;
      throw who.get();
    }
    fHasMergeFn = false;
    fCharStr = "";
  }

  void UserFactoryHandler::endElement(
      const XMLCh* const uri ATTR_UNUSED,
      const XMLCh* const localname,
      const XMLCh* const qname ATTR_UNUSED)
  {
    StrX who(localname);
    if (!handlesElement(who.get())) {
      *waliErr << "[ERROR] UserFactoryHandler::endElement - unhandled element.\n";
      *waliErr << "        Element is '" << who.get() << "'" << std::endl;
      throw who.get();
    }
    if (who.get() == wali::SemElem::XMLTag) 
    {
      std::string str = getCharactersString();
      //NAKR -- debugging
      //*waliErr << "##### Weight string == " << str << std::endl;
      fWeight = fWeightFactory->getWeight(str);
    }
    else if (who.get() == wali::MergeFn::XMLTag) 
    {
      if (fMergeFactory == NULL) 
      {
        *waliErr 
          << "[ERROR] UserFactoryHandler::endElement - no MergeFactory given." 
          << std::endl;
        assert(0);
      }
      fMergeFn = fMergeFactory->getMergeFn(getCharactersString());
      fHasMergeFn = true;
    }
    else 
    {
      *waliErr << "[ERROR] Unhandled XML element : " << who.get() << std::endl;
      assert(0);
    }
  }

  sem_elem_t UserFactoryHandler::getWeight()
  {
    return fWeight;
  }

  bool UserFactoryHandler::hasMergeFn()
  {
    if (fMergeFactory == NULL)
      return false;
    else
      return fHasMergeFn;
  }

  merge_fn_t UserFactoryHandler::getMergeFn(sem_elem_t se ATTR_UNUSED)
  {
    return fMergeFn;
  }

}

