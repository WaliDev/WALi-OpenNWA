/**
 * @author Nicholas Kidd
 * @version $Id$
 */


#include "wali/IUserHandler.hpp"

namespace wali
{
  IUserHandler::IUserHandler()
  {
  }

  IUserHandler::~IUserHandler()
  {
  }

  bool IUserHandler::handlesElement( std::string tag )
  {
    return (
        (tag == SemElem::XMLTag) ||
        (tag == MergeFn::XMLTag)
        );
  }

  ref_ptr<wpds::Wrapper> IUserHandler::getWrapper()
  {
    return NULL;
  }

}

