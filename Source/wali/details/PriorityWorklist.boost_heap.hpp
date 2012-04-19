#ifndef wali_BOOST_HEAP_PRIORITY_WORKLIST_GUARD
#define wali_BOOST_HEAP_PRIORITY_WORKLIST_GUARD 1

/*!
 * @author Nicholas Kidd
 */

#include "wali/Common.hpp"
#include "wali/Worklist.hpp"
#include "wali/wfa/Trans.hpp"
#include <map>
#include <set>
#include <functional>

#include <boost/heap/fibonacci_heap.hpp>
#include <boost/heap/d_ary_heap.hpp>

namespace wali
{
  namespace details
  {
    template<typename Compare>
    struct binary_negate_with_default_constructor : std::binary_negate<Compare>
    {
      binary_negate_with_default_constructor()
        : std::binary_negate<Compare>(Compare())
      {}

      binary_negate_with_default_constructor(Compare const & cmp)
        : std::binary_negate<Compare>(cmp)
      {}

      binary_negate_with_default_constructor(binary_negate_with_default_constructor const & cmp)
        : std::binary_negate<Compare>(cmp)
      {}
    };
    
    
    template<typename Compare>
    class BoostHeapPriorityWorklist : public Worklist<wfa::ITrans>
    {
      // Boost implements max-heaps, not min-heaps. We want to hide this
      // pecularity, so negate the comparison functor.
      typedef binary_negate_with_default_constructor<Compare> NotCompare;
      
    public:

#if 1
      typedef boost::heap::d_ary_heap< wfa::ITrans*,
                                       boost::heap::compare<NotCompare>,
                                       boost::heap::mutable_<true>,
                                       boost::heap::arity<3> > pwl_t;
#else
      typedef boost::heap::fibonacci_heap< wfa::ITrans*,
                                           boost::heap::compare<NotCompare> > pwl_t;
#endif
      
    public:
      BoostHeapPriorityWorklist();

      virtual ~BoostHeapPriorityWorklist();

      virtual bool put( wfa::ITrans *t );

      virtual wfa::ITrans * get();

      virtual bool empty() const;

      virtual void clear();

    protected:
      pwl_t workset;

    private:

      struct HeapHandleData : wali::wfa::ITrans::WorklistData
      {
        typedef typename pwl_t::handle_type Handle;

        Handle handle;

        HeapHandleData(Handle const & h) : handle(h) {}
      };


    }; // class BoostHeapPriorityWorklist


  } // namespace details
} // namespace wali

#include "wali/details/PriorityWorklist.boost_heap.cpp"

// Yo, Emacs!
// Local Variables:
//   c-file-style: "ellemtel"
//   c-basic-offset: 2
// End:


#endif  // wali_BOOST_HEAP_PRIORITY_WORKLIST_GUARD

