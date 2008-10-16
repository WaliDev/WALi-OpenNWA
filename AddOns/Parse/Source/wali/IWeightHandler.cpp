/**
 * @author Nicholas Kidd
 */


#include "wali/Common.hpp"
#include "wali/SemElem.hpp"
#include "wali/IWeightHandler.hpp"

namespace wali
{
  IWeightHandler::IWeightHandler()
  {
  }

  IWeightHandler::~IWeightHandler()
  {
  }

  bool IWeightHandler::handlesElement( std::string str )
  {
    return (str == SemElem::XMLTag);
  }
}
