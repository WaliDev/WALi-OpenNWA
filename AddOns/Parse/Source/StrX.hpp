#ifndef STRX_GUARD
#define STRX_GUARD 1

/**
 * Class to get std::string from a XMLCh* xerces-c wide char
 * string. Based on googling.
 *
 * @author kidd
 */

#include <string>
#include <iostream>
#include <xercesc/util/TransService.hpp>
#include <xercesc/util/XMLString.hpp>


XERCES_CPP_NAMESPACE_USE

class StrX
{
  public:
    StrX(const XMLCh* const toTranscode=0);

    StrX( const StrX& rhs ); 

    ~StrX();

    StrX& operator=( const XMLCh* const toTranscode );

    bool operator==( std::string s );

    std::string get() const;

  private:
    std::string fStr;
};

inline std::ostream& operator<<(std::ostream& o, const StrX& x)
{
  o << x.get();
  return o;
}

#endif // STRX_GUARD 1

