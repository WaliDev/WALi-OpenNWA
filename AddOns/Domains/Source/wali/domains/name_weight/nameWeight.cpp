#include "nameWeight.hpp"

#include <iostream>
#include <sstream>

using namespace wali::domains::name_weight;
using std::cout;
using std::endl;
typedef wali::ref_ptr<NameWeight> name_weight_t;

namespace wali
{
  namespace domains
  {
    namespace name_weight
    {
      
      static NameWeight* convert(wali::SemElem* se)
      {
        NameWeight* nw = dynamic_cast<NameWeight*>(se);
        if (nw == NULL) {
          *waliErr << "[ERROR] Cannot cast to class wali::name_weight::NameWeight.\n";
          se->print( *waliErr << "    " ) << endl;
          assert(false);
        }
        // When done with developement
        // NameWeight* br = static_cast<NameWeight*>(se)
        return nw;
      }

      name_weight_t operator*(name_weight_t a, name_weight_t b)
      {
        return a->Compose(b);
      }

      name_weight_t operator|(name_weight_t a, name_weight_t b)
      {
        return a->Union(b);
      }

      name_weight_t operator&(name_weight_t a, name_weight_t b)
      {
        return a->Intersect(b);
      }
    }
  }
}
// ////////////////////////////
// Members and Con/Destructors
NameWeight::NameWeight(const NameWeight& that) :
  wali::SemElemTensor(that),
  nm1(that.nm1),
  nm2(that.nm2)
{}

NameWeight::NameWeight(int name1, int name2) :
  nm1(name1),
  nm2(name2)
{}
NameWeight::~NameWeight() {}
// ////////////////////////////

name_weight_t NameWeight::Compose( name_weight_t that ) const
{
  return that;
}

name_weight_t NameWeight::Union( name_weight_t that ) const
{
  return that;
}
name_weight_t NameWeight::Intersect( name_weight_t that ) const
{
  return that;
}
bool NameWeight::Equal( name_weight_t that) const
{
  return (nm1 == that->getName1() && nm2 == that->getName2());
}
name_weight_t NameWeight::Transpose() const
{
  return new NameWeight(0,0);
}

wali::sem_elem_t NameWeight::star()
{
  return new NameWeight(0,0);
}

wali::sem_elem_t NameWeight::combine(wali::SemElem* se)
{
  name_weight_t that( convert(se) );
  return Union(that);
}

wali::sem_elem_t NameWeight::extend(wali::SemElem* se)
{
  name_weight_t that( convert(se) );
  return Compose(that);
}

bool NameWeight::equal(wali::SemElem* se) const
{
  name_weight_t that( convert(se) );
  return Equal(that);
}
wali::sem_elem_t NameWeight::one() const
{
  return new NameWeight(-1,-1);
}

wali::sem_elem_t NameWeight::zero() const
{
  return new NameWeight(0,0);
}

std::ostream& NameWeight::print( std::ostream& o ) const
{
  o << "Src: " << nm1 << "\t Target: " << nm2;
  return o;
}

wali::sem_elem_tensor_t NameWeight::transpose()
{
  return Transpose();
}

wali::sem_elem_tensor_t NameWeight::tensor(wali::SemElemTensor* se)
{
  name_weight_t that( convert(se) );
  return that;
}
wali::sem_elem_tensor_t NameWeight::detensor()
{
  return new NameWeight(-1,-1);
}

wali::sem_elem_tensor_t NameWeight::detensorTranspose()
{
  return new NameWeight(-1,-1);
}
