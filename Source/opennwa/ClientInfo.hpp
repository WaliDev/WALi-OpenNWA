#ifndef wali_nwa_ClientInfo_GUARD
#define wali_nwa_ClientInfo_GUARD 1

/**
 * @author Amanda Burton, Evan Driscoll, Aditya Thakur
 */

// ::wali
#include "wali/Countable.hpp"
#include "opennwa/NwaFwd.hpp"

namespace opennwa
{
  /*
   *
   * This class is a placeholder for any additional information that you might want to
   * associate with a state.  It must be extended for the client's specific needs.
   *
   */
  class ClientInfo : public wali::Countable
  { 
  public:
    /// Allocates and returns a new ClientInfo object that is a copy
    /// of this one.
    virtual ClientInfo* clone() = 0;

    /// Clones this ClientInfo object and returns a ref_ptr to it
    /// instead of a normal pointer.
    virtual ref_ptr<ClientInfo> cloneRp()
    {
      return ref_ptr<ClientInfo>(clone());
    }
  };

}


// Yo, Emacs!
// Local Variables:
//   c-file-style: "ellemtel"
//   c-basic-offset: 2
// End:

#endif
