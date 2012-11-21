#ifndef WALI_DOMAINS_GENKILL_SORTED_CONTAINER_ADAPTER
#define WALI_DOMAINS_GENKILL_SORTED_CONTAINER_ADAPTER

#include <algorithm>

namespace wali {
  namespace domains {
    namespace genkill {

      /// SortedContainerSetAdapter is a class that lets you use sorted
      /// containers as a Set for the GenKillXformerTemplate.
      ///
      /// Instances of SortedContainerSetAdapter<BackingType> are identical
      /// to BackingType (except in their type, of course), but
      /// SortedContainerSetAdapter provides several static functions for
      /// performing operations such as comparison and difference.
      ///
      /// (Unfortunately the constructors of BackingType will not be
      /// available, but you can construct a
      /// sortedContainerSetAdapter<BackingType> from a BackingType.)
      template <typename BackingType>
      class SortedContainerSetAdapter
        : public BackingType
      {
      public:
        
        SortedContainerSetAdapter()
        {}

        SortedContainerSetAdapter(BackingType const & other)
          : BackingType(other)
        {}


        static
        SortedContainerSetAdapter
        EmptySet()
        {
          return SortedContainerSetAdapter();
        }
    

        static
        bool
        Eq(SortedContainerSetAdapter const & x, SortedContainerSetAdapter const & y)
        {
          return x == y;
        }


        static
        SortedContainerSetAdapter
        Diff(SortedContainerSetAdapter const & x,
             SortedContainerSetAdapter const & y,
             bool UNUSED_PARAMETER(normalizing) = false)
        {      
          SortedContainerSetAdapter ret;
          std::set_difference(x.begin(), x.end(),
                              y.begin(), y.end(),
                              std::inserter(ret, ret.begin()));
          return ret;
        }
    

        static
        SortedContainerSetAdapter
        Union(SortedContainerSetAdapter const & x,
              SortedContainerSetAdapter const & y)
        {
          SortedContainerSetAdapter ret;
          std::set_union(x.begin(), x.end(),
                         y.begin(), y.end(),
                         std::inserter(ret, ret.begin()));
          return ret;
        }        


        static
        SortedContainerSetAdapter
        Intersect(SortedContainerSetAdapter const & x,
                  SortedContainerSetAdapter const & y )
        {
          SortedContainerSetAdapter ret;
          std::set_intersection(x.begin(), x.end(),
                                y.begin(), y.end(),
                                std::inserter(ret, ret.begin()));
          return ret;
        }        


        std::ostream &
        print(std::ostream& o) const
        {
          o << "{";
          for (typename SortedContainerSetAdapter::const_iterator iter = this->begin();
               iter != this->end(); ++iter)
          {
            o << *iter << ", ";
          }
          o << "}";
          return o;
        }
      };

    } // namespace genkill
  }
}


// Yo, Emacs!
// Local Variables:
//   c-file-style: "ellemtel"
//   c-basic-offset: 2
// End:

#endif /* WALI_DOMAINS_GENKILL_SORTED_CONTAINER_ADAPTER */
