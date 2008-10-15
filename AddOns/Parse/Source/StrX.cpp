/**
 * @author kidd
 * @version $Id$
 */

#include "StrX.hpp"
#include <string>

StrX::StrX(const XMLCh* const toTranscode)
{
  // Call the private transcoding method
  if (toTranscode) {
    char* cstr = XMLString::transcode(toTranscode);
    fStr = cstr;
    XMLString::release(&cstr);
  }
}

StrX::StrX( const StrX& rhs ) 
{
  fStr = rhs.get();
}

StrX::~StrX()
{
}

bool StrX::operator==( std::string s )
{
  return s == fStr;
}

std::string StrX::get() const
{
  return fStr;
}

