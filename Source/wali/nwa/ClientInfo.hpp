#ifndef wali_nwa_ClientInfo_GUARD
#define wali_nwa_ClientInfo_GUARD 1

/**
 * @author Amanda Burton
 */

// ::wali
#include "wali/Countable.hpp"

namespace wali
{
  namespace nwa
  {
    /*
     *
     * This class is a placeholder for any additional information that you might want to
     * associate with a state.  It must be extended for the client's specific needs.
     *
     */
    class ClientInfo : public Countable
    { 
      virtual ClientInfo & operator=( const ClientInfo & other )
      { 
        return *this;
      };

      virtual bool operator== (ClientInfo const & other)
      {
        return true;
      }

      virtual ClientInfo* clone()
      {
        return new ClientInfo(*this);
      }

      virtual ref_ptr<ClientInfo> cloneRp()
      {
        return ref_ptr<ClientInfo>(clone());
      }
    };
  }
}
#endif
