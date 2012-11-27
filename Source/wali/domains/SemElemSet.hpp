#ifndef WALI_SEMELEMSET_HPP
#define WALI_SEMELEMSET_HPP

#include "wali/SemElem.hpp"

#include <set>
#include <tr1/unordered_set>

#include <boost/function.hpp>
#include <boost/container/flat_set.hpp>

namespace wali
{
  namespace domains
  {

    /// Represents a set of other SemElem instances. You must provide the
    /// "base" SemElem in the constructor.
    ///
    /// 0 = {}
    /// 1 = {1 of the base SemElem}
    /// + = union
    /// * = pairwise extend (i.e. {a,b} * {c, d} = {a*c, a*d, b*c, b*d})
    class SemElemSet : public SemElem
    {
    public:
      //typedef boost::container::flat_set<sem_elem_t, SemElemRefPtrFastLessThan> ElementSet;
      //typedef std::set<sem_elem_t, SemElemRefPtrFastLessThan> ElementSet;
      typedef std::tr1::unordered_set<sem_elem_t, SemElemRefPtrFastHash, SemElemRefPtrEqual> ElementSet;
      
      typedef ref_ptr<SemElemSet> Ptr;
      typedef ref_ptr<SemElemSet const> ConstPtr;
      
      /// Constructs a SemElemSet with the given base element, which is only
      /// used to get a handle on base_element->one(). The result is semiring
      /// 0.
      SemElemSet(sem_elem_t base_element);
      SemElemSet(sem_elem_t base_element, ElementSet const & elements);
      
      virtual sem_elem_t one() const;
      virtual sem_elem_t zero() const;

      virtual sem_elem_t extend( SemElem * se );
      virtual sem_elem_t combine( SemElem * se );

      virtual bool equal( SemElem * se ) const;

      virtual std::ostream& print( std::ostream & o ) const;

      ElementSet const & getElements() const;

    private:
      sem_elem_t base_one;
      ElementSet elements;
    };


  }
}


// Yo emacs!
// Local Variables:
//     c-file-style: "ellemtel"
//     c-basic-offset: 2
//     indent-tabs-mode: nil
// End:

#endif
