/*!
 * @version $Id$
 */

#include "StrX.hpp"
#include <string>

StrX::StrX(const XMLCh* const toTranscode) :
  fLocalForm(0)
{
  // Call the private transcoding method
  if( toTranscode )
    fLocalForm = XMLString::transcode(toTranscode);
}

StrX::StrX( const StrX& rhs ) {
  fLocalForm = strdup(rhs.get());
}

StrX::~StrX()
{
  if( fLocalForm )
    XMLString::release(&fLocalForm);
}

  StrX& StrX::operator=( const XMLCh* const toTranscode ) {
    if( fLocalForm )
      XMLString::release(&fLocalForm);
    if(toTranscode)
      fLocalForm = XMLString::transcode(toTranscode);
    return *this;
  }

bool StrX::operator==( std::string s )
{
  std::string tmp(get());
  return s == tmp;
}

const char* StrX::localForm() const
{
  return fLocalForm;
}

const char* StrX::get() const
{
  return fLocalForm;
}

