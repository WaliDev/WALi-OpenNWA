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

    public:

      /**
       * If there is a partial order among the client infos, 
       * then this function returns true if *other <= *this and
       * returns the residual client info viz. *other \ *this in the
       * parameter residual.
       * If *other and *this are not related or *other > *this, then the
       * function returns false.
       * For example, if client info is a predicate,
       * then if other->predicate implies this->predicate, then 
       * the function would return true and 
       * residual->predicate = this->predicate /\ not(other->predicate)
      **/
      virtual bool subsumes( ref_ptr<ClientInfo> other, ref_ptr<ClientInfo> & residual ) 
      {
        return false;
      }

    };
  }
}
#endif
