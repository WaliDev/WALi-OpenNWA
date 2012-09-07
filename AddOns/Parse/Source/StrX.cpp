/**
 * @author kidd
 * @version $Id: StrX.cpp 463 2008-10-16 22:27:58Z kidd $
 */

#include "StrX.hpp"
#include <string>

StrX::StrX(const XMLCh* const toTranscode)
{
  // Keep in sync w/ the below StrX::operator=(...)
  if (toTranscode) {
    char* cstr = XMLString::transcode(toTranscode);
    fStr = cstr;
    XMLString::release(&cstr);
  }
  else { 
    // We treat toTranscode=NULL as assigning this.fStr=""
    fStr = "";
  }
}

StrX::StrX( const StrX& rhs ) 
{
  fStr = rhs.get();
}

StrX::~StrX()
{
}

StrX& StrX::operator=( const XMLCh* const toTranscode )
{
  // Cut-n-paste from StrX::StrX(...)
  if (toTranscode) {
    char* cstr = XMLString::transcode(toTranscode);
    fStr = cstr;
    XMLString::release(&cstr);
  }
  else { 
    // We treat toTranscode=NULL as assigning this.fStr=""
    fStr = "";
  }
  return *this;
}

bool StrX::operator==( std::string s )
{
  return s == fStr;
}

std::string StrX::get() const
{
  return fStr;
}

