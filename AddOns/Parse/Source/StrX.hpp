#ifndef STRX_GUARD
#define STRX_GUARD 1

// ---------------------------------------------------------------------------
//  Includes for all the program files to see
// ---------------------------------------------------------------------------
#include <string.h>
#include <iostream>
#include <stdlib.h>
#include <xercesc/util/TransService.hpp>
#include <xercesc/util/XMLString.hpp>


XERCES_CPP_NAMESPACE_USE

// ---------------------------------------------------------------------------
//  This is a simple class that lets us do easy (though not terribly efficient)
//  trancoding of XMLCh data to local code page for display.
// ---------------------------------------------------------------------------
class StrX
{
  public :
    // -----------------------------------------------------------------------
    //  Constructors and Destructor
    // -----------------------------------------------------------------------
    StrX(const XMLCh* const toTranscode=0);

    StrX( const StrX& rhs ); 

    ~StrX();

    StrX& operator=( const XMLCh* const toTranscode );

    bool operator==( std::string s );

    // -----------------------------------------------------------------------
    //  Getter methods
    // -----------------------------------------------------------------------
    const char* localForm() const;

    const char* get() const;

  private :
    // -----------------------------------------------------------------------
    //  Private data members
    //
    //  fLocalForm
    //      This is the local code page form of the string.
    // -----------------------------------------------------------------------
    char*   fLocalForm;
};

inline XERCES_STD_QUALIFIER ostream& operator<<(XERCES_STD_QUALIFIER ostream& target, const StrX& toDump)
{
  target << toDump.localForm();
  return target;
}

#endif // STRX_GUARD 1
