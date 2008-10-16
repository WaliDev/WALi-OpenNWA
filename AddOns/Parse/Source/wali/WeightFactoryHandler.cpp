/**
 * @author Nicholas Kidd
 * @version $Id$
 */

#include "StrX.hpp"
#include "wali/SemElem.hpp"
#include "wali/WeightFactory.hpp"
#include "wali/WeightFactoryHandler.hpp"

namespace wali
{

  WeightFactoryHandler::WeightFactoryHandler(WeightFactory& wf)
    : fWeightFactory(wf)
  {
  }

  WeightFactoryHandler::~WeightFactoryHandler()
  {
  }

  sem_elem_t WeightFactoryHandler::getWeight()
  {
    return se;
  }

  void WeightFactoryHandler::endElement(
      const XMLCh* const uri ATTR_UNUSED,
      const XMLCh* const localname,
      const XMLCh* const qname ATTR_UNUSED)
  {
    StrX who(localname);
    assert( who.get() == wali::SemElem::XMLTag );
    se = fWeightFactory.getWeight(getCharactersString());
  }

}

